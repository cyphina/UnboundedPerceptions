#pragma once

#include "GameplayTagContainer.h"
#include "StatChangeEffect.h"
#include "DamageStructs.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpellFunctionLibrary.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class ARTSProjectile;
class UMySpell;
class AUnit;

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
class USpellFunctionLibrary : public UBlueprintFunctionLibrary
{
   GENERATED_UCLASS_BODY()

   static const int CONFIRM_SPELL_ID = 1003;
   static const int CONFIRM_SPELL_TARGET_ID = 1004;

 public:

   /**Function with custom BPNode which wraps around make gameplay effect to provide it with more functionality*/
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Gameplay Effect", BlueprintInternalUseOnly = "true"), Category = "Spell Creation Helper")
   static struct FGameplayEffectSpecHandle MakeGameplayEffect(UGameplayAbility* AbilityRef, TSubclassOf<UGameplayEffect> EffectClass, float Level, float Duration,
                                                              float Period, FGameplayTag Elem, FGameplayTag Name, FGameplayTagContainer assetTags);

   /**Creates gameplay damage effect*/
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Damage Effect", BlueprintInternalUseOnly = "true"), Category = "EffectFactory")
   static struct FGameplayEffectSpecHandle MakeDamageEffect(UGameplayAbility* AbilityRef, TSubclassOf<UGameplayEffect> EffectClass, float Level, float Duration,
                                                            float Period, FGameplayTag Elem, FGameplayTag Name, FGameplayTagContainer assetTags,
                                                            FDamageScalarStruct damageValues);

   /** ! WARNING NOT CURRENTLY WORKING
    *    Creates gameplay stat change effect*/
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Stat Change Effect", BlueprintInternalUseOnly = "true"), Category = "EffectFactory")
   static struct FGameplayEffectSpecHandle MakeStatChangeEffect(UGameplayAbility* AbilityRef, TSubclassOf<UGameplayEffect> EffectClass, float Level, float Duration,
                                                                float Period, FGameplayTag Elem, FGameplayTag Name, FGameplayTagContainer assetTags,
                                                                TArray<FStatChange> StatChanges);

   /**Sets up targetting for spells with bullets that can be casted by ally or enemy units but requires different targetting for either*/
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "Setup Bullet Targetting"), Category = "EffectFactory")
   static ARTSProjectile* SetupBulletTargetting(TSubclassOf<ARTSProjectile> bulletClass, AUnit* unitRef, UPARAM(ref) FGameplayEffectSpecHandle& specHandle,
                                                bool canGoThroughWalls);

   /**Parses spell descriptions to place keywords with the actual statistic*/
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "Parse Descrption"), Category = "Spell Description Helper")
   static FText ParseDesc(FText inputText, UAbilitySystemComponent* compRef, UMySpell* spell, TMap<FString, FString> args);

   /**If a spell requires another press for confirmation like telekinesis and ice pillar, we can use this function to swap out a unit's spell temporarily*/
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "Spell Swap"), Category = "Spell Functionality Extender")
   static void SpellSwap(TSubclassOf<UMySpell> originalSpell, TSubclassOf<UMySpell> newSpell, AUnit* ownerRef);

   /**If a spell requires another press for confirmation like telekinesis and ice pillar, we can use this function to swap out a unit's spell temporarily*/
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "Spell Confirm Swap"), Category = "Spell Functionality Extender")
   static void SpellConfirmSwap(TSubclassOf<UMySpell> confirmSpell, TSubclassOf<UMySpell> originalSpell, AUnit* ownerRef, bool bSwapInConfirm);

};

