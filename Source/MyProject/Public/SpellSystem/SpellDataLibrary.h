// Created 7/23/20 10:40 PM

#pragma once
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpellDataLibrary.generated.h"

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

   static const FGameplayTagContainer      supportTags;   // List of tags support spells will have
   static const FGameplayTagContainer      offensiveTags; // List of tags elemental spells will have
   static const TMap<FGameplayTag, int>    purgeTagMap;   // Mapping of purge tags to how many effects a purge can attempt to dispel
   static const TMap<FGameplayTag, FColor> elementalMap;  // Maps a gameplay tag representing an element to its corresponding color
};
