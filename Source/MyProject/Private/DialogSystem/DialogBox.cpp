// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "DialogBox.h"
#include "RTSGameMode.h"
#include "WorldObjects/NPC.h"
#include "UserInput.h"
#include "EventSystem/RTSConditional.h"
#include "UI/HUDManager.h"
#include "DialogSystem/DialogUI.h"

void UDialogBox::InitDialogPortraitMaps()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> defaultPortrait{ TEXT("/Game/RTS_Tutorial/Textures/girldoge") };
	if (defaultPortrait.Succeeded())
		dialogPortraitMap.Add("default", defaultPortrait.Object);
}

UDialogBox::UDialogBox(const FObjectInitializer& ObjectInitializer)
{
	InitDialogPortraitMaps();
}

void UDialogBox::Construct_Implementation()
{
	Super::Construct_Implementation();
	gameModeRef = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

bool UDialogBox::OnWidgetAddToViewport_Implementation()
{
	if (!hasValidDialog)
	{
		UE_LOG(LogTemp, Warning, TEXT("Set dialog before calling AddHud on the dialog UI widget"))
			return false;
	}
	return true;
}

void UDialogBox::SetConversation(FName newDialogName)
{
	dialogTopic = newDialogName;
	dialogLines = UDialogManager::Get().LoadDialog(dialogTopic, dialogTopic.ToString() + FString("did not load correctly"));
	if (dialogLines.Num() > 0)
		hasValidDialog = true;
}

void UDialogBox::SetDialogLines(TArray<FDialogData> newDialogLines)
{
	dialogLines = newDialogLines;
	if (dialogLines.Num() > 0)
		hasValidDialog = true;
}

#define LOCTEXT_NAMESPACE "Dialog"

const FDialogData& UDialogBox::GetNextLine()
{
	//If the current node is connected to two dialog nodes
	if (dialogLines[currentNodeNum].nextDialogs.Num() > 1)
	{
		HandleChoices();
	}
	//If the current node is only connected to one dialogNode
	else if (dialogLines[currentNodeNum].nextDialogs.Num() == 1)
	{
		//Move to the next node.  If this node is a regular node, this is all we have to do 
		currentNodeNum = dialogLines[currentNodeNum].nextDialogs[0];
		//if this dialogNode is a trigger node (activates an event trigger)
		if (dialogLines[currentNodeNum].actorName == "Trigger")
		{
			HandleTriggers();
		}
		//else if this dialogNode is a condition node
		else if(dialogLines[currentNodeNum].actorName == "Condition")
		{
			HandleConditions();
		}
	}
	//if this node is not connected to any other dialog nodes
	else
	{
		//If there are no dialogNodes connected to this one, this is the end of the conversation so return some ending marker
		ResetDialog();
		return defaultDialog;
	}

	return dialogLines[currentNodeNum];
}

const FDialogData& UDialogBox::PickChoice(int choice)
{
	if (choice == -1)
	{
		//If there are no dialogNodes connected to this one, this is the end of the conversation so return some ending marker
		ResetDialog();
		return defaultDialog;
	}

	//Not the end of the dialog, so we'll get a valid choice from clicking on a choice button
	currentNodeNum = choice;

	//if the choice we picked leads to a dialogNode with more than one connection
	if (dialogLines[currentNodeNum].nextDialogs.Num() > 1)
	{
		if (dialogLines[currentNodeNum].actorName == "Condition")
		{
			HandleConditions();
			return dialogLines[currentNodeNum];
		}

		HandleChoices();
		return dialogLines[currentNodeNum];
	}
	else
	{
		//if the choice we picks leads to a trigger
		if (dialogLines[currentNodeNum].actorName == "Trigger")
		{
			HandleTriggers();

			//If after activating this trigger, we reach the end of dialog (it will have already reset the dialog)
			if (currentNodeNum == 0)
			{
				//ResetDialog();
				return defaultDialog;
			}
			return dialogLines[currentNodeNum];
		}

		//return the node the choice leads to
		return dialogLines[choice];
	}
}

void UDialogBox::HandleChoices()
{
	//Don't move onto next node, instead set the buttons to have the text on each of the next nodes and make it so if those buttons
	//have a next node number, that clicking on it leads to the dialogNode with that offset.
	TArray<FDialogData> choiceLines;
	for (int i : dialogLines[currentNodeNum].nextDialogs)
	{
		choiceLines.Emplace(dialogLines[i]);
	}
	//Call GetNextLine() again since we don't want to display the trigger text
	ShowChoices(choiceLines);
}

void UDialogBox::HandleTriggers()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString("Trigger Line!"));
	//Check for double slash (\\n) since when converting \n in FText to \n in FString, it replaces \n with \\n
	//Ensure there's three lines (TriggerType, TriggerObjects, TriggerValues)
	if (dialogLines[currentNodeNum].dialog.ToString().ParseIntoArray(parsingInfo, TEXT("\\n"), false) == 3) {
		FTriggerData triggerData;
		triggerData.triggerType = static_cast<ETriggerType>(FCString::Atoi(*parsingInfo[0]));
		if (!parsingInfo[1].IsEmpty())
		{
			parsingInfo[1].ParseIntoArray(triggerData.triggerObjects, TEXT(","));
		}
		if (!parsingInfo[2].IsEmpty())
		{
			parsingInfo[2].ParseIntoArray(triggerData.triggerValues, TEXT(","));
		}

		gameModeRef->GetTriggerManager()->ActivateTrigger(triggerData);
	}
	GetNextLine();
}

void UDialogBox::HandleConditions()
{
	//Check for double slash (\\n) since when converting \n in FText to \n in FString, it replaces \n with \\n
	//Ensure there's three lines (ConditionType, IsReversed, ConditionValues)
	if (dialogLines[currentNodeNum].dialog.ToString().ParseIntoArray(parsingInfo, TEXT("\\n"), false) == 3) {
		FConditionData conditionData;
		conditionData.conditionalType = static_cast<EConditionalType>(FCString::Atoi(*parsingInfo[0]));
		if (!parsingInfo[1].IsEmpty())
		{
			//If this field is a 1 or T, then represent that as true.  Anything else will be false
			if (parsingInfo[1] == "1" || parsingInfo[1] == "T")
			{
				conditionData.reverseResult = true;
			}
			else
				conditionData.reverseResult = false;
		}
		if (!parsingInfo[2].IsEmpty())
			parsingInfo[2].ParseIntoArray(conditionData.conditionalValues, TEXT(","));

		//If this conditition is true, we move onto the first node
		 
		gameModeRef->GetConditionalManager()->GetCondition(conditionData) ? PickChoice(dialogLines[currentNodeNum].nextDialogs[0]) : PickChoice(dialogLines[currentNodeNum].nextDialogs[1]); 
	}
}

void UDialogBox::ResetDialog()
{
	if (onDialogEndTrigger && onDialogEndTrigger->triggerType != ETriggerType::None)
		gameModeRef->GetTriggerManager()->ActivateTrigger(*onDialogEndTrigger);

	hasValidDialog = false;
	currentNodeNum = 0;
	//TODO: Figure out if resetting topic is good here
}

#undef LOCTEXT_NAMESPACE
