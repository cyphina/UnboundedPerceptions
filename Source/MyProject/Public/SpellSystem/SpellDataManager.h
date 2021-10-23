#pragma once
#include "GameplayTagContainer.h"
#include "SpellTargetingTypes.h"
#include "SpellDataManager.generated.h"

class UMySpell;

/**
 * Singleton class for a spell manager that lets us get our spell data table whenever we need information about a spell (aka our SpellInfo)
 * If one day we have so many spells it becomes a memory issue, we may find ways to load and unload spell data dynamically.
 */
UCLASS()
class MYPROJECT_API USpellDataManager : public UObject
{
   GENERATED_BODY()
 public:
   USpellDataManager();
   ~USpellDataManager() = default;

   FORCEINLINE static USpellDataManager& GetData()
   {
      if(SingletonManager == nullptr)
      {
         InitializeManager();
      }
      return *SingletonManager;
   }

   FORCEINLINE TSubclassOf<UMySpell> GetSpellClass(FGameplayTag spellID) const
   {
      checkf(spellClasses.Contains(spellID), TEXT("SpellID %s missing from list of spell classes"), *spellID.ToString());
      return spellClasses[spellID];
   }

   const FGameplayTagContainer&      GetSupportTags() const { return supportTags; }
   const FGameplayTagContainer&      GetOffensiveTags() const { return offensiveTags; }
   const TMap<FGameplayTag, int>&    GetPurgeTagMap() const { return purgeTagMap; }
   const TMap<FGameplayTag, FColor>& GetElementalColorMap() const { return elementalColorMap; }

 private:
   static USpellDataManager* SingletonManager;

   static TMap<FGameplayTag, TSubclassOf<UMySpell>> spellClasses;

   static FGameplayTagContainer      supportTags;       // List of tags support spells will have
   static FGameplayTagContainer      offensiveTags;     // List of tags elemental spells will have
   static TMap<FGameplayTag, int>    purgeTagMap;       // Mapping of purge tags to how many effects a purge can attempt to dispel
   static TMap<FGameplayTag, FColor> elementalColorMap; // Maps a gameplay tag representing an element to its corresponding color

   static void InitializeManager();
   static void SetupCachedSpellClassMap();
};
