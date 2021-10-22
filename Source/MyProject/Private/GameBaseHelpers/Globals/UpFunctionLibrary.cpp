// Created 2/11/21 12:54 AM

#include "MyProject.h"
#include "UpFunctionLibrary.h"
#include "RTSGameMode.h"
#include "UpResourceManager.h"
#include "UserInput.h"

void UUpFunctionLibrary::ActivateTrigger(const FTriggerData& triggerData, const UObject* worldContextObject)
{
   if(ARTSGameMode* gameModeRef = Cast<ARTSGameMode>(worldContextObject->GetWorld()->GetAuthGameMode()))
   {
      gameModeRef->GetTriggerManager()->ActivateTrigger(triggerData);
   }
}

TArray<ABaseHero*> UUpFunctionLibrary::GetAllHeroes(const UObject* WorldContextObject)
{
   TArray<ABaseHero*> AllHeroesSpawned;

   if(UWorld* World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr)
   {
      for(TActorIterator<ABaseHero> ActItr(World, ABaseHero::StaticClass()); ActItr; ++ActItr)
      {
         AllHeroesSpawned.Add(*ActItr);
      }
   }

   return AllHeroesSpawned;
}

const TArray<ABaseHero*>& UUpFunctionLibrary::GetHeroes(const UObject* worldContextObject)
{
   AUserInput* controllerRef = Cast<AUserInput>(worldContextObject->GetWorld()->GetFirstPlayerController());
   check(controllerRef);

   return controllerRef->GetBasePlayer()->GetHeroes();
}

AUnit* UUpFunctionLibrary::GetFocusedUnit(const UObject* worldContextObject)
{
   AUserInput* controllerRef = Cast<AUserInput>(worldContextObject->GetWorld()->GetFirstPlayerController());
   check(controllerRef);

   return controllerRef->GetBasePlayer()->GetFocusedUnit();
}

ABaseHero* UUpFunctionLibrary::GetHeroFromIndex(const UObject* worldContextObject, int heroIndex)
{
   return GetHeroes(worldContextObject)[heroIndex];
}

AHUDManager* UUpFunctionLibrary::GetHUDManager(const UObject* worldContextObject)
{
   if(AUserInput* controllerRef = Cast<AUserInput>(worldContextObject->GetWorld()->GetFirstPlayerController()))
   {
      return controllerRef->GetHUDManager();
   }
   return nullptr;
}

FName UUpFunctionLibrary::GetStreamingLevelNameFromActor(const AActor* Actor)
{
   if(Actor)
   {
      if(const ULevel* Level = Actor->GetLevel())
      {
         return Level->GetOuter()->GetFName();
      }
   }
   return "No Stream Level";
}
