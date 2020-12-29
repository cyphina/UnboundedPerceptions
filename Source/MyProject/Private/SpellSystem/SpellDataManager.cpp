// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "MySpell.h"
#include "SpellDataManager.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "SpellTargetingTypes.h"
#include "GameplayEffects/DamageEffect.h"

USpellDataManager* USpellDataManager::SingletonManager = nullptr;

const FUpSpellTargeting_None*   USpellDataManager::spellNoTarget     = NewObject<FUpSpellTargeting_None>(SingletonManager);
const FUpSpellTargeting_SingleUnit* USpellDataManager::spellSingleTarget = NewObject<FUpSpellTargeting_SingleUnit>(SingletonManager);
const FUpSpellTargeting_Area*   USpellDataManager::spellAreaTarget   = NewObject<FUpSpellTargeting_Area>(SingletonManager);
const FUpSpellTargeting_Vector* USpellDataManager::spellVectorTarget = NewObject<FUpSpellTargeting_Vector>(SingletonManager);

const TMap<FGameplayTag, const FUpSpellTargeting*> USpellDataManager::spellTargetings = {{FGameplayTag::RequestGameplayTag("Spell.Targeting.None"), spellNoTarget},
                                                                                         {FGameplayTag::RequestGameplayTag("Spell.Targeting.Single"), spellSingleTarget},
                                                                                         {FGameplayTag::RequestGameplayTag("Spell.Targeting.Area"), spellAreaTarget},
                                                                                         {FGameplayTag::RequestGameplayTag("Spell.Targeting.Vector"), spellVectorTarget}};

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
   if(spellLookupTable) {
      TArray<FSpellbookLookupRow*> table;
      TArray<FName>                rowNames = spellLookupTable->GetRowNames();
      FSpellInfo                   spellInfo;

      spellLookupTable->GetAllRows<FSpellbookLookupRow>(ContextString, table);
      // UE_LOG(LogTemp, Warning, TEXT("%d"), rowNames.Num());
      if(rowNames.Num() > 0 && table.Num() > 0) {
         int rowName;
         for(int i = 0; i < rowNames.Num(); ++i) {
            rowName                 = FCString::Atoi(*rowNames[i].ToString());
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
            spellInfo.targeting     = MakeSpellTargeting(table[i]->Targeting);
            spellInfo.duration      = table[i]->Duration;
            spellInfo.damage        = table[i]->Damage;
            spellInfo.period        = table[i]->Period;
            spellInfo.AOE           = table[i]->AOE;
            spellInfo.preReqs       = table[i]->PreReqs;

            spellClasses.Add(rowName, table[i]->spellClass);
            spells.Add(rowName, spellInfo);
         }
      }
   }
}

const FUpSpellTargeting* USpellDataManager::MakeSpellTargeting(FGameplayTag targetTag)
{
   return spellTargetings[targetTag];
}

void USpellDataManager::InitializeManager()
{
   check(!SingletonManager);
   SingletonManager = NewObject<USpellDataManager>(GetTransientPackage(), NAME_None);
   SingletonManager->AddToRoot();
}
