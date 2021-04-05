#include "MyProject.h"
#include "MySpell.h"
#include "GameplayEffects/CoolDownEffect.h"
#include "GameplayEffects/RTSDamageEffect.h"
#include "SpellDataManager.h"
#include "WorldObjects/Unit.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
<<<<<<< HEAD
#include "UpStatComponent.h"
#include "DA_DefaultTargetingScheme.h"
#include "GlobalDataAssetsSS.h"
=======
#include "BrainComponent.h"
#include "UpStatComponent.h"
#include "DA_DefaultTargetingScheme.h"
#include "GlobalDataAssetsSS.h"
#include "SpellDataLibrary.h"
#include "SpellFunctionLibrary.h"
#include "TargetComponent.h"
#include "SpellTargetingTypes.h"
#include "UnitMessages.h"
>>>>>>> componentrefactor

UMySpell::UMySpell() : UGameplayAbility()
{
   CooldownGameplayEffectClass = UCoolDownEffect::StaticClass();
<<<<<<< HEAD
=======
   InstancingPolicy            = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UMySpell::HasSpellSpecificResources_Implementation(UAbilitySystemComponent* ASC) const
{
   return true;
}

FText UMySpell::GetMessageDeficientResources_Implementation() const
{
   return FText::GetEmpty();
>>>>>>> componentrefactor
}

TSubclassOf<UUpSpellTargeting> UMySpell::GetTargeting() const
{
   return spellDefaults.Targeting;
}

bool UMySpell::CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
   const FGameplayTagContainer* CooldownTags = GetCooldownTags();
<<<<<<< HEAD
   if(CooldownTags) {
=======
   if(CooldownTags)
   {
>>>>>>> componentrefactor
      check(ActorInfo->AbilitySystemComponent.IsValid());

      // CooldownTags->Num() > 0 && ActorInfo->AbilitySystemComponent->HasAnyMatchingGameplayTags(*CooldownTags) ||
<<<<<<< HEAD
      if(ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(AbilityTags.GetByIndex(0))) {
         const FGameplayTag& CooldownTag = UAbilitySystemGlobals::Get().ActivateFailCooldownTag;

         if(OptionalRelevantTags && CooldownTag.IsValid()) { OptionalRelevantTags->AddTag(CooldownTag); }
=======
      if(ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(spellDefaults.nameTag))
      {
         const FGameplayTag& CooldownTag = UAbilitySystemGlobals::Get().ActivateFailCooldownTag;

         if(OptionalRelevantTags && CooldownTag.IsValid())
         {
            OptionalRelevantTags->AddTag(CooldownTag);
         }
>>>>>>> componentrefactor
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
<<<<<<< HEAD
   //ApplyCooldown(Handle, ActorInfo, ActivationInfo);
   //Our version of applying the cooldown
   FGameplayEffectSpecHandle sH = MakeOutgoingGameplayEffectSpec(CooldownGameplayEffectClass, 1);
   UAbilitySystemBlueprintLibrary::SetDuration(sH, GetCDDuration(ActorInfo->AbilitySystemComponent.Get()));
   UAbilitySystemBlueprintLibrary::AddGrantedTag(sH, AbilityTags.GetByIndex(0));
   //ActorInfo->AbilitySystemComponent.GetData()->AddLooseGameplayTag(AbilityTags.GetByIndex(0));
   //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FromInt(GetCDDuration(ActorInfo->AbilitySystemComponent.GetData())));
   K2_ApplyGameplayEffectSpecToOwner(sH);

   unitRef = Cast<AUnit>(ActorInfo->AvatarActor.Get());
   if(unitRef) {
      unitRef->GetStatComponent()->ModifyStats<false>(unitRef->GetStatComponent()->GetVitalCurValue(EVitals::Mana) - GetCost(unitRef->GetAbilitySystemComponent()),
                                                      EVitals::Mana);
      // GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString("Committing ability for unit:") + unitRef->GetName());
=======
   // Our cooldown effects are all the same but we manually add the name tag of this spell to know it is on cooldown.
   if(GetCDDuration(ActorInfo->AbilitySystemComponent.Get()) > 0)
   {
      FGameplayEffectSpecHandle sH = MakeOutgoingGameplayEffectSpec(CooldownGameplayEffectClass, 1);
      UAbilitySystemBlueprintLibrary::SetDuration(sH, GetCDDuration(ActorInfo->AbilitySystemComponent.Get()));
      UAbilitySystemBlueprintLibrary::AddGrantedTag(sH, spellDefaults.nameTag);
      K2_ApplyGameplayEffectSpecToOwner(sH);
>>>>>>> componentrefactor
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
<<<<<<< HEAD
   if(ActorInfo->AbilitySystemComponent.IsValid()) {
      const FGameplayTag         cdTag     = AbilityTags.GetByIndex(0);
      FGameplayTagContainer      c         = FGameplayTagContainer(cdTag);
      FGameplayEffectQuery const Query     = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(c);
      TArray<float>              Durations = ActorInfo->AbilitySystemComponent->GetActiveEffectsTimeRemaining(Query);
=======
   if(ActorInfo->AbilitySystemComponent.IsValid())
   {
      const FGameplayTagContainer c         = FGameplayTagContainer(spellDefaults.nameTag);
      FGameplayEffectQuery const  Query     = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(c);
      TArray<float>               Durations = ActorInfo->AbilitySystemComponent->GetActiveEffectsTimeRemaining(Query);
>>>>>>> componentrefactor
      if(Durations.Num() > 0) return Durations[Durations.Num() - 1];
   }

   return 0.f;
}

<<<<<<< HEAD
bool UMySpell::IsOnCD(const UAbilitySystemComponent* abilityComponent) const
{
   if(abilityComponent->HasMatchingGameplayTag(AbilityTags.GetByIndex(0))) return true;
   return false;
=======
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
>>>>>>> componentrefactor
}

void UMySpell::AddCallbackToEffectRemoval(UAbilitySystemComponent* ASC, FGameplayTag EffectName, FName FunctionToCallName)
{
<<<<<<< HEAD
   FGameplayAbilitySpec* abilitySpec = abilityComponent->FindAbilitySpecFromClass(GetClass());
   // GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, GetName().ToString());
   // checkf(abilitySpec != nullptr, TEXT("Ability hasn't been registered to unit"));
   if(!abilitySpec) {
      UE_LOG(LogTemp, Warning, TEXT("%s"),
             *(FString("No ability system component found") + FString::FromInt(abilityComponent->GetActivatableAbilities().Num()) + GetName().ToString()));
      return -1;
=======
   TArray<FActiveGameplayEffectHandle> effectsToListenTo = ASC->GetActiveEffectsWithAllTags(EffectName.GetSingleTagContainer());
   for(const FActiveGameplayEffectHandle effectToListenToHandle : effectsToListenTo)
   {
      ASC->OnGameplayEffectRemoved_InfoDelegate(effectToListenToHandle)->AddUFunction(this, FunctionToCallName);
>>>>>>> componentrefactor
   }
}

<<<<<<< HEAD
   int numUpgrades = USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->cdDuration.Num();
   return USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->cdDuration[GetIndex(abilitySpec->Level, numUpgrades, GetMaxLevel())];
=======
void UMySpell::RemoveEffectWtihNameTagFromAvatar(FGameplayTag EffectName, int StacksToRemove, int NumEffectsToRemove)
{
   for(int i = 0; i < NumEffectsToRemove; ++i)
   {
      USpellDataLibrary::RemoveEffectWtihNameTag(GetAvatarAbilitySystemComponent(), EffectName, StacksToRemove);
   }
>>>>>>> componentrefactor
}

void UMySpell::FinishBlockingCaster()
{
<<<<<<< HEAD
   FGameplayAbilitySpec* abilitySpec = abilityComponent->FindAbilitySpecFromClass(GetClass());
   checkf(abilitySpec, TEXT("Ability hasn't been registered to unit"));
   int numUpgrades = USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->range.Num();
   if(numUpgrades > 0) return USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->range[GetIndex(abilitySpec->Level, numUpgrades, GetMaxLevel())];
   return 0;
=======
   if(!bCasterDone)
   {
      const FAIMessage msg(UnitMessages::AIMessage_SpellFinished, this);
      FAIMessage::Send(GetAvatarUnit()->GetUnitController(), msg);
      bCasterDone = true;
   }
>>>>>>> componentrefactor
}

bool UMySpell::IsOnCD(const UAbilitySystemComponent* abilityComponent) const
{
<<<<<<< HEAD
   FGameplayAbilitySpec* abilitySpec = abilityComponent->FindAbilitySpecFromClass(GetClass());
   checkf(abilitySpec, TEXT("Ability hasn't been registered to unit"));
   int numUpgrades = USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->reqLevel.Num();
   return USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->reqLevel[GetIndex(abilitySpec->Level, numUpgrades, GetMaxLevel())];
}

int UMySpell::GetCost(const URTSAbilitySystemComponent* abilityComponent) const
{
   const FGameplayAbilitySpec* abilitySpec = abilityComponent->FindAbilitySpecFromClass(GetClass());
   checkf(abilitySpec, TEXT("Ability hasn't been registered to unit"));
   const int numUpgrades = USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->cost.Num();
   if(numUpgrades > 0) return USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->cost[GetIndex(abilitySpec->Level, numUpgrades, GetMaxLevel())];
   return 0;
=======
   if(abilityComponent->HasMatchingGameplayTag(spellDefaults.nameTag))
   {
      return true;
   }
   return false;
}

float UMySpell::GetCDDuration(const UAbilitySystemComponent* abilityComponent) const
{
   return GetSpellDefaultValueChecked(abilityComponent, spellDefaults.Cooldown);
>>>>>>> componentrefactor
}

int UMySpell::GetRange(UAbilitySystemComponent* abilityComponent) const
{
<<<<<<< HEAD
   FGameplayAbilitySpec* abilitySpec = abilityComponent->FindAbilitySpecFromClass(GetClass());
   checkf(abilitySpec, TEXT("Ability hasn't been registered to unit"));
   int numUpgrades = USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->duration.Num();
   if(numUpgrades > 0) return USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->duration[GetIndex(abilitySpec->Level, numUpgrades, GetMaxLevel())];
   return 0;
=======
   return GetSpellDefaultValueChecked(abilityComponent, spellDefaults.Range);
}

int UMySpell::GetReqLevel(const UAbilitySystemComponent* abilityComponent) const
{
   return GetSpellDefaultValueChecked(abilityComponent, spellDefaults.LevelReq);
}

int UMySpell::GetCost(const UAbilitySystemComponent* abilityComponent) const
{
   return GetSpellDefaultValueChecked(abilityComponent, spellDefaults.Cost);
>>>>>>> componentrefactor
}

TArray<FText> UMySpell::GetPreReqNames() const
{
<<<<<<< HEAD
   FGameplayAbilitySpec* abilitySpec = abilityComponent->FindAbilitySpecFromClass(GetClass());
   checkf(abilitySpec, TEXT("Ability hasn't been registered to unit"));
   const int           numUpgrades         = USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->damage.Num() / 4;
   FDamageScalarStruct damageScalerScalars = FDamageScalarStruct();
   if(numUpgrades > 0) {
      const int damageOffsetIndex       = GetIndex(abilitySpec->Level, numUpgrades, GetMaxLevel());
      damageScalerScalars.strength      = USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->damage[NUM_SCALING_DAMAGE_ATT * damageOffsetIndex];
      damageScalerScalars.intelligence  = USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->damage[NUM_SCALING_DAMAGE_ATT * damageOffsetIndex + 1];
      damageScalerScalars.agility       = USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->damage[NUM_SCALING_DAMAGE_ATT * damageOffsetIndex + 2];
      damageScalerScalars.understanding = USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->damage[NUM_SCALING_DAMAGE_ATT * damageOffsetIndex + 3];
=======
   TArray<FText> preReqNames;
   for(TSubclassOf<UMySpell> preReqSpell : spellDefaults.PreReqs)
   {
      preReqNames.Add(preReqSpell.GetDefaultObject()->GetSpellDefaults().Name);
>>>>>>> componentrefactor
   }
   return preReqNames;
}

float UMySpell::GetSpellDuration(const UAbilitySystemComponent* abilityComponent) const
{
<<<<<<< HEAD
   FGameplayAbilitySpec* abilitySpec = abilityComponent->FindAbilitySpecFromClass(GetClass());
   checkf(abilitySpec, TEXT("Ability hasn't been registered to unit"));
   const int numUpgrades = USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->period.Num();
   if(numUpgrades > 0) return USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->period[GetIndex(abilitySpec->Level, numUpgrades, GetMaxLevel())];
   return 0;
=======
   return GetSpellDefaultValueChecked(abilityComponent, spellDefaults.Duration);
>>>>>>> componentrefactor
}

FDamageScalarStruct UMySpell::GetDamage(const UAbilitySystemComponent* abilityComponent) const
{
<<<<<<< HEAD
   FGameplayAbilitySpec* abilitySpec = abilityComponent->FindAbilitySpecFromClass(GetClass());
   checkf(abilitySpec, TEXT("Ability hasn't been registered to unit"));
   return USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->castTime;
=======
   return GetSpellDefaultValueChecked(abilityComponent, spellDefaults.Damage);
>>>>>>> componentrefactor
}

float UMySpell::GetPeriod(const UAbilitySystemComponent* abilityComponent) const
{
<<<<<<< HEAD
   FGameplayAbilitySpec* abilitySpec = abilityComponent->FindAbilitySpecFromClass(GetClass());
   checkf(abilitySpec, TEXT("Ability hasn't been registered to unit"));
   return USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->secondaryTime;
=======
   return GetSpellDefaultValueChecked(abilityComponent, spellDefaults.Period);
>>>>>>> componentrefactor
}

float UMySpell::GetCastTime(const UAbilitySystemComponent* abilityComponent) const
{
<<<<<<< HEAD
   FGameplayAbilitySpec* abilitySpec = abilityComponent->FindAbilitySpecFromClass(GetClass());
   checkf(abilitySpec, TEXT("Ability hasn't been registered to unit"));
   const int numUpgrades = USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->AOE.Num();
   if(numUpgrades > 0) return USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->AOE[GetIndex(abilitySpec->Level, numUpgrades, GetMaxLevel())];
   return 0;
=======
   return GetSpellDefaultValueChecked(abilityComponent, spellDefaults.CastTime);
}

float UMySpell::GetSecondaryTime(const UAbilitySystemComponent* abilityComponent) const
{
   return GetSpellDefaultValueChecked(abilityComponent, spellDefaults.SecondaryTime);
>>>>>>> componentrefactor
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
<<<<<<< HEAD
=======
   specHandle =
       UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(specHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Health"), damageScalings.hitpoints);
>>>>>>> componentrefactor
   return specHandle;
}

FGameplayEffectSpecHandle UMySpell::SetPeriod(FGameplayEffectSpecHandle specHandle, float period)
{
   FGameplayEffectSpec* Spec = specHandle.Data.Get();
<<<<<<< HEAD
   if(Spec) {
=======
   if(Spec)
   {
>>>>>>> componentrefactor
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
<<<<<<< HEAD
   FGameplayEffectSpecHandle effect      = MakeOutgoingGameplayEffectSpec(effectClass, GetLevel(abilityComp));
=======
   FGameplayEffectSpecHandle effect      = MakeOutgoingGameplayEffectSpec(effectClass, GetAbilityLevel());
>>>>>>> componentrefactor

   effect.Data->DynamicAssetTags.AddTag(GetElemTag());
   effect.Data->DynamicAssetTags.AddTag(name);
   effect.Data->DynamicAssetTags.AppendTags(assetTags);
<<<<<<< HEAD
   if(effect.Data->Def->DurationPolicy != EGameplayEffectDurationType::Instant) {
      effect.Data->Period = GetPeriod(abilityComp);
      // If we don't lock the duration, the duration will be recalcuated somewhere in active effect creation ...
      effect.Data->SetDuration(GetSpellDuration(abilityComp), true);
   }

   if(effectClass->IsChildOf(URTSDamageEffect::StaticClass())) SetScaling(effect);
=======
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
>>>>>>> componentrefactor
   return effect;
}

UEnvQuery* UMySpell::GetDefaultQueryForSpell(UWorld* worldRef)
{
   // ! Not making classes for these since most spell cast usage will have custom targeting anyways
<<<<<<< HEAD
   if(const auto dataAssetSS = worldRef->GetGameInstance()->GetSubsystem<UGlobalDataAssetsSS>()) {
      if(descriptionTags.HasTag(FGameplayTag::RequestGameplayTag("Skill.Category.Offensive"))) {
         return GetTargeting()->GetDefaultQueryForTargetingScheme(dataAssetSS->defaultOffensiveTargetSchemes);
      }
      if(descriptionTags.HasTag(FGameplayTag::RequestGameplayTag("Skill.Category.Support"))) {
         return GetTargeting()->GetDefaultQueryForTargetingScheme(dataAssetSS->defaultSupportTargetSchemes);
      }
      if(descriptionTags.HasTag(FGameplayTag::RequestGameplayTag("Skill.Category.Purge"))) {
         return GetTargeting()->GetDefaultQueryForTargetingScheme(dataAssetSS->defaultPurgeTargetSchemes);
      }
      if(descriptionTags.HasTag(FGameplayTag::RequestGameplayTag("Skill.Category.Disable"))) {
         return GetTargeting()->GetDefaultQueryForTargetingScheme(dataAssetSS->defaultDisableTargetSchemes);
=======
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
>>>>>>> componentrefactor
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

FGameplayAbilityTargetDataHandle UMySpell::GetTargetDataFromAvatarLoc() const
{
   return UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(GetAvatarActorFromActorInfo());
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

void UMySpell::AddSetByCallerTag(UAbilitySystemComponent* ASC, FActiveGameplayEffectHandle activeEffectHandle, FGameplayTag callerTag, float magnitude)
{
   FActiveGameplayEffect* activeGE = const_cast<FActiveGameplayEffect*>(ASC->GetActiveGameplayEffect(activeEffectHandle));
   activeGE->Spec.SetSetByCallerMagnitude(callerTag, magnitude);
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

FVector UMySpell::GetCenterOfTargetCapsuleLocation() const
{
   const AUnit* targetUnit     = GetAvatarTargetUnit();
   const float  unitHalfHeight = targetUnit->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
   return targetUnit->GetActorLocation() + FVector(0, 0, unitHalfHeight);
}

FGameplayAbilitySpec* UMySpell::GetAbilitySpec(const UAbilitySystemComponent* abilityComponent) const
{
   FGameplayAbilitySpec* abilitySpec = const_cast<UAbilitySystemComponent*>(abilityComponent)->FindAbilitySpecFromClass(GetClass());
   ensureMsgf(abilitySpec, TEXT("Ability hasn't been registered to unit"));
   return abilitySpec;
}

int UMySpell::GetIndex(int currentLevel, int numCategories, int maxLevel)
{
   if(currentLevel >= 0 && maxLevel >= 0 && numCategories >= 0)
   {
      const int index = FMath::CeilToInt(static_cast<float>(numCategories) * currentLevel / maxLevel) - 1;
      return index;
   }
   return -1;
}
