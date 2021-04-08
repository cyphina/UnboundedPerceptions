#pragma once

#include "GameplayTagContainer.h"
#include "StatChangeEffect.h"
#include "DamageStructs.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpellFunctionLibrary.generated.h"

class URTSProjectileStrategy;
class UGameplayAbility;
class UGameplayEffect;
class ARTSProjectile;
class UMySpell;
class AUnit;
class UCompositeCurveTable;

/**
 * This function library is used throughout custom ability creation (done purely in blueprints)
 * Some important functionality includes:
 * -- Getting list of tags that are important in categorizing
 * -- Helper functions to create gameplay effects in a single node
 * -- Helper functions to create gameplay effects from reading data off our table instead of having to assign the values manually
 * -- Helper functions to create simple bullet spells
 * -- Helper functions to create spells that require an additional confirm to setup
 */
UCLASS()
class MYPROJECT_API USpellFunctionLibrary : public UBlueprintFunctionLibrary
{
   GENERATED_UCLASS_BODY()

 public:
   /** Function with custom BPNode which wraps around make gameplay effect to provide it with more functionality*/
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Gameplay Effect", BlueprintInternalUseOnly = "true", DefaultToSelf = "AbilityRef"),
             Category = "Spell Creation Helper")
   static struct FGameplayEffectSpecHandle MakeGameplayEffect(UGameplayAbility* AbilityRef, TSubclassOf<UGameplayEffect> EffectClass, int Level, float Duration,
                                                              float Period, UPARAM(meta = (Categories = "Combat.Element")) FGameplayTag Elem,
                                                              UPARAM(meta = (Categories = "Skill.Name")) FGameplayTag EffectName, FGameplayTagContainer AssetTags);

   /** Creates a damage or healing effect. Both effects scale off damage vals, so they use a common function be manipulated. */
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Damage Effect", BlueprintInternalUseOnly = "true", DefaultToSelf = "AbilityRef"),
             Category = "EffectFactory")
   static struct FGameplayEffectSpecHandle MakeDamageOrHealingEffect(UGameplayAbility* AbilityRef, TSubclassOf<UGameplayEffect> EffectClass, int Level, float Duration,
                                                                     float Period, FGameplayTag Elem, FGameplayTag EffectName, FGameplayTagContainer assetTags,
                                                                     FDamageScalarStruct damageVals);

   /**
    * Stat change effects all have their individual gameplay effect classes with name and element already setup (ideally).
    * This is used to to pass in parameters like duration, period, or the stat change values granted
    * if those values were not already properly set on the gameplay effect class.
    * Any stat changes passed will only affect those that are setup in the modifiers of the gameplay effect. If we wanted a generic
    * stat change effect then we'd have to give it every modifier but then it would take a while in the logic loop somewhere downstream
    * since some logic in the GA system loops through all the modifiers.
    */
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Stat Change Effect", BlueprintInternalUseOnly = "true", DefaultToSelf = "AbilityRef"),
             Category = "EffectFactory")
   static struct FGameplayEffectSpecHandle MakeStatChangeEffect(UGameplayAbility* AbilityRef, TSubclassOf<UGameplayEffect> EffectClass, int Level, float Duration,
                                                                float Period, FGameplayTag Elem, FGameplayTag EffectName, FGameplayTagContainer assetTags,
                                                                TArray<FStatChange> StatChanges);

   /** Exposes factory bullet function to BPs */
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "Setup Bullet Targetting"), Category = "EffectFactory")
   static ARTSProjectile* SetupBulletTargetting(AUnit* casterRef, TSubclassOf<ARTSProjectile> bulletClass, TSubclassOf<URTSProjectileStrategy> projectileStrategyClass,
                                                TArray<FGameplayEffectSpecHandle> specHandles, bool canGoThroughWalls);

   /** Parses spell descriptions to place keywords with the actual statistic*/
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "Parse Descrption"), Category = "Spell Description Helper")
   static FText ParseDesc(const FText& inputText, const UAbilitySystemComponent* compRef, const UMySpell* spell);

   /** If a spell requires another press for confirmation like telekinesis and ice pillar, we can use this function to swap out a unit's spell temporarily */
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "Spell Swap"), Category = "Spell Functionality Extender")
   static void SpellSwap(TSubclassOf<UMySpell> originalSpell, TSubclassOf<UMySpell> newSpell, AUnit* ownerRef);

   /** See SpellSwap() for more information. This swaps out a spell specifically for the "confirmation" spell. */
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "Spell Confirm Swap"), Category = "Spell Functionality Extender")
   static void SpellConfirmSwap(TSubclassOf<UMySpell> confirmSpell, TSubclassOf<UMySpell> originalSpell, AUnit* ownerRef, bool bSwapInConfirm);

 private:
   static UCompositeCurveTable* effectPowerTableRef;
};
