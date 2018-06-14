// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "DialogWheel.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "UI/HUDManager.h"
#include "DialogSystem/DialogUI.h"
#include "WorldObjects/NPC.h"

UDialogWheel::UDialogWheel()
{
	
}

void UDialogWheel::SelectNextConversationTopics(int selectedIndex)
{
	previouslySelectedTopicNode = currentlySelectedTopicNode;
	currentlySelectedTopicNode = conversationTopicTagNodes[selectedIndex];

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, FString::FromInt(currentlySelectedTopicNode->GetChildTagNodes().Num()));
	
	//If this is a root node in our tree of dialog options (gameplaytag tree)
	if (currentlySelectedTopicNode->GetChildTagNodes().Num() == 0)
	{
		//Close the social menu, and add the textbox withh the conversation loaded up
		CPC->GetHUDManager()->AddHUD(static_cast<int>(HUDs::HS_Social));
		CPC->GetHUDManager()->AddHUDDialog(socialWindowRef->GetNPC()->GetConversationName(currentlySelectedTopicNode->GetCompleteTag()), socialWindowRef->GetNPC()->GetOnDialogFinishedTrigger());
		//Add this to our list of conversed dialogs (its a set so don't worry about doubles)
		socialWindowRef->GetNPC()->AddConversedDialog(socialWindowRef->GetNPC()->GetConversationName(currentlySelectedTopicNode->GetCompleteTag()));
		return;
	}

	conversationTopicTagNodes.Empty();

	for (TSharedPtr<FGameplayTagNode> it : currentlySelectedTopicNode->GetChildTagNodes())
	{
		if (CPC->GetBasePlayer()->GetDialogTopics().HasTag(it->GetSingleTagContainer().First()))
			conversationTopicTagNodes.Add(it.Get());
	}
	UpdateDialogWheelText();
}

void UDialogWheel::SelectPreviousConversationTopics()
{
	if (currentlySelectedTopicNode->GetSimpleTagName() != "Dialog")
	{
		currentlySelectedTopicNode = previouslySelectedTopicNode;
		if(previouslySelectedTopicNode->GetSimpleTagName() != "Dialog")
			previouslySelectedTopicNode = previouslySelectedTopicNode->GetParentTagNode().Get();
		else
			previouslySelectedTopicNode = nullptr;
	}
	else
	{
		socialWindowRef->SetMainView();
		return;
	}

	conversationTopicTagNodes.Empty();
	for (TSharedPtr<FGameplayTagNode> it : currentlySelectedTopicNode->GetChildTagNodes())
	{
		if (CPC->GetBasePlayer()->GetDialogTopics().HasTag(it->GetSingleTagContainer().First()))
			conversationTopicTagNodes.Add(it.Get());
	}
	UpdateDialogWheelText();
}

void UDialogWheel::Construct_Implementation()
{
	Super::Construct_Implementation();
}

bool UDialogWheel::OnWidgetAddToViewport_Implementation()
{
	currentlySelectedTopicNode = UGameplayTagsManager::Get().FindTagNode("Dialog").Get();
	previouslySelectedTopicNode = nullptr;

	conversationTopicTagNodes.Empty();
	for (TSharedPtr<FGameplayTagNode> it : currentlySelectedTopicNode->GetChildTagNodes())
	{
		//not running before we actually learn teh dialogs in camerapawncontroller fix dis
		if (CPC->GetBasePlayer()->GetDialogTopics().HasTag(it->GetSingleTagContainer().First()))
			conversationTopicTagNodes.Add(it.Get());
	}

	UpdateDialogWheelText();
	return true;
}



