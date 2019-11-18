// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "DialogBox.h"
#include "RTSGameMode.h"
#include "WorldObjects/NPC.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "EventSystem/RTSConditional.h"
#include "UI/HUDManager.h"
#include "DialogSystem/DialogUI.h"

void UDialogBox::InitDialogPortraitMaps()
{
}

UDialogBox::UDialogBox(const FObjectInitializer& ObjectInitializer)
{
   InitDialogPortraitMaps();
}

void UDialogBox::NativeConstruct()
{
   gameModeRef   = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
   controllerRef = Cast<AUserInput>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
   Super::NativeConstruct();
}

bool UDialogBox::OnWidgetAddToViewport_Implementation()
{
   if (!hasValidDialog) {
      UE_LOG(LogTemp, Warning, TEXT("Set dialog before calling AddHud on the dialog UI widget"))
      return false;
   }
   return true;
}

void UDialogBox::SetConversation(FName newDialogName)
{
   dialogTopic = newDialogName;
   dialogLines = UDialogManager::Get().LoadDialog(dialogTopic, dialogTopic.ToString() + FString("did not load correctly"));
   if (dialogLines.Num() > 0) hasValidDialog = true;
}

void UDialogBox::SetDialogLines(TArray<FDialogData> newDialogLines)
{
   dialogLines = newDialogLines;
   if (dialogLines.Num() > 0) hasValidDialog = true;
}

#define LOCTEXT_NAMESPACE "Dialog"

const FDialogData& UDialogBox::GetNextLine()
{
   // If the current node is connected to two dialog nodes
   if (dialogLines[currentNodeNum].nextDialogs.Num() > 1) {
      HandleChoices();
   }
   // If the current node is only connected to one dialogNode
   else if (dialogLines[currentNodeNum].nextDialogs.Num() == 1) {
      // Move to the next node.  If this node is a regular node, this is all we have to do
      currentNodeNum = dialogLines[currentNodeNum].nextDialogs[0];
      // if this dialogNode is a trigger node (activates an event trigger)
      if (dialogLines[currentNodeNum].actorName == "Trigger") {
         return HandleTriggers();
      }
      // else if this dialogNode is a condition node
      else if (dialogLines[currentNodeNum].actorName == "Condition") {
         return HandleConditions();
      }
   }
   // if this node is not connected to any other dialog nodes
   else {
      // If there are no dialogNodes connected to this one, this is the end of the conversation so return some ending marker
      ResetDialog();
      return defaultDialog;
   }

   return dialogLines[currentNodeNum];
}

const FDialogData& UDialogBox::PickChoice(int choice)
{
   // Not the end of the dialog, so we'll get a valid choice from clicking on a choice button
   currentNodeNum = dialogLines[currentNodeNum].nextDialogs[choice];

   ///---Dialog node lookahead to check what kind of node is next ---
   // if the choice we picked leads to a dialogNode with more than one connection
   if (dialogLines[currentNodeNum].nextDialogs.Num() > 1) {
      if (dialogLines[currentNodeNum].actorName == "Condition") { return HandleConditions(); }

      HandleChoices();
      return dialogLines[currentNodeNum];
   } else {
      // if the choice we picks leads to a trigger
      if (dialogLines[currentNodeNum].actorName == "Trigger") { return HandleTriggers(); }

      // else just return the node the choice leads to
      return dialogLines[currentNodeNum];
   }
}

void UDialogBox::HandleChoices()
{
   // Don't move onto next node, instead set the buttons to have the text on each of the next nodes and make it so if those buttons
   // have a next node number, that clicking on it leads to the dialogNode with that offset.
   TArray<FDialogData> choiceLines;
   for (int i : dialogLines[currentNodeNum].nextDialogs) {
      choiceLines.Emplace(dialogLines[i]);
   }
   // Call GetNextLine() again since we don't want to display the trigger text
   ShowChoices(choiceLines);
}

const FDialogData& UDialogBox::HandleTriggers()
{
   // GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString("Trigger Line!"));
   // Check for double slash (\\n) since when converting \n in FText to \n in FString, it replaces \n with \\n
   // Ensure there's three lines (TriggerType, TriggerObjects, TriggerValues)
   const TCHAR* delimArray[] = {TEXT("\n"), LINE_TERMINATOR};
   if (dialogLines[currentNodeNum].dialog.ToString().ParseIntoArray(parsingInfo, delimArray, 2, false) == 3) {
      FTriggerData triggerData = FTriggerData();
      triggerData.triggerType  = static_cast<ETriggerType>(FCString::Atoi(*parsingInfo[0]));

      if (!parsingInfo[1].IsEmpty()) { parsingInfo[1].ParseIntoArray(triggerData.triggerObjects, TEXT(",")); }
      if (!parsingInfo[2].IsEmpty()) { parsingInfo[2].ParseIntoArray(triggerData.triggerValues, TEXT(",")); }

      gameModeRef->GetTriggerManager()->ActivateTrigger(triggerData);
   }
   return GetNextLine();
}

const FDialogData& UDialogBox::HandleConditions()
{
   // Check for double slash (\\n) since when converting \n in FText to \n in FString, it replaces \n with \\n
   // Ensure there's three lines (ConditionType, IsReversed, ConditionValues)
   if (dialogLines[currentNodeNum].dialog.ToString().ParseIntoArray(parsingInfo, TEXT("\n"), false) == 3) {
      FConditionData conditionData  = FConditionData();
      conditionData.conditionalType = static_cast<EConditionalType>(FCString::Atoi(*parsingInfo[0]));
      if (!parsingInfo[1].IsEmpty()) {
         // If this field is a 1 or T, then represent that as true.  Anything else will be false
         if (parsingInfo[1] == "1" || parsingInfo[1] == "T") {
            conditionData.reverseResult = true;
         } else
            conditionData.reverseResult = false;
      }
      if (!parsingInfo[2].IsEmpty()) parsingInfo[2].ParseIntoArray(conditionData.conditionalValues, TEXT(","));

      // If this conditition is true, we move onto the first node

      if (dialogLines[currentNodeNum].nextDialogs.Num() > 1)
         return gameModeRef->GetConditionalManager()->GetCondition(conditionData) ? PickChoice(0) : PickChoice(1);
      else if (dialogLines[currentNodeNum].nextDialogs.Num() == 1)
         return gameModeRef->GetConditionalManager()->GetCondition(conditionData) ? GetNextLine() : defaultDialog;
      else
         return defaultDialog;
   }
   return defaultDialog;
}

void UDialogBox::ResetDialog()
{
   switch (dialogSource) {
      case EDialogSource::npc:
         controllerRef->GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_Social));
         break;
      case EDialogSource::conversation: controllerRef->GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_Social)); break;
      case EDialogSource::trigger: break;
	   case EDialogSource::none: controllerRef->GetBasePlayer()->interactedHero = nullptr; break;
      default: break;
   }

   hasValidDialog = false;
   currentNodeNum = 0;
   controllerRef->GetHUDManager()->AddHUD("", EDialogSource::none);

   // TODO: Figure out if resetting topic is good here
}

#undef LOCTEXT_NAMESPACE
