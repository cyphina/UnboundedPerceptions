#include "MyProject.h"
#include "MySpell.h"
#include "SpellDataManager.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AssetRegistryModule.h"
#include "GameplayEffects/RTSDamageEffect.h"

USpellDataManager*                        USpellDataManager::SingletonManager = nullptr;
TMap<FGameplayTag, TSubclassOf<UMySpell>> USpellDataManager::spellClasses;

FGameplayTagContainer      USpellDataManager::supportTags;
FGameplayTagContainer      USpellDataManager::offensiveTags;
TMap<FGameplayTag, int>    USpellDataManager::purgeTagMap;
TMap<FGameplayTag, FColor> USpellDataManager::elementalColorMap;

USpellDataManager::USpellDataManager()
{
}

void USpellDataManager::SetupCachedSpellClassMap()
{
   FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
   IAssetRegistry&       AssetRegistry       = AssetRegistryModule.Get();

   const FName SpellPath = TEXT("/Game/RTS_Tutorial/Blueprints/SpellSystem/Spells");

   TArray<FString> ScanPaths;
   ScanPaths.Add(SpellPath.ToString());
   AssetRegistry.ScanPathsSynchronous(ScanPaths);

   TArray<FAssetData> SpellAssetList;
   FARFilter          Filter;
   Filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
   Filter.PackagePaths.Add(SpellPath);
   Filter.bRecursiveClasses = true;
   Filter.bRecursivePaths   = true;
   AssetRegistry.GetAssets(Filter, SpellAssetList);

   TArray<TSubclassOf<UMySpell>> SpellClasses;
   for(const FAssetData& Asset : SpellAssetList)
   {
      const UBlueprint* BlueprintObj = Cast<UBlueprint>(Asset.GetAsset());
      if(BlueprintObj)
      {
         UClass* BlueprintClass = BlueprintObj->GeneratedClass;
         if(!BlueprintClass || !BlueprintClass->IsChildOf(UMySpell::StaticClass()))
         {
            continue;
         }

         spellClasses.Add(BlueprintClass->GetDefaultObject<UMySpell>()->GetSpellDefaults().nameTag, BlueprintClass);
      }
      else
      {
         auto    GeneratedClassName = Asset.AssetName.ToString() + "_C";
         UClass* Class              = FindObject<UClass>(Asset.GetPackage(), *GeneratedClassName);

         if(Class && Class->IsChildOf(UMySpell::StaticClass()))
         {
            spellClasses.Add(Class->GetDefaultObject<UMySpell>()->GetSpellDefaults().nameTag, Class);
         }
      }
   }
}

void USpellDataManager::InitializeManager()
{
   check(!SingletonManager);
   SingletonManager = NewObject<USpellDataManager>(GetTransientPackage(), NAME_None);
   SetupCachedSpellClassMap();

   elementalColorMap = {{FGameplayTag::RequestGameplayTag("Combat.Element.None"), FColor::White},
                        {FGameplayTag::RequestGameplayTag("Combat.Element.Arcane"), FColor(0, 0x66, 0xBB)},
                        {FGameplayTag::RequestGameplayTag("Combat.Element.Blood"), FColor(255, 51, 51)},
                        {FGameplayTag::RequestGameplayTag("Combat.Element.Chaos"), FColor::Purple},
                        {FGameplayTag::RequestGameplayTag("Combat.Element.Cosmic"), FColor(213, 127, 209)},
                        {FGameplayTag::RequestGameplayTag("Combat.Element.Dark"), FColor(0, 0, 0x33)},
                        {FGameplayTag::RequestGameplayTag("Combat.Element.Earth"), FColor(210, 180, 140)},
                        {FGameplayTag::RequestGameplayTag("Combat.Element.Electric"), FColor::Yellow},
                        {FGameplayTag::RequestGameplayTag("Combat.Element.Ethereal"), FColor::Emerald},
                        {FGameplayTag::RequestGameplayTag("Combat.Element.Fire"), FColor::Red},
                        {FGameplayTag::RequestGameplayTag("Combat.Element.Force"), FColor(96, 96, 96)},
                        {FGameplayTag::RequestGameplayTag("Combat.Element.Light"), FColor::White},
                        {FGameplayTag::RequestGameplayTag("Combat.Element.Poison"), FColor(255, 102, 255)},
                        {FGameplayTag::RequestGameplayTag("Combat.Element.Water"), FColor::Blue},
                        {FGameplayTag::RequestGameplayTag("Combat.Element.Wind"), FColor(51, 255, 153)},
                        {FGameplayTag::EmptyTag, FColor::White}};

   purgeTagMap = {{FGameplayTag::RequestGameplayTag("Combat.Effect.Purge.One"), 1},   {FGameplayTag::RequestGameplayTag("Combat.Effect.Purge.Two"), 2},
                  {FGameplayTag::RequestGameplayTag("Combat.Effect.Purge.Three"), 3}, {FGameplayTag::RequestGameplayTag("Combat.Effect.Purge.Four"), 4},
                  {FGameplayTag::RequestGameplayTag("Combat.Effect.Purge.Five"), 5},  {FGameplayTag::RequestGameplayTag("Combat.Effect.Purge.Six"), 6},
                  {FGameplayTag::RequestGameplayTag("Combat.Effect.Purge.All"), 100}};

   supportTags   = FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Skill.Category.Support"));
   offensiveTags = FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Skill.Category.Offensive"));

   SingletonManager->AddToRoot();
}
