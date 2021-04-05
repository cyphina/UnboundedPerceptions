#pragma once
<<<<<<< HEAD

#include "AOETargeting.h"
#include "GameplayTagContainer.h"
#include "NoTargeting.h"
#include "SingleUnitTargeting.h"
#include "SpellDataStruct.h"
#include "SpellTargetingTypes.h"
#include "VectorTargeting.h"
#include "SpellDataManager.generated.h"

class UMySpell;
class UMyGameplayEffect;
=======
#include "GameplayTagContainer.h"
#include "SpellTargetingTypes.h"
#include "SpellDataManager.generated.h"

class UMySpell;
>>>>>>> componentrefactor

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

<<<<<<< HEAD
   FORCEINLINE const FSpellInfo* GetSpellInfo(int spellID) const
   {
      checkf(spells.Contains(spellID), TEXT("SpellID %d missing from list of spell classes"), spellID);
      return &spells[spellID];
   }

   FORCEINLINE TSubclassOf<UMySpell> GetSpellClass(int spellID) const
   {
      checkf(spellClasses.Contains(spellID), TEXT("SpellID %d missing from list of spell classes"), spellID);
=======
   FORCEINLINE TSubclassOf<UMySpell> GetSpellClass(FGameplayTag spellID) const
   {
      checkf(spellClasses.Contains(spellID), TEXT("SpellID %s missing from list of spell classes"), *spellID.ToString());
>>>>>>> componentrefactor
      return spellClasses[spellID];
   }

private:
   static USpellDataManager* SingletonManager;

<<<<<<< HEAD
   TMap<int, FSpellInfo>                                     spells;
   TMap<int, TSubclassOf<UMySpell>>                          spellClasses;
   static const TMap<FGameplayTag, const FUpSpellTargeting*> spellTargetings;

   static void              InitializeManager();
   void                     SetupSpells(UDataTable* spellLookupTable);
   void                     SetupCachedSpellClassMap();
   static const FUpSpellTargeting& MakeSpellTargeting(FGameplayTag targetTag);

   static const FUpSpellTargeting_None       spellNoTarget;
   static const FUpSpellTargeting_SingleUnit spellSingleTarget;
   static const FUpSpellTargeting_Area       spellAreaTarget;
   static const FUpSpellTargeting_InteractableOrUnit      spellInteractableOrActorTarget;
   static const FUpSpellTargeting_Vector     spellVectorTarget;
=======
   static TMap<FGameplayTag, TSubclassOf<UMySpell>> spellClasses;

   static void InitializeManager();
   static void SetupCachedSpellClassMap();
>>>>>>> componentrefactor
};
