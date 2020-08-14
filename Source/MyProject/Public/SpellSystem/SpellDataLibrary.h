// Created 7/23/20 10:40 PM

#pragma once
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpellDataLibrary.generated.h"

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
   static const FGameplayTagContainer      supportTags;   // List of tags support spells will have
   static const FGameplayTagContainer      offensiveTags; // List of tags elemental spells will have
   static const TMap<FGameplayTag, int>    purgeTagMap;   // Mapping of purge tags to how many effects a purge can attempt to dispel
   static const TMap<FGameplayTag, FColor> elementalMap;  // Maps a gameplay tag representing an element to its corresponding color

   /** Stunned prevents any action from being taken by the unit and should disrupt any queued actions as well*/
   UFUNCTION(BlueprintCallable)
   static bool IsStunned(const IGameplayTagAssetInterface&);

   /** Prevents spell casting */
   UFUNCTION(BlueprintCallable)
   static bool IsSilenced(const IGameplayTagAssetInterface&);

   /**Check to see if the unit has any statuses that cause the unit to be considered invisible and not revealed by true sight*/
   UFUNCTION(BlueprintCallable)
   static bool IsInvisible(const IGameplayTagAssetInterface&);

   /**Check to see if the unit can die*/
   UFUNCTION(BlueprintCallable)
   static bool IsImmortal(const IGameplayTagAssetInterface&);

   /**Check to see if the unit is a ward type (only takes one damage per hit)*/
   UFUNCTION(BlueprintCallable)
   static bool IsWard(const IGameplayTagAssetInterface&);

   /** Check to see if this unit has god mode (cannot be hurt whatsoever)*/
   UFUNCTION(BlueprintCallable)
   static bool IsGodMode(const IGameplayTagAssetInterface&);

   /** Check to see if this unit is still targetable by attacks*/
   UFUNCTION(BlueprintCallable)
   static bool IsAttackable(const IGameplayTagAssetInterface&);
};
