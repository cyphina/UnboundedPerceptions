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
   for(FConditionData condition : conditions) {
      if(!gameModeRef->GetConditionalManager()->GetCondition(condition)) {
         FTriggerData triggerData;

         if(customDialogConversation == "") {
            TArray<FDialogData> dialogData;
            triggerData.enabled     = true;
            triggerData.numCalls    = 1;
            triggerData.triggerType = ETriggerType::DisplayDialogTrigger;
            // Create a message formulaically depending on the conditions
            triggerData.triggerValues[0] = gameModeRef->GetConditionalManager()->GetConditionString(conditions).ToString();
            gameModeRef->GetTriggerManager()->ActivateTrigger(triggerData);
         } else { // If we have a custom messagecustom message
                  // Create a trigger so we don't have to dependency inject the AHUDManager
            triggerData.enabled          = true;
            triggerData.numCalls         = 1;
            triggerData.triggerType      = ETriggerType::DisplayConversationTrigger;
            triggerData.triggerValues[0] = customDialogConversation.ToString();
            gameModeRef->GetTriggerManager()->ActivateTrigger(triggerData);
         }
         return false;
      }
   }

   if(decoratedInteractable)
      decoratedInteractable->Interact();

   return true;
}
