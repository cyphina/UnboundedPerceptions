#include "SpellDataLibrary.h"
#include "GameplayTagAssetInterface.h"
#include "RTSAbilitySystemComponent.h"
#include "SpellDataManager.h"
#include "UpResourceManager.h"

bool USpellDataLibrary::BP_IsStunned(const URTSAbilitySystemComponent* abilityComponent)
{
   return IsStunned(abilityComponent);
}

bool USpellDataLibrary::BP_IsSilenced(const URTSAbilitySystemComponent* abilityComponent)
{
   return IsSilenced(abilityComponent);
}

bool USpellDataLibrary::BP_IsInvisible(const URTSAbilitySystemComponent* abilityComponent)
{
   return IsInvisible(abilityComponent);
}

bool USpellDataLibrary::BP_IsImmortal(const URTSAbilitySystemComponent* abilityComponent)
{
   return IsImmortal(abilityComponent);
}

bool USpellDataLibrary::BP_IsWard(const URTSAbilitySystemComponent* abilityComponent)
{
   return IsWard(abilityComponent);
}

bool USpellDataLibrary::BP_IsGodMode(const URTSAbilitySystemComponent* abilityComponent)
{
   return IsGodMode(abilityComponent);
}

bool USpellDataLibrary::BP_IsAttackable(const URTSAbilitySystemComponent* abilityComponent)
{
   return IsStunned(abilityComponent);
}

bool USpellDataLibrary::IsStunned(const IGameplayTagAssetInterface* abilityComponent)
{
   return abilityComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Debuff.Stunned")) ? true : false;
}

bool USpellDataLibrary::IsSilenced(const IGameplayTagAssetInterface* abilityComponent)
{
   return abilityComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Debuff.Silenced")) ? true : false;
}

bool USpellDataLibrary::IsInvisible(const IGameplayTagAssetInterface* abilityComponent)
{
   return abilityComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Buff.Invisibility")) &&
          !abilityComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Marked"));
}

bool USpellDataLibrary::IsImmortal(const IGameplayTagAssetInterface* abilityComponent)
{
   return abilityComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Buff.Immortality"));
}

bool USpellDataLibrary::IsWard(const IGameplayTagAssetInterface* abilityComponent)
{
   return abilityComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Ward"));
}

bool USpellDataLibrary::IsGodMode(const IGameplayTagAssetInterface* abilityComponent)
{
   return abilityComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Buff.GodMode"));
}

bool USpellDataLibrary::IsAttackable(const IGameplayTagAssetInterface* abilityComponent)
{
   return !(abilityComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Buff.Phased")) ||
            abilityComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Buff.Ghost")));
}

float USpellDataLibrary::GetSetByCallerTagMagnitude(UAbilitySystemComponent* AbilityComponent, FGameplayTag EffectName, FGameplayTag SetByCallerTag)
{
   if(AbilityComponent)
   {
      TArray<FActiveGameplayEffectHandle> effects = AbilityComponent->GetActiveEffectsWithAllTags(EffectName.GetSingleTagContainer());
      if(effects.Num() > 0)
      {
         const FActiveGameplayEffectHandle effect = effects[0];
         if(effect.IsValid())
         {
            return AbilityComponent->GetActiveGameplayEffect(effect)->Spec.GetSetByCallerMagnitude(SetByCallerTag);
         }
      }
   }
   return 0;
}

FGameplayTag USpellDataLibrary::GetEffectNameTagFromSpec(UAbilitySystemComponent* ASC, const FGameplayEffectSpecHandle& EffectSpecHandle)
{
   FGameplayTagContainer assetTags;
   EffectSpecHandle.Data->GetAllAssetTags(assetTags);
   return assetTags.Filter(GetEffectNameTag()).First();
}

FGameplayTag USpellDataLibrary::GetEffectNameTagFromActiveHandle(UAbilitySystemComponent* ASC, FActiveGameplayEffectHandle EffectHandle)
{
   FGameplayTagContainer assetTags;
   ASC->GetActiveGameplayEffect(EffectHandle)->Spec.GetAllAssetTags(assetTags);
   return assetTags.Filter(GetEffectNameTag()).First();
}

void USpellDataLibrary::RemoveEffectWtihNameTag(UAbilitySystemComponent* ASC, FGameplayTag EffectName, int StacksToRemove)
{
   const auto effectsWithTag = ASC->GetActiveEffectsWithAllTags(EffectName.GetSingleTagContainer());
   if(effectsWithTag.Num() > 0)
   {
      ASC->RemoveActiveGameplayEffect(effectsWithTag[0], StacksToRemove);
   }
}

FGameplayTagContainer USpellDataLibrary::GetEffectNameTag()
{
   return FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.EffectName"));
}

FGameplayTagContainer USpellDataLibrary::GetEffectElemTag()
{
   return FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Element"));
}

FGameplayTagContainer USpellDataLibrary::GetEffectRemoveableTag()
{
   return FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Effect.Removable"));
}

FGameplayTagContainer USpellDataLibrary::GetEffectPseudoStackTag()
{
   return FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Effect.ShowEffectsAsStack"));
}

FGameplayTag USpellDataLibrary::GetConfirmSpellTag()
{
   return FGameplayTag::RequestGameplayTag("Skill.Name.Confirm Spell");
}

FGameplayTag USpellDataLibrary::GetConfirmSpellTargetTag()
{
   return FGameplayTag::RequestGameplayTag("Skill.Name.Confirm Target");
}

const FGameplayTagContainer& USpellDataLibrary::GetSupportTags()
{
   return USpellDataManager::GetData().GetSupportTags();
}

const FGameplayTagContainer& USpellDataLibrary::GetOffensiveTags()
{
   return USpellDataManager::GetData().GetOffensiveTags();
}

const TMap<FGameplayTag, int>& USpellDataLibrary::GetPurgeTagMap()
{
   return USpellDataManager::GetData().GetPurgeTagMap();
}

const TMap<FGameplayTag, FColor>& USpellDataLibrary::GetElementalColorMap()
{
   return USpellDataManager::GetData().GetElementalColorMap();
}
