#include "MyProject.h"
#include "SpellFunctionLibrary.h"
#include "GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"

USpellFunctionLibrary::USpellFunctionLibrary(const FObjectInitializer& o) : Super(o)
{
}

FGameplayEffectSpecHandle USpellFunctionLibrary::MakeGameplayEffect(UGameplayAbility* AbilityRef, TSubclassOf<UGameplayEffect> EffectClass, float Level, float Duration, float Period,
                                                                    FGameplayTag Elem, FGameplayTag Name, FGameplayTagContainer assetTags)
{
   FGameplayEffectSpecHandle effect = AbilityRef->MakeOutgoingGameplayEffectSpec(EffectClass, Level);
   effect.Data->DynamicAssetTags.AddTag(Elem); // Use add tag to check if tag is valid and prevents duplicate tags.
   effect.Data->DynamicAssetTags.AddTag(Name);
   effect.Data->DynamicAssetTags.AppendTags(assetTags);
   if (effect.Data->Def->DurationPolicy != EGameplayEffectDurationType::Instant) // Do this check since some instant effects rely on this procedure too
   {
      effect.Data->Period = Period;
      effect.Data->SetDuration(Duration, true); // if we don't lock the duration, the duration will be recalcuated somewhere in active effect creation ...
   }
   return effect;
}

FGameplayEffectSpecHandle USpellFunctionLibrary::MakeDamageEffect(UGameplayAbility* AbilityRef, TSubclassOf<UGameplayEffect> EffectClass, float Level, float Duration, float Period, FGameplayTag Elem,
                                                                  FGameplayTag Name, FGameplayTagContainer assetTags, FDamageScalarStruct damageValues)
{
   FGameplayEffectSpecHandle effect = MakeGameplayEffect(AbilityRef, EffectClass, Level, Duration, Period, Elem, Name, assetTags);
   effect.Data->DynamicAssetTags.AddTag(Elem); // Use add tag to check if tag is valid and prevents duplicate tags.
   effect.Data->DynamicAssetTags.AddTag(Name);

   // no period since damage is instant application
   effect.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Combat.Stats.Strength"), damageValues.strength);
   effect.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Combat.Stats.Intelligence"), damageValues.intelligence);
   effect.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Combat.Stats.Agility"), damageValues.agility);
   effect.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Combat.Stats.Understanding"), damageValues.agility);
   return effect;
}

FGameplayEffectSpecHandle USpellFunctionLibrary::MakeStatChangeEffect(UGameplayAbility* AbilityRef, TSubclassOf<UGameplayEffect> EffectClass, float Level, float Duration, float Period,
                                                                      FGameplayTag Elem, FGameplayTag Name, FGameplayTagContainer assetTags, TArray<FStatChange> StatChanges = TArray<FStatChange>())
{
   FGameplayEffectSpecHandle effect = MakeGameplayEffect(AbilityRef, EffectClass, Level, Duration, Period, Elem, Name, assetTags);
   for (FStatChange statChange : StatChanges) {
      effect.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(*(FString("Combat.Stats.") + statChange.changedAtt.GetName())), statChange.changeStatMagnitude);
      // GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, *(FString("Combat.Stats.") + statChange.changedAtt.GetName()));
   }
   // statChanges = MoveTemp(StatChanges);
   return effect;
}