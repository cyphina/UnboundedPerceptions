#include "MyProject.h"
#include "MySpell.h"
#include "SpellDataManager.h"
#include "AbilitySystemBlueprintLibrary.h"
<<<<<<< HEAD
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "SpellTargetingTypes.h"
#include "GameplayEffects/RTSDamageEffect.h"
#include "SpellDataTableRow.h"

USpellDataManager* USpellDataManager::SingletonManager = nullptr;

const FUpSpellTargeting_None USpellDataManager::spellNoTarget = FUpSpellTargeting_None(FGameplayTag::RequestGameplayTag("Spell.Targeting.None"));
const FUpSpellTargeting_SingleUnit USpellDataManager::spellSingleTarget = FUpSpellTargeting_SingleUnit(FGameplayTag::RequestGameplayTag("Spell.Targeting.Single"));
const FUpSpellTargeting_InteractableOrUnit USpellDataManager::spellInteractableOrActorTarget = FUpSpellTargeting_InteractableOrUnit(
FGameplayTag::RequestGameplayTag("Spell.Targeting.InteractableOrActor"));
const FUpSpellTargeting_Area   USpellDataManager::spellAreaTarget   = FUpSpellTargeting_Area(FGameplayTag::RequestGameplayTag("Spell.Targeting.Area"));
const FUpSpellTargeting_Vector USpellDataManager::spellVectorTarget = FUpSpellTargeting_Vector(FGameplayTag::RequestGameplayTag("Spell.Targeting.Vector"));

const TMap<FGameplayTag, const FUpSpellTargeting*> USpellDataManager::spellTargetings = {{spellNoTarget.GetTargetTag(), &spellNoTarget},
                                                                                         {spellSingleTarget.GetTargetTag(), &spellSingleTarget},
                                                                                         {spellInteractableOrActorTarget.GetTargetTag(), &spellInteractableOrActorTarget},
                                                                                         {spellAreaTarget.GetTargetTag(), &spellAreaTarget},
                                                                                         {spellVectorTarget.GetTargetTag(), &spellVectorTarget}};

USpellDataManager::USpellDataManager()
{
   static ConstructorHelpers::FObjectFinder<UDataTable> SpellLookupTableFinder(TEXT("/Game/RTS_Tutorial/Tables/SpellList"));
   // Caching the data table information can be problematic if we reimport
   if(SpellLookupTableFinder.Object)
      SetupSpells(SpellLookupTableFinder.Object);
}

void USpellDataManager::SetupSpells(UDataTable* spellLookupTable)
{
   static const FString ContextString(TEXT("GENERAL"));
   if(spellLookupTable)
   {
      TArray<FSpellbookLookupRow*> table;
      TArray<FName>                rowNames = spellLookupTable->GetRowNames();
      FSpellInfo                   spellInfo;

      spellLookupTable->GetAllRows<FSpellbookLookupRow>(ContextString, table);
      // UE_LOG(LogTemp, Warning, TEXT("%d"), rowNames.Num());
      if(rowNames.Num() > 0 && table.Num() > 0)
      {
         for(int i = 0; i < rowNames.Num(); ++i)
         {
            int rowName             = FCString::Atoi(*rowNames[i].ToString());
            spellInfo.name          = table[i]->Name;
            spellInfo.elem          = table[i]->Elem;
            spellInfo.desc          = table[i]->Desc;
            spellInfo.castTime      = table[i]->CastTime;
            spellInfo.secondaryTime = table[i]->SecondaryTime;
            spellInfo.cdDuration    = table[i]->Cooldown;
            spellInfo.maxLevel      = table[i]->MaxLevel;
            spellInfo.range         = table[i]->Range;
            spellInfo.reqLevel      = table[i]->LevelReq;
            spellInfo.cost          = table[i]->Cost;
            spellInfo.targeting     = &MakeSpellTargeting(table[i]->Targeting);
            spellInfo.duration      = table[i]->Duration;
            spellInfo.damage        = table[i]->Damage;
            spellInfo.period        = table[i]->Period;
            spellInfo.AOE           = table[i]->AOE;
            spellInfo.preReqs       = table[i]->PreReqs;
            spells.Add(rowName, spellInfo);
         }
      }
   }

   SetupCachedSpellClassMap();
}

void USpellDataManager::SetupCachedSpellClassMap()
{
   for(TObjectIterator<UClass> it; it; ++it)
   {
      if(it->IsChildOf(UMySpell::StaticClass()))
      {
         FStructProperty* spellDefaultProperty = FindFieldChecked<FStructProperty>(*it, GET_MEMBER_NAME_CHECKED(UMySpell, spellDefaults));
         int              spellId              = *spellDefaultProperty->ContainerPtrToValuePtr<int>((*it)->GetDefaultObject());
         spellClasses.Add(spellId, *it);
      }
   }
}

const FUpSpellTargeting& USpellDataManager::MakeSpellTargeting(FGameplayTag targetTag)
{
   return *spellTargetings[targetTag];
=======
#include "AssetRegistryModule.h"
#include "GameplayEffects/RTSDamageEffect.h"

USpellDataManager*                        USpellDataManager::SingletonManager = nullptr;
TMap<FGameplayTag, TSubclassOf<UMySpell>> USpellDataManager::spellClasses;

USpellDataManager::USpellDataManager()
{
}

void USpellDataManager::SetupCachedSpellClassMap()
{
   FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
   IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

   const FName SpellPath = TEXT("/Game/RTS_Tutorial/Blueprints/SpellSystem/Spells");

   TArray<FString> ScanPaths;
   ScanPaths.Add(SpellPath.ToString());
   AssetRegistry.ScanPathsSynchronous(ScanPaths);

   TArray<FAssetData> SpellAssetList;
   FARFilter Filter;
   Filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
   Filter.PackagePaths.Add(SpellPath);
   Filter.bRecursiveClasses = true;
   Filter.bRecursivePaths = true;
   AssetRegistry.GetAssets(Filter, SpellAssetList);

   TArray<TSubclassOf<UMySpell>> SpellClasses;
   for(const FAssetData& Asset : SpellAssetList)
   {
      const UBlueprint* BlueprintObj = Cast<UBlueprint>(Asset.GetAsset());
      if(!BlueprintObj)
      {
         continue;
      }

      UClass* BlueprintClass = BlueprintObj->GeneratedClass;
      if(!BlueprintClass || !BlueprintClass->IsChildOf(UMySpell::StaticClass()))
      {
         continue;
      }

      spellClasses.Add(BlueprintClass->GetDefaultObject<UMySpell>()->GetSpellDefaults().nameTag, BlueprintClass);
   }
>>>>>>> componentrefactor
}

void USpellDataManager::InitializeManager()
{
   check(!SingletonManager);
   SingletonManager = NewObject<USpellDataManager>(GetTransientPackage(), NAME_None);
<<<<<<< HEAD
=======
   SetupCachedSpellClassMap();
>>>>>>> componentrefactor
   SingletonManager->AddToRoot();
}
