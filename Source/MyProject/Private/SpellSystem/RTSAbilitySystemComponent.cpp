// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BrainComponent.h"

#include "WorldObjects/BaseHero.h"

#include "AbilitySystemGlobals.h"
#include "GameplayEffectCustomApplicationRequirement.h"
#include "MySpell.h"
#include "GameplayCueManager.h"
#include "SpellDataLibrary.h"
#include "SpellDelegateStore.h"
#include "UpStatComponent.h"
#include "SpellSystem/SpellFunctionLibrary.h"
#include "SpellSystem/GameplayEffects/RTSDamageEffect.h"

URTSAbilitySystemComponent::URTSAbilitySystemComponent() : UAbilitySystemComponent()
{
}

void URTSAbilitySystemComponent::BeginPlay()
{
   Super::BeginPlay();
   unitOwnerRef = Cast<AUnit>(GetOwner());

   GiveAbility(FGameplayAbilitySpec(USpellDataManager::GetData().GetSpellClass(USpellFunctionLibrary::CONFIRM_SPELL_TAG)));
   GiveAbility(FGameplayAbilitySpec(USpellDataManager::GetData().GetSpellClass(USpellFunctionLibrary::CONFIRM_SPELL_TARGET_TAG)));

   // TODO: Set this more elegantly I guess.
   if(Cast<ABaseHero>(GetOwner())) {
      abilities.SetNum(6);
   }
}

int URTSAbilitySystemComponent::FindSlotIndexOfSpell(TSubclassOf<UMySpell> spellToLookFor) const
{
   return abilities.Find(spellToLookFor);
}

bool URTSAbilitySystemComponent::CanCast(TSubclassOf<UMySpell> spellToCheck) const
{
   UMySpell* spell = spellToCheck.GetDefaultObject();

   if(IsValid(spell) && GetActivatableAbilities().ContainsByPredicate([&spellToCheck](const FGameplayAbilitySpec& registeredAbilitySpec) {
         return registeredAbilitySpec.Ability == spellToCheck.GetDefaultObject();
      })) {
      if(spell->GetCost(this) <= unitOwnerRef->FindComponentByClass<UUpStatComponent>()->GetVitalCurValue(EVitals::Mana)) {
         if(!spell->IsOnCD(this) && !USpellDataLibrary::IsStunned(this) && !USpellDataLibrary::IsSilenced(this)) { return true; }
      }
   }
   return false;
}

FActiveGameplayEffectHandle URTSAbilitySystemComponent::ApplyGameplayEffectSpecToSelf(const FGameplayEffectSpec& Spec, FPredictionKey PredictionKey)
{
   // Scope lock the container after the addition has taken place to prevent the new effect from potentially getting mangled during the remainder
   // of the add operation
   FScopedActiveGameplayEffectLock ScopeLock(ActiveGameplayEffects);

   FScopeCurrentGameplayEffectBeingApplied ScopedGEApplication(&Spec, this);

   const bool bIsNetAuthority = IsOwnerActorAuthoritative();

   // Check Network Authority
   if(!HasNetworkAuthorityToApplyGameplayEffect(PredictionKey)) { return FActiveGameplayEffectHandle(); }

   // Don't allow prediction of periodic effects
   if(PredictionKey.IsValidKey() && Spec.GetPeriod() > 0.f) {
      if(IsOwnerActorAuthoritative()) {
         // Server continue with invalid prediction key
         PredictionKey = FPredictionKey();
      } else {
         // Client just return now
         return FActiveGameplayEffectHandle();
      }
   }

   // Are we currently immune to this? (ApplicationImmunity)
   const FActiveGameplayEffect* ImmunityGE = nullptr;
   if(ActiveGameplayEffects.HasApplicationImmunityToSpec(Spec, ImmunityGE)) {
      OnImmunityBlockGameplayEffect(Spec, ImmunityGE);
      return FActiveGameplayEffectHandle();
   }

   // Check AttributeSet requirements: make sure all attributes are valid
   // We may want to cache this off in some way to make the runtime check quicker.
   // We also need to handle things in the execution list
   for(const FGameplayModifierInfo& Mod : Spec.Def->Modifiers) {
      if(!Mod.Attribute.IsValid()) {
         ABILITY_LOG(Warning, TEXT("%s has a null modifier attribute."), *Spec.Def->GetPathName());
         return FActiveGameplayEffectHandle();
      }
   }

   // check if the effect being applied actually succeeds
   float ChanceToApply = Spec.GetChanceToApplyToTarget();
   if((ChanceToApply < 1.f - SMALL_NUMBER) && (FMath::FRand() > ChanceToApply)) { return FActiveGameplayEffectHandle(); }

   // Get MyTags.
   //	We may want to cache off a GameplayTagContainer instead of rebuilding it every time.
   //	But this will also be where we need to merge in context tags? (Headshot, executing ability, etc?)
   //	Or do we push these tags into (our copy of the spec)?

   {
      // Note: static is ok here since the scope is so limited, but wider usage of MyTags is not safe since this function can be recursively called
      static FGameplayTagContainer MyTags;
      MyTags.Reset();

      GetOwnedGameplayTags(MyTags);

      if(Spec.Def->ApplicationTagRequirements.RequirementsMet(MyTags) == false) { return FActiveGameplayEffectHandle(); }
   }

   // Custom application requirement check
   for(const TSubclassOf<UGameplayEffectCustomApplicationRequirement>& AppReq : Spec.Def->ApplicationRequirements) {
      if(*AppReq && AppReq->GetDefaultObject<UGameplayEffectCustomApplicationRequirement>()->CanApplyGameplayEffect(Spec.Def, Spec, this) == false) {
         return FActiveGameplayEffectHandle();
      }
   }

   // Clients should treat predicted instant effects as if they have infinite duration. The effects will be cleaned up later.
   bool bTreatAsInfiniteDuration =
       GetOwnerRole() != ROLE_Authority && PredictionKey.IsLocalClientKey() && Spec.Def->DurationPolicy == EGameplayEffectDurationType::Instant;

   // Make sure we create our copy of the spec in the right place
   // We initialize the FActiveGameplayEffectHandle here with INDEX_NONE to handle the case of instant GE
   // Initializing it like this will set the bPassedFiltersAndWasExecuted on the FActiveGameplayEffectHandle to true so we can know that we applied a GE
   FActiveGameplayEffectHandle MyHandle(INDEX_NONE);
   bool                        bInvokeGameplayCueApplied =
       Spec.Def->DurationPolicy !=
       EGameplayEffectDurationType::Instant; // Cache this now before possibly modifying predictive instant effect to infinite duration effect.
   bool bFoundExistingStackableGE = false;

   FActiveGameplayEffect* AppliedEffect = nullptr;

   FGameplayEffectSpec*            OurCopyOfSpec = nullptr;
   TSharedPtr<FGameplayEffectSpec> StackSpec;
   {
      if(Spec.Def->DurationPolicy != EGameplayEffectDurationType::Instant || bTreatAsInfiniteDuration) {
         AppliedEffect = ActiveGameplayEffects.ApplyGameplayEffectSpec(Spec, PredictionKey, bFoundExistingStackableGE);
         if(!AppliedEffect) { return FActiveGameplayEffectHandle(); }

         MyHandle      = AppliedEffect->Handle;
         OurCopyOfSpec = &(AppliedEffect->Spec);

         // Log results of applied GE spec
         if(UE_LOG_ACTIVE(VLogAbilitySystem, Log)) {
            ABILITY_VLOG(OwnerActor, Log, TEXT("Applied %s"), *OurCopyOfSpec->Def->GetFName().ToString());

            for(FGameplayModifierInfo Modifier : Spec.Def->Modifiers) {
               float Magnitude = 0.f;
               Modifier.ModifierMagnitude.AttemptCalculateMagnitude(Spec, Magnitude);
               ABILITY_VLOG(OwnerActor, Log, TEXT("         %s: %s %f"), *Modifier.Attribute.GetName(), *EGameplayModOpToString(Modifier.ModifierOp), Magnitude);
            }
         }
      }

      if(!OurCopyOfSpec) {
         StackSpec     = TSharedPtr<FGameplayEffectSpec>(new FGameplayEffectSpec(Spec));
         OurCopyOfSpec = StackSpec.Get();
         UAbilitySystemGlobals::Get().GlobalPreGameplayEffectSpecApply(*OurCopyOfSpec, this);
         OurCopyOfSpec->CaptureAttributeDataFromTarget(this);
      }

      // if necessary add a modifier to OurCopyOfSpec to force it to have an infinite duration
      if(bTreatAsInfiniteDuration) {
         // This should just be a straight set of the duration float now
         OurCopyOfSpec->SetDuration(UGameplayEffect::INFINITE_DURATION, true);
      }
   }

   if(OurCopyOfSpec) {
      // Update (not push) the global spec being applied [we want to switch it to our copy, from the const input copy)
      UAbilitySystemGlobals::Get().SetCurrentAppliedGE(OurCopyOfSpec);
   }

   // We still probably want to apply tags and stuff even if instant?
   // If bSuppressStackingCues is set for this GameplayEffect, only add the GameplayCue if this is the first instance of the GameplayEffect
   if(!bSuppressGameplayCues && bInvokeGameplayCueApplied && AppliedEffect && !AppliedEffect->bIsInhibited &&
      (!bFoundExistingStackableGE || !Spec.Def->bSuppressStackingCues)) {
      // We both added and activated the GameplayCue here.
      // On the client, who will invoke the gameplay cue from an OnRep, he will need to look at the StartTime to determine
      // if the Cue was actually added+activated or just added (due to relevancy)

      // Fixme: what if we wanted to scale Cue magnitude based on damage? E.g, scale an cue effect when the GE is buffed?

      if(OurCopyOfSpec->StackCount > Spec.StackCount) {
         // Because PostReplicatedChange will get called from modifying the stack count
         // (and not PostReplicatedAdd) we won't know which GE was modified.
         // So instead we need to explicitly RPC the client so it knows the GC needs updating
         UAbilitySystemGlobals::Get().GetGameplayCueManager()->InvokeGameplayCueAddedAndWhileActive_FromSpec(this, *OurCopyOfSpec, PredictionKey);
      } else {
         // Otherwise these will get replicated to the client when the GE gets added to the replicated array
         InvokeGameplayCueEvent(*OurCopyOfSpec, EGameplayCueEvent::OnActive);
         InvokeGameplayCueEvent(*OurCopyOfSpec, EGameplayCueEvent::WhileActive);
      }
   }

   // Execute the GE at least once (if instant, this will execute once and be done. If persistent, it was added to ActiveGameplayEffects above)

   // Execute if this is an instant application effect
   if(bTreatAsInfiniteDuration) {
      // This is an instant application but we are treating it as an infinite duration for prediction. We should still predict the execute GameplayCUE.
      // (in non predictive case, this will happen inside ::ExecuteGameplayEffect)

      if(!bSuppressGameplayCues) { UAbilitySystemGlobals::Get().GetGameplayCueManager()->InvokeGameplayCueExecuted_FromSpec(this, *OurCopyOfSpec, PredictionKey); }
   } else if(Spec.Def->DurationPolicy == EGameplayEffectDurationType::Instant) {
      if(OurCopyOfSpec->Def->OngoingTagRequirements.IsEmpty()) {
         ExecuteGameplayEffect(*OurCopyOfSpec, PredictionKey);
      } else {
         ABILITY_LOG(
             Warning,
             TEXT(
                 "%s is instant but has tag requirements. Tag requirements can only be used with gameplay effects that have a duration. This gameplay effect will be ignored."),
             *Spec.Def->GetPathName());
      }
   }

   if(Spec.GetPeriod() != UGameplayEffect::NO_PERIOD && Spec.TargetEffectSpecs.Num() > 0) {
      ABILITY_LOG(Warning, TEXT("%s is periodic but also applies GameplayEffects to its target. GameplayEffects will only be applied once, not every period."),
                  *Spec.Def->GetPathName());
   }

   // ! Here's where I modified the code
   // TODO: Add RNG to Removal
   // ------------------------------------------------------
   //	Remove gameplay effects with tags
   //		Remove any active gameplay effects that match the RemoveGameplayEffectsWithTags in the definition for this spec
   //		Only call this if we are the Authoritative owner and we have some RemoveGameplayEffectsWithTags.CombinedTag to remove
   // ------------------------------------------------------
   if(bIsNetAuthority && Spec.Def->RemoveGameplayEffectsWithTags.CombinedTags.Num() > 0) {
      // Clear tags is always removing all stacks.
      FGameplayEffectQuery ClearQuery = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(Spec.Def->RemoveGameplayEffectsWithTags.CombinedTags);
      if(MyHandle.IsValid()) { ClearQuery.IgnoreHandles.Add(MyHandle); }

      // way to add a number to how many buffs get purged
      FGameplayTag purgeDesc =
          Spec.Def->InheritableGameplayEffectTags.CombinedTags.Filter(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Effect.Purge"))).First();

      TArray<FActiveGameplayEffectHandle> removableEffects = GetActiveEffects(ClearQuery);
      for(int i = 0; i < USpellDataLibrary::purgeTagMap[purgeDesc] && i < removableEffects.Num(); ++i) {
         // TODO: Do some kind of roll to see if it the spell is successfully purged
         // GetActiveGameplayEffect(activeEffectHandle)->Spec.GetSetByCallerMagnitude()
         RemoveActiveGameplayEffect(removableEffects[i]);
      }
   }

   // ------------------------------------------------------
   // Apply Linked effects
   // todo: this is ignoring the returned handles, should we put them into a TArray and return all of the handles?
   // ------------------------------------------------------
   for(const FGameplayEffectSpecHandle TargetSpec : Spec.TargetEffectSpecs) {
      if(TargetSpec.IsValid()) { ApplyGameplayEffectSpecToSelf(*TargetSpec.Data.Get(), PredictionKey); }
   }

   UAbilitySystemComponent* InstigatorASC = Spec.GetContext().GetInstigatorAbilitySystemComponent();

   // Send ourselves a callback
   OnGameplayEffectAppliedToSelf(InstigatorASC, *OurCopyOfSpec, MyHandle);

   // Send the instigator a callback
   if(InstigatorASC) { InstigatorASC->OnGameplayEffectAppliedToTarget(this, *OurCopyOfSpec, MyHandle); }

   return MyHandle;
}

TSubclassOf<UMySpell> URTSAbilitySystemComponent::GetSpellAtSlot(int index) const
{
   if(index >= 0 && index < abilities.Num()) return abilities[index];
   return nullptr;
}

void URTSAbilitySystemComponent::SetSpellAtSlot(TSubclassOf<UMySpell> spellClassToSet, int slotIndex)
{
   if(slotIndex >= 0 && slotIndex < abilities.Num())
   {
      abilities[slotIndex] = spellClassToSet;
      SpellHUDEvents::OnSpellSlotReplacedEvent.Broadcast(unitOwnerRef, spellClassToSet, slotIndex);
   }
   else
   {
      UE_LOG(LogTemp, Error, TEXT("Tried to set spell on unit named: %s on a spell slot that is nonexistent!"), *unitOwnerRef->GetGameName().ToString());
   }
}

void URTSAbilitySystemComponent::TryRemoveInvisibility()
{
   const bool invis = HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Invisibility"));
   if(invis) RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Effect.Invisibility")));
}

void URTSAbilitySystemComponent::ApplyDamageToSelf(FDamageScalarStruct damageScalars, FGameplayTag attackElement)
{
   const auto damageEffect = MakeDamageEffect(damageScalars, attackElement);
   ApplyGameplayEffectSpecToSelf(*damageEffect.Data.Get(), FPredictionKey());
}

void URTSAbilitySystemComponent::ApplyDamageToTarget(URTSAbilitySystemComponent* targetComponent, FDamageScalarStruct damageScalars, FGameplayTag attackElement)
{
   const auto damageEffect = MakeDamageEffect(damageScalars, attackElement);
   ApplyGameplayEffectSpecToTarget(*damageEffect.Data.Get(), targetComponent);
}

FGameplayAbilitySpec* URTSAbilitySystemComponent::FindAbilitySpecFromClass(TSubclassOf<UGameplayAbility> InAbilityClass)
{
   return const_cast<FGameplayAbilitySpec*>(AsConst(this)->FindAbilitySpecFromClass(InAbilityClass));
}

const FGameplayAbilitySpec* URTSAbilitySystemComponent::FindAbilitySpecFromClass(TSubclassOf<UGameplayAbility> InAbilityClass) const
{
   for(const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items) {
      if(Spec.Ability->GetClass() == InAbilityClass) { return &Spec; }
   }
   return nullptr;
}

FGameplayEffectSpecHandle URTSAbilitySystemComponent::MakeDamageEffect(FDamageScalarStruct damageScalars, FGameplayTag attackElement)
{
   FGameplayEffectContextHandle context = MakeEffectContext();
   context.AddInstigator(GetOwner(), GetOwner());
   const FGameplayEffectSpecHandle damageEffectHandle = MakeOutgoingSpec(URTSDamageEffect::StaticClass(), 1, context);

   // Set all the effect's custom magnitude values else it complains
   UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Strength"), damageScalars.strength);
   UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Intelligence"),
                                                                 damageScalars.intelligence);
   UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Understanding"),
                                                                 damageScalars.understanding);
   UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Agility"), damageScalars.agility);
   UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Health"), damageScalars.hitpoints);
   UAbilitySystemBlueprintLibrary::AddAssetTag(damageEffectHandle, attackElement);

   return damageEffectHandle;
}
