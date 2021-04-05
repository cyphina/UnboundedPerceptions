#include "MyProject.h"
#include "MySpell.h"
#include "SpellDataManager.h"
#include "AbilitySystemBlueprintLibrary.h"
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
}

void USpellDataManager::InitializeManager()
{
   check(!SingletonManager);
   SingletonManager = NewObject<USpellDataManager>(GetTransientPackage(), NAME_None);
   SetupCachedSpellClassMap();
   SingletonManager->AddToRoot();
}
