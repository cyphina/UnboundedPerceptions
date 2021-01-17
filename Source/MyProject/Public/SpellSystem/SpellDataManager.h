#pragma once
#include "GameplayTagContainer.h"
#include "SpellTargetingTypes.h"
#include "SpellDataManager.generated.h"

class UMySpell;
class UMyGameplayEffect;

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
      if(SingletonManager == nullptr) { InitializeManager(); }
      return *SingletonManager;
   }

   FORCEINLINE TSubclassOf<UMySpell> GetSpellClass(FGameplayTag spellID) const
   {
      checkf(spellClasses.Contains(spellID), TEXT("SpellID %s missing from list of spell classes"), *spellID.ToString());
      return spellClasses[spellID];
   }

private:
   static USpellDataManager* SingletonManager;

   TMap<FGameplayTag, TSubclassOf<UMySpell>>                 spellClasses;

   static void                     InitializeManager();
   void                            SetupSpells(UDataTable* spellLookupTable);
   void                            SetupCachedSpellClassMap();
};
