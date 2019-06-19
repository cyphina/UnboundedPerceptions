#pragma once

#include "GameplayTagContainer.h"
#include "StatChangeEffect.h"
#include "DamageStructs.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpellFunctionLibrary.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class ARTSProjectile;
class AUnit;

/**A class that holds a blueprint library, that is it can be used anywhere inside blueprints so we can get our spellInfo via the manager in our blueprints
 * Recently I've exposed all the information inside UMySpell, so if you can use one of the accesors to get a default instance of the spell, you can get all the data
 * from that.
 */
UCLASS(MinimalAPI)
class USpellFunctionLibrary : public UBlueprintFunctionLibrary
{
   GENERATED_UCLASS_BODY()

 public:
   /**Function with custom BPNode which wraps around make gameplay effect to provide it with more functionality*/
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Gameplay Effect", BlueprintInternalUseOnly = "true"), Category = "Spell Creation Helper")
   static struct FGameplayEffectSpecHandle MakeGameplayEffect(UGameplayAbility* AbilityRef, TSubclassOf<UGameplayEffect> EffectClass, float Level, float Duration, float Period, FGameplayTag Elem,
                                                              FGameplayTag Name, FGameplayTagContainer assetTags);

   /**Creates gameplay damage effect*/
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Damage Effect", BlueprintInternalUseOnly = "true"), Category = "EffectFactory")
   static struct FGameplayEffectSpecHandle MakeDamageEffect(UGameplayAbility* AbilityRef, TSubclassOf<UGameplayEffect> EffectClass, float Level, float Duration, float Period, FGameplayTag Elem,
                                                            FGameplayTag Name, FGameplayTagContainer assetTags, FDamageScalarStruct damageValues);

   /**WARNING NOT CURRENTLY WORKING Creates gameplay stat change effect*/
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Stat Change Effect", BlueprintInternalUseOnly = "true"), Category = "EffectFactory")
   static struct FGameplayEffectSpecHandle MakeStatChangeEffect(UGameplayAbility* AbilityRef, TSubclassOf<UGameplayEffect> EffectClass, float Level, float Duration, float Period, FGameplayTag Elem,
                                                                FGameplayTag Name, FGameplayTagContainer assetTags, TArray<FStatChange> StatChanges);

   /**Sets up targetting for spells with bullets that can be casted by ally or enemy units but requires different targetting for either*/
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "Setup Bullet Targetting"), Category = "EffectFactory")
   static ARTSProjectile* SetupBulletTargetting(TSubclassOf<ARTSProjectile> bulletClass, AUnit* unitRef, UPARAM(ref) FGameplayEffectSpecHandle& specHandle, bool canGoThroughWalls);
};