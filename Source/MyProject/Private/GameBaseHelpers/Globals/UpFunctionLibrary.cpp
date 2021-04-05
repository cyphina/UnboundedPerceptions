// Created 2/11/21 12:54 AM

#include "MyProject.h"
#include "UpFunctionLibrary.h"
#include "RTSGameMode.h"
#include "UserInput.h"

void UUpFunctionLibrary::ActivateTrigger(const FTriggerData& triggerData, const UObject* worldContextObject)
{
   if(ARTSGameMode* gameModeRef = Cast<ARTSGameMode>(worldContextObject->GetWorld()->GetAuthGameMode()))
   {
      gameModeRef->GetTriggerManager()->ActivateTrigger(triggerData);
   }
}

const TArray<ABaseHero*>& UUpFunctionLibrary::GetHeroes(const UObject* worldContextObject)
{
   AUserInput* controllerRef = Cast<AUserInput>(worldContextObject->GetWorld()->GetFirstPlayerController());
   check(controllerRef);

   return controllerRef->GetBasePlayer()->GetHeroes();
}

AHUDManager* UUpFunctionLibrary::GetHUDManager(const UObject* worldContextObject)
{
   if(AUserInput* controllerRef = Cast<AUserInput>(worldContextObject->GetWorld()->GetFirstPlayerController()))
   {
      return controllerRef->GetHUDManager();
   }
   return nullptr;
}
