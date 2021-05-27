// Created 7/23/20 10:40 PM

#pragma once
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpellDataLibrary.generated.h"

class AUnit;
struct FGameplayEffectSpec;
class UAbilitySystemComponent;
class URTSAbilitySystemComponent;
class IGameplayTagAssetInterface;

/** Used to hold data and data accessors associated with the SpellSystem
 * -- Holds accessors to check if a unit has common game effects (IsStunned, IsPhased, etc.). Logic for this is put in this class over the component itself so we can
 * -- utilize the interface which lets the user know that this code boils down to tag checking inside a tag container. If we put it inside the component the user will have to figure out
 * -- that the component also acts as a tag container by exploring the code which is already monolithic.
 * -- Holds groups of tags used to categorize spells based on (used in targeting)
 */
UCLASS()
class MYPROJECT_API USpellDataLibrary : public UBlueprintFunctionLibrary
{
   GENERATED_BODY()

 public:
   UFUNCTION(BlueprintPure, meta = (BlueprintInternalUseOnly = "TRUE"))
   static bool BP_IsStunned(const URTSAbilitySystemComponent* abilityComponent);

   UFUNCTION(BlueprintPure, meta = (BlueprintInternalUseOnly = "TRUE"))
   static bool BP_IsSilenced(const URTSAbilitySystemComponent* abilityComponent);

   UFUNCTION(BlueprintPure, meta = (BlueprintInternalUseOnly = "TRUE"))
   static bool BP_IsInvisible(const URTSAbilitySystemComponent* abilityComponent);

   UFUNCTION(BlueprintPure, meta = (BlueprintInternalUseOnly = "TRUE"))
   static bool BP_IsImmortal(const URTSAbilitySystemComponent* abilityComponent);

   UFUNCTION(BlueprintPure, meta = (BlueprintInternalUseOnly = "TRUE"))
   static bool BP_IsWard(const URTSAbilitySystemComponent* abilityComponent);

   UFUNCTION(BlueprintPure, meta = (BlueprintInternalUseOnly = "TRUE"))
   static bool BP_IsGodMode(const URTSAbilitySystemComponent* abilityComponent);

   UFUNCTION(BlueprintPure, meta = (BlueprintInternalUseOnly = "TRUE"))
   static bool BP_IsAttackable(const URTSAbilitySystemComponent* abilityComponent);

   /** Stunned prevents any action from being taken by the unit and should disrupt any queued actions as well*/
   static bool IsStunned(const IGameplayTagAssetInterface* abilityComponent);

   /** Prevents spell casting */
   static bool IsSilenced(const IGameplayTagAssetInterface* abilityComponent);

   /**Check to see if the unit has any statuses that cause the unit to be considered invisible and not revealed by true sight*/
   static bool IsInvisible(const IGameplayTagAssetInterface* abilityComponent);

   /**Check to see if the unit can die*/
   static bool IsImmortal(const IGameplayTagAssetInterface* abilityComponent);

   /**Check to see if the unit is a ward type (only takes one damage per hit)*/
   static bool IsWard(const IGameplayTagAssetInterface* abilityComponent);

   /** Check to see if this unit has god mode (cannot be hurt whatsoever)*/
   static bool IsGodMode(const IGameplayTagAssetInterface* abilityComponent);

   /** Check to see if this unit is still targetable by attacks? Will only stop new attacks from being casted on this unit,
     * not ones already initiated before the unit becomes un-targetable. */
   static bool IsAttackable(const IGameplayTagAssetInterface* abilityComponent);

   UFUNCTION(BlueprintCallable, BlueprintPure)
   static float GetPercentageOfBaseStat(AUnit* UnitToGetStatFrom, const FGameplayAttribute& GameplayAttribute, float Percent);

   UFUNCTION(BlueprintCallable, BlueprintPure)
   static float GetPercentageOfStat(AUnit* UnitToGetStatFrom, const FGameplayAttribute& GameplayAttribute, float Percent);

   UFUNCTION(BlueprintCallable, BlueprintPure)
   static float GetSetByCallerTagMagnitude(UAbilitySystemComponent* AbilityComponent, UPARAM(meta = (Categories = "Combat.EffectName")) FGameplayTag EffectName,
                                           FGameplayTag SetByCallerTag);

   UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (DisplayName = "GetEffectNameTag"))
   static FGameplayTag GetEffectNameTagFromSpec(UAbilitySystemComponent* ASC, const FGameplayEffectSpecHandle& EffectSpecHandle);

   UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (DisplayName = "GetEffectNameTag"))
   static FGameplayTag GetEffectNameTagFromActiveHandle(UAbilitySystemComponent* ASC, FActiveGameplayEffectHandle EffectHandle);

   /**
    * @brief Removes one instance of a gameplay effect given a name tag to identify the effect.
    * @param ASC - Reference to ASC we want to remove effect from.
    * @param EffectName - Name tag to identify the effect.
    * @param StacksToRemove - If -1, removes all stacks.
    */
   UFUNCTION(BlueprintCallable, Meta = (DisplayName = "GetEffectNameTag"))
   static void RemoveEffectWtihNameTag(UAbilitySystemComponent* ASC, FGameplayTag EffectName, int StacksToRemove = -1);

   UFUNCTION(BlueprintCallable, BlueprintPure)
   static FGameplayTagContainer GetEffectNameTag();

   UFUNCTION(BlueprintCallable, BlueprintPure)
   static FGameplayTagContainer GetEffectElemTag();

   UFUNCTION(BlueprintCallable, BlueprintPure)
   static FGameplayTagContainer GetEffectRemoveableTag();

   UFUNCTION(BlueprintCallable, BlueprintPure)
   static FGameplayTagContainer GetEffectPseudoStackTag();

   UFUNCTION(BlueprintCallable, BlueprintPure)
   static FGameplayTag GetConfirmSpellTag();

   UFUNCTION(BlueprintCallable, BlueprintPure)
   static FGameplayTag GetConfirmSpellTargetTag();

   UFUNCTION(BlueprintCallable, BlueprintPure)
   static const FGameplayTagContainer& GetSupportTags();

   UFUNCTION(BlueprintCallable, BlueprintPure)
   static const FGameplayTagContainer& GetOffensiveTags();

   UFUNCTION(BlueprintCallable, BlueprintPure)
   static const TMap<FGameplayTag, int>& GetPurgeTagMap();

   UFUNCTION(BlueprintCallable, BlueprintPure)
   static const TMap<FGameplayTag, FColor>& GetElementalColorMap();

 private:
   const FGameplayTagContainer EffectNameTagFilter       = FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.EffectName"));
   const FGameplayTagContainer EffectElemTagFilter       = FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Element"));
   const FGameplayTagContainer EffectRemoveableTagFilter = FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Effect.Removable"));
   // Use this when we have multiple effects that have their own timers, but should be treated as a single effect (called pseudo-stackable effects)
   const FGameplayTagContainer EffectPseudoStackTagFilter = FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Effect.ShowEffectsAsStack"));
};
