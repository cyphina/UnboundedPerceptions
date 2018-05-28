// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "DialogBox.h"
#include "MyGameInstance.h"
#include "DialogGraph.h"
#include "WorldObjects/NPC.h"
#include "UserInput.h"
#include "UI/HUDManager.h"

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
	gameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

#define LOCTEXT_NAMESPACE "Dialog"

const FDialogData& UDialogBox::GetNextLine()
{
	if(dialogLines[currentNodeNum].nextDialogs.Num())
	{
		//more than one next node?
		if(dialogLines[currentNodeNum].nextDialogs.Num() > 1)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString("WEEPUPS"));
			//then it's the player given a choice of what to say
			TArray<FDialogData> choiceLines;
			for(int i : dialogLines[currentNodeNum].nextDialogs)
			{
				choiceLines.Emplace(dialogLines[i]);
			}
			ShowChoices(choiceLines);
		}
		else
		{
			if (dialogLines[currentNodeNum].actorName == "Trigger")
			{
				//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString("WEEPUPS"));
				if(dialogLines[currentNodeNum].dialog.ToString().ParseIntoArrayLines(triggerInfo) > 0) {
					FTriggerData triggerData;
					triggerData.triggerType = static_cast<TriggerType>(FCString::Atoi(*triggerInfo[0]));
					triggerInfo[1].ParseIntoArray(triggerData.triggerObjects, TEXT(","));
					triggerInfo[2].ParseIntoArray(triggerData.triggerValues, TEXT(","));
					gameInstanceRef->GetTriggerManager()->ActivateTrigger(triggerData);
				}
			}
			currentNodeNum = dialogLines[currentNodeNum].nextDialogs[0];
		}
		return dialogLines[currentNodeNum];
	}
	//None left
	ResetDialog();
	return defaultDialog;
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
	if(dialogLines.Num() > 0)
		hasValidDialog = true;
}

void UDialogBox::ResetDialog()
{
	if (onDialogEndTrigger && onDialogEndTrigger->triggerType != TriggerType::None)
		gameInstanceRef->GetTriggerManager()->ActivateTrigger(*onDialogEndTrigger);
	
	hasValidDialog = false;
	currentNodeNum = 0;
}

#undef LOCTEXT_NAMESPACE
