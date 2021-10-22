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
}

bool UConditionalInteractableDecorator::Interact()
{
   for(FConditionData Condition : conditions)
   {
      if(const ARTSGameMode* GameMode = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode()))
      {
         const bool ConditionEval = !GameMode->GetConditionalManager()->GetCondition(Condition);
         if(ConditionEval)
         {
            if(customDialogConversation == "")
            {
               TArray<FDialogData> dialogData;
               const FText         ConditionString = GameMode->GetConditionalManager()->GetConditionString(conditions);
               dialogData.Add(FDialogData({}, ConditionString, TEXT("")));

               if(const AUserInput* PC = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()))
               {
                  PC->GetHUDManager()->ShowDialogCustomLines(dialogData, EDialogBoxCloseCase::finishedInteractableDialog);
               }
            }
            else
            {
               if(const AUserInput* PC = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()))
               {
                  PC->GetHUDManager()->ShowDialogWithSource(customDialogConversation, EDialogBoxCloseCase::finishedInteractableDialog);
               }
            }
            return false;
         }
      }
   }

   if(decoratedInteractable)
   {
      decoratedInteractable->Interact();
   }

   return true;
}
