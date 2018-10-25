// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "ConditionalInteractableDecorator.h"
#include "RTSGameMode.h"
#include "UserInput.h"
#include "DialogSystem/DialogStructs.h"
#include "DialogBox.h"
#include "UI/HUDManager.h"

void UConditionalInteractableDecorator::Init()
{
   UInteractableActorDecoratorBase::Init();
   cpcRef      = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
   gameModeRef = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode());
}

bool UConditionalInteractableDecorator::Interact()
{
   for (FConditionData condition : conditions) {
      if (!gameModeRef->GetConditionalManager()->GetCondition(condition)) {
         if (customDialogConversation == "") {
            TArray<FDialogData> dialogData;
            dialogData.Emplace(TArray<int>(), gameModeRef->GetConditionalManager()->GetConditionString(conditions), "");
            cpcRef->GetHUDManager()->AddHUD(MoveTemp(dialogData), EDialogSource::none);
         } else {
            cpcRef->GetHUDManager()->AddHUD(customDialogConversation, EDialogSource::none);
         }
         return false;
      }
   }

   if (decoratedInteractable) decoratedInteractable->Interact();

   return true;
}
