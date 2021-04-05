#include "SpellDataLibrary.h"
#include "GameplayTagAssetInterface.h"
#include "RTSAbilitySystemComponent.h"
#include "UpResourceManager.h"

const TMap<FGameplayTag, FColor> USpellDataLibrary::elementalMap = {{FGameplayTag::RequestGameplayTag("Combat.Element.None"), FColor::White},
                                                                    {FGameplayTag::RequestGameplayTag("Combat.Element.Arcane"), FColor(0, 0x66, 0xBB)},
                                                                    {FGameplayTag::RequestGameplayTag("Combat.Element.Blood"), FColor(255, 51, 51)},
                                                                    {FGameplayTag::RequestGameplayTag("Combat.Element.Chaos"), FColor::Purple},
                                                                    {FGameplayTag::RequestGameplayTag("Combat.Element.Cosmic"), FColor(213, 127, 209)},
                                                                    {FGameplayTag::RequestGameplayTag("Combat.Element.Dark"), FColor(0, 0, 0x33)},
                                                                    {FGameplayTag::RequestGameplayTag("Combat.Element.Earth"), FColor(210, 180, 140)},
                                                                    {FGameplayTag::RequestGameplayTag("Combat.Element.Electric"), FColor::Yellow},
                                                                    {FGameplayTag::RequestGameplayTag("Combat.Element.Ethereal"), FColor::Emerald},
                                                                    {FGameplayTag::RequestGameplayTag("Combat.Element.Fire"), FColor::Red},
                                                                    {FGameplayTag::RequestGameplayTag("Combat.Element.Force"), FColor(96, 96, 96)},
                                                                    {FGameplayTag::RequestGameplayTag("Combat.Element.Light"), FColor::White},
                                                                    {FGameplayTag::RequestGameplayTag("Combat.Element.Poison"), FColor(255, 102, 255)},
                                                                    {FGameplayTag::RequestGameplayTag("Combat.Element.Water"), FColor::Blue},
                                                                    {FGameplayTag::RequestGameplayTag("Combat.Element.Wind"), FColor(51, 255, 153)},
                                                                    {FGameplayTag::EmptyTag, FColor::White}};

const TMap<FGameplayTag, int> USpellDataLibrary::purgeTagMap = {
    {FGameplayTag::RequestGameplayTag("Combat.Effect.Purge.One"), 1},   {FGameplayTag::RequestGameplayTag("Combat.Effect.Purge.Two"), 2},
    {FGameplayTag::RequestGameplayTag("Combat.Effect.Purge.Three"), 3}, {FGameplayTag::RequestGameplayTag("Combat.Effect.Purge.Four"), 4},
    {FGameplayTag::RequestGameplayTag("Combat.Effect.Purge.Five"), 5},  {FGameplayTag::RequestGameplayTag("Combat.Effect.Purge.Six"), 6},
    {FGameplayTag::RequestGameplayTag("Combat.Effect.Purge.All"), 100}};

const FGameplayTagContainer USpellDataLibrary::supportTags   = FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Skill.Category.Support"));
const FGameplayTagContainer USpellDataLibrary::offensiveTags = FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Skill.Category.Offensive"));

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
   return assetTags.Filter(UpResourceManager::EffectNameTagFilter).First();
}

FGameplayTag USpellDataLibrary::GetEffectNameTagFromActiveHandle(UAbilitySystemComponent* ASC, FActiveGameplayEffectHandle EffectHandle)
{
   FGameplayTagContainer assetTags;
   ASC->GetActiveGameplayEffect(EffectHandle)->Spec.GetAllAssetTags(assetTags);
   return assetTags.Filter(UpResourceManager::EffectNameTagFilter).First();
}

void USpellDataLibrary::RemoveEffectWtihNameTag(UAbilitySystemComponent* ASC, FGameplayTag EffectName, int StacksToRemove)
{
   const auto effectsWithTag = ASC->GetActiveEffectsWithAllTags(EffectName.GetSingleTagContainer());
   if(effectsWithTag.Num() > 0)
   {
      ASC->RemoveActiveGameplayEffect(effectsWithTag[0], StacksToRemove);
   }
}
