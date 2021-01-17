#include "MyProject.h"
#include "MySpell.h"
#include "SpellDataManager.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffects/RTSDamageEffect.h"


USpellDataManager* USpellDataManager::SingletonManager = nullptr;



USpellDataManager::USpellDataManager()
{

}

void USpellDataManager::SetupSpells(UDataTable* spellLookupTable)
{
   SetupCachedSpellClassMap();
}

void USpellDataManager::SetupCachedSpellClassMap()
{
   for(TObjectIterator<UClass> it; it; ++it)
   {
      auto x = *it;
      
      if((*it)->IsChildOf(UMySpell::StaticClass()))
      {
         FProperty* prop = it->FindPropertyByName("spellDefaults");
         if(FStructProperty* p = prop ? CastField<FStructProperty>(prop) : nullptr)
         {
            FSpellDefaults* structAddress      = prop->ContainerPtrToValuePtr<FSpellDefaults>((*it)->GetDefaultObject());
            spellClasses.Add(structAddress->nameTag, *it);
         }
      }
   }
}

void USpellDataManager::InitializeManager()
{
   check(!SingletonManager);
   SingletonManager = NewObject<USpellDataManager>(GetTransientPackage(), NAME_None);
   SingletonManager->AddToRoot();
}
