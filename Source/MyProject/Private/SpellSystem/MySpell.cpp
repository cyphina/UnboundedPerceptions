#include "MyProject.h"
#include "MySpell.h"
#include "GameplayEffects/CoolDownEffect.h"
#include "GameplayEffects/RTSDamageEffect.h"
#include "SpellDataManager.h"
#include "WorldObjects/Unit.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "BrainComponent.h"
#include "UpStatComponent.h"
#include "DA_DefaultTargetingScheme.h"
#include "GlobalDataAssetsSS.h"
#include "SpellFunctionLibrary.h"
#include "TargetComponent.h"
#include "SpellTargetingTypes.h"
#include "UnitMessages.h"

UMySpell::UMySpell() : UGameplayAbility()
{
   CooldownGameplayEffectClass = UCoolDownEffect::StaticClass();
   InstancingPolicy            = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

TSubclassOf<UUpSpellTargeting> UMySpell::GetTargeting() const
{
   return spellDefaults.Targeting;
}

bool UMySpell::CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
   const FGameplayTagContainer* CooldownTags = GetCooldownTags();
   if(CooldownTags)
   {
      check(ActorInfo->AbilitySystemComponent.IsValid());

      // CooldownTags->Num() > 0 && ActorInfo->AbilitySystemComponent->HasAnyMatchingGameplayTags(*CooldownTags) ||
      if(ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(spellDefaults.nameTag))
      {
         const FGameplayTag& CooldownTag = UAbilitySystemGlobals::Get().ActivateFailCooldownTag;

         if(OptionalRelevantTags && CooldownTag.IsValid())
         {
            OptionalRelevantTags->AddTag(CooldownTag);
         }
         // GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Cooldown checked");
         return false;
      }
   }
   return true;
}

FGameplayEffectContextHandle UMySpell::MakeEffectContext(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const
{
   check(ActorInfo);
   FGameplayEffectContextHandle Context = FGameplayEffectContextHandle(UAbilitySystemGlobals::Get().AllocGameplayEffectContext());
   Context.AddInstigator(ActorInfo->AvatarActor.Get(), ActorInfo->AvatarActor.Get());
   Context.SetAbility(this);
   return Context;
}

void UMySpell::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
   // Our cooldown effects are all the same but we manually add the name tag of this spell to know it is on cooldown.
   if(GetCDDuration(ActorInfo->AbilitySystemComponent.Get()) > 0)
   {
      FGameplayEffectSpecHandle sH = MakeOutgoingGameplayEffectSpec(CooldownGameplayEffectClass, 1);
      UAbilitySystemBlueprintLibrary::SetDuration(sH, GetCDDuration(ActorInfo->AbilitySystemComponent.Get()));
      UAbilitySystemBlueprintLibrary::AddGrantedTag(sH, spellDefaults.nameTag);
      K2_ApplyGameplayEffectSpecToOwner(sH);
   }

   if(AUnit* unitRef = GetAvatarUnit())
   {
      if(GetCost(ActorInfo->AbilitySystemComponent.Get()) > 0)
      {
         unitRef->GetStatComponent()->ModifyStats<false>(unitRef->GetStatComponent()->GetVitalCurValue(EVitals::Mana) - GetCost(unitRef->GetAbilitySystemComponent()),
                                                         EVitals::Mana);
      }
   }
}

float UMySpell::GetCooldownTimeRemaining(const FGameplayAbilityActorInfo* ActorInfo) const
{
   if(ActorInfo->AbilitySystemComponent.IsValid())
   {
      const FGameplayTagContainer c         = FGameplayTagContainer(spellDefaults.nameTag);
      FGameplayEffectQuery const  Query     = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(c);
      TArray<float>               Durations = ActorInfo->AbilitySystemComponent->GetActiveEffectsTimeRemaining(Query);
      if(Durations.Num() > 0) return Durations[Durations.Num() - 1];
   }

   return 0.f;
}

void UMySpell::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                          bool bReplicateEndAbility, bool bWasCancelled)
{
   Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
   if(!bCasterDone)
   {
      const FAIMessage msg(UnitMessages::AIMessage_SpellFinished, this);
      FAIMessage::Send(GetAvatarUnit()->GetUnitController(), msg);
   }
   bCasterDone = false;
}

void UMySpell::FinishBlockingCaster()
{
   if(!bCasterDone)
   {
      const FAIMessage msg(UnitMessages::AIMessage_SpellFinished, this);
      FAIMessage::Send(GetAvatarUnit()->GetUnitController(), msg);
      bCasterDone = true;
   }
}

bool UMySpell::IsOnCD(const UAbilitySystemComponent* abilityComponent) const
{
   if(abilityComponent->HasMatchingGameplayTag(spellDefaults.nameTag))
   {
      return true;
   }
   return false;
}

float UMySpell::GetCDDuration(const UAbilitySystemComponent* abilityComponent) const
{
   return GetSpellDefaultValueChecked(abilityComponent, spellDefaults.Cooldown);
}

int UMySpell::GetRange(UAbilitySystemComponent* abilityComponent) const
{
   return GetSpellDefaultValueChecked(abilityComponent, spellDefaults.Range);
}

int UMySpell::GetReqLevel(const UAbilitySystemComponent* abilityComponent) const
{
   return GetSpellDefaultValueChecked(abilityComponent, spellDefaults.LevelReq);
}

int UMySpell::GetCost(const UAbilitySystemComponent* abilityComponent) const
{
   return GetSpellDefaultValueChecked(abilityComponent, spellDefaults.Cost);
}

TArray<FText> UMySpell::GetPreReqNames() const
{
   TArray<FText> preReqNames;
   for(TSubclassOf<UMySpell> preReqSpell : spellDefaults.PreReqs)
   {
      preReqNames.Add(preReqSpell.GetDefaultObject()->GetSpellDefaults().Name);
   }
   return preReqNames;
}

float UMySpell::GetSpellDuration(const UAbilitySystemComponent* abilityComponent) const
{
   return GetSpellDefaultValueChecked(abilityComponent, spellDefaults.Duration);
}

FDamageScalarStruct UMySpell::GetDamage(const UAbilitySystemComponent* abilityComponent) const
{
   return GetSpellDefaultValueChecked(abilityComponent, spellDefaults.Damage);
}

float UMySpell::GetPeriod(const UAbilitySystemComponent* abilityComponent) const
{
   return GetSpellDefaultValueChecked(abilityComponent, spellDefaults.Period);
}

float UMySpell::GetCastTime(const UAbilitySystemComponent* abilityComponent) const
{
   return GetSpellDefaultValueChecked(abilityComponent, spellDefaults.CastTime);
}

float UMySpell::GetSecondaryTime(const UAbilitySystemComponent* abilityComponent) const
{
   return GetSpellDefaultValueChecked(abilityComponent, spellDefaults.SecondaryTime);
}

float UMySpell::GetAOE(const UAbilitySystemComponent* abilityComponent) const
{
   return GetSpellDefaultValueChecked(abilityComponent, spellDefaults.AOE);
}

int UMySpell::GetLevel(const UAbilitySystemComponent* abilityComponent) const
{
   FGameplayAbilitySpec* spec = GetAbilitySpec(abilityComponent);
   if(spec)
   {
      return spec->Level;
   }
   return 0;
}

FGameplayEffectSpecHandle UMySpell::SetScaling(FGameplayEffectSpecHandle specHandle)
{
   const FDamageScalarStruct damageScalings = GetDamage(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()));
   specHandle =
       UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(specHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Strength"), damageScalings.strength);
   specHandle = UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(specHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Intelligence"),
                                                                              damageScalings.intelligence);
   specHandle =
       UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(specHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Agility"), damageScalings.agility);
   specHandle = UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(specHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Understanding"),
                                                                              damageScalings.understanding);
   return specHandle;
}

FGameplayEffectSpecHandle UMySpell::SetPeriod(FGameplayEffectSpecHandle specHandle, float period)
{
   FGameplayEffectSpec* Spec = specHandle.Data.Get();
   if(Spec)
   {
      Spec->Period = period;
   }
   else
   {
      ABILITY_LOG(Warning, TEXT("UMySpell::SetPeriod called with invalid SpecHandle"));
   }
   return specHandle;
}

FGameplayEffectSpecHandle UMySpell::CreateGameplayEffectFromTableValues(TSubclassOf<UGameplayEffect> effectClass, FGameplayTag name, FGameplayTagContainer assetTags)
{
   UAbilitySystemComponent*  abilityComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetActorInfo().AvatarActor.Get());
   FGameplayEffectSpecHandle effect      = MakeOutgoingGameplayEffectSpec(effectClass, GetAbilityLevel());

   effect.Data->DynamicAssetTags.AddTag(GetElemTag());
   effect.Data->DynamicAssetTags.AddTag(name);
   effect.Data->DynamicAssetTags.AppendTags(assetTags);
   if(effect.Data->Def->DurationPolicy != EGameplayEffectDurationType::Instant)
   {
      effect.Data->Period = GetPeriod(abilityComp);
      // If we don't lock the duration, the duration will be recalculated somewhere in active effect creation ...
      effect.Data->SetDuration(GetSpellDuration(abilityComp), true);
   }

   if(effectClass->IsChildOf(URTSDamageEffect::StaticClass()))
   {
      SetScaling(effect);
   }
   return effect;
}

UEnvQuery* UMySpell::GetDefaultQueryForSpell(UWorld* worldRef)
{
   // ! Not making classes for these since most spell cast usage will have custom targeting anyways
   if(const auto dataAssetSS = worldRef->GetGameInstance()->GetSubsystem<UGlobalDataAssetsSS>())
   {
      if(spellDefaults.descriptionTags.HasTag(FGameplayTag::RequestGameplayTag("Skill.Category.Offensive")))
      {
         return GetTargeting().GetDefaultObject()->GetDefaultQueryForTargetingScheme(dataAssetSS->defaultOffensiveTargetSchemes);
      }
      if(spellDefaults.descriptionTags.HasTag(FGameplayTag::RequestGameplayTag("Skill.Category.Support")))
      {
         return GetTargeting().GetDefaultObject()->GetDefaultQueryForTargetingScheme(dataAssetSS->defaultSupportTargetSchemes);
      }
      if(spellDefaults.descriptionTags.HasTag(FGameplayTag::RequestGameplayTag("Skill.Category.Purge")))
      {
         return GetTargeting().GetDefaultObject()->GetDefaultQueryForTargetingScheme(dataAssetSS->defaultPurgeTargetSchemes);
      }
      if(spellDefaults.descriptionTags.HasTag(FGameplayTag::RequestGameplayTag("Skill.Category.Disable")))
      {
         return GetTargeting().GetDefaultObject()->GetDefaultQueryForTargetingScheme(dataAssetSS->defaultDisableTargetSchemes);
      }
   }
   return nullptr;
}

UAbilitySystemComponent* UMySpell::GetOwnerAbilitySystemComponent() const
{
   return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningActorFromActorInfo());
}

UAbilitySystemComponent* UMySpell::GetAvatarAbilitySystemComponent() const
{
   return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
}

AUnit* UMySpell::GetAvatarUnit() const
{
   return Cast<AUnit>(GetAvatarActorFromActorInfo());
}

FGameplayAbilityTargetingLocationInfo UMySpell::MakeTargetLocationInfoFromTargetComponent()
{
   FGameplayAbilityTargetingLocationInfo targetLocInfo;
   targetLocInfo.LiteralTransform = FTransform(GetAvatarTargetLocation());
   targetLocInfo.SourceAbility    = this;
   targetLocInfo.SourceActor      = GetAvatarActorFromActorInfo();
   return targetLocInfo;
}

FGameplayAbilityTargetingLocationInfo UMySpell::MakeTargetLocationInfoFromLocation(FVector targetLocation)
{
   FGameplayAbilityTargetingLocationInfo targetLocInfo;
   targetLocInfo.LiteralTransform = FTransform(targetLocation);
   targetLocInfo.SourceAbility    = this;
   targetLocInfo.SourceActor      = GetAvatarActorFromActorInfo();
   return targetLocInfo;
}

FGameplayAbilityTargetDataHandle UMySpell::GetAvatarTargetData() const
{
   return GetAvatarUnit()->GetTargetComponent()->GetTargetData();
}

AUnit* UMySpell::GetAvatarTargetUnit() const
{
   return GetAvatarUnit()->GetTargetComponent()->GetTargetUnit();
}

FVector UMySpell::GetAvatarActorLocation() const
{
   return GetAvatarUnit()->GetActorLocation();
}

FVector UMySpell::GetAvatarTargetLocation() const
{
   return GetAvatarUnit()->GetTargetComponent()->GetTargetLocationVisit();
}

TArray<TEnumAsByte<ECollisionChannel>> UMySpell::GetTraceChannelForEnemy() const
{
   AUnit* unit = GetAvatarUnit();
   if(unit->GetIsEnemy())
   {
      return {ALLY_TRACE_CHANNEL};
   }
   else
   {
      return {ENEMY_TRACE_CHANNEL};
   }
}

TArray<TEnumAsByte<ECollisionChannel>> UMySpell::GetTraceChannelForFriendly() const
{
   AUnit* unit = GetAvatarUnit();
   if(unit->GetIsEnemy())
   {
      return {ENEMY_TRACE_CHANNEL};
   }
   else
   {
      return {ALLY_TRACE_CHANNEL};
   }
}

TArray<TEnumAsByte<ECollisionChannel>> UMySpell::GetObjectChannelForEnemy() const
{
   AUnit* unit = GetAvatarUnit();
   if(unit->GetIsEnemy())
   {
      return {ALLY_OBJECT_CHANNEL};
   }
   else
   {
      return {ENEMY_OBJECT_CHANNEL};
   }
}

void UMySpell::ApplyEffectsTargetIfNoMiss(const FGameplayAbilityTargetDataHandle& targetData, const TArray<FGameplayEffectSpecHandle>& effects)
{
   AUnit*                           avatarUnit   = GetAvatarUnit();
   const FGameplayEffectSpecHandle& damageHandle = CreateGameplayEffectFromTableValues(URTSDamageEffect::StaticClass(), FGameplayTag::EmptyTag, FGameplayTagContainer());

   if(damageHandle.IsValid() && HasAuthorityOrPredictionKey(CurrentActorInfo, &CurrentActivationInfo))
   {
      TARGETLIST_SCOPE_LOCK(*CurrentActorInfo->AbilitySystemComponent);

      if(targetData.Data[0].IsValid())
      {
         for(TWeakObjectPtr<AActor> actor : targetData.Data[0]->GetActors())
         {
            if(actor.IsValid())
            {
               if(AUnit* target = Cast<AUnit>(actor.Get()))
               {
                  const float healthBeforeDamageApplied = target->GetStatComponent()->GetVitalCurValue(EVitals::Health);

                  avatarUnit->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*damageHandle.Data.Get(), target->GetAbilitySystemComponent());
                  if(healthBeforeDamageApplied > target->GetStatComponent()->GetVitalCurValue(EVitals::Health))
                  {
                     for(const FGameplayEffectSpecHandle& specHandle : effects)
                     {
                        avatarUnit->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*specHandle.Data.Get(), target->GetAbilitySystemComponent());
                     }
                  }
               }
            }
         }
      }
      else
      {
         ABILITY_LOG(Warning, TEXT("UGameplayAbility::ApplyGameplayEffectSpecToTarget invalid target data passed in. Ability: %s"), *GetPathName());
      }
   }
}

bool UMySpell::IsLastHitAMiss()
{
   return GetAvatarUnit()->GetCombatInfo()->bMissLastHit;
}

TArray<TEnumAsByte<ECollisionChannel>> UMySpell::GetObjectChannelForFriendly() const
{
   AUnit* unit = GetAvatarUnit();
   if(unit->GetIsEnemy())
   {
      return {ENEMY_OBJECT_CHANNEL};
   }
   else
   {
      return {ALLY_OBJECT_CHANNEL};
   }
}

FGameplayAbilitySpec* UMySpell::GetAbilitySpec(const UAbilitySystemComponent* abilityComponent) const
{
   FGameplayAbilitySpec* abilitySpec = const_cast<UAbilitySystemComponent*>(abilityComponent)->FindAbilitySpecFromClass(GetClass());
   ensureMsgf(abilitySpec, TEXT("Ability hasn't been registered to unit"));
   return abilitySpec;
}

int UMySpell::GetIndex(int currentLevel, int numCategories, int maxLevel)
{
   if(numCategories <= 0)
   {
      UE_LOG(LogTemp, Error, TEXT("%s read some out of bounds spell property!"), ANSI_TO_TCHAR(__FUNCTION__));
   }
   if(maxLevel != 0)
   {
      const int index = FMath::CeilToInt(static_cast<float>(numCategories) * currentLevel / maxLevel) - 1;
      return index;
   }
   return -1;
}
