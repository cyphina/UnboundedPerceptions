// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "DialogWheel.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "UI/HUDManager.h"
#include "DialogSystem/DialogUI.h"
#include "DialogSystem/DialogBox.h"
#include "WorldObjects/NPC.h"

#include "RTSGameMode.h"
#include "Quests/QuestManager.h"

UDialogWheel::UDialogWheel()
{
}

void UDialogWheel::SelectNextConversationTopics(int selectedIndex)
{
   previouslySelectedTopicNode = currentlySelectedTopicNode;
   currentlySelectedTopicNode  = conversationTopicTagNodes[selectedIndex];

   //If this is a root node in our tree of dialog options (gameplaytag tree) then we talk about this topic
   if (currentlySelectedTopicNode->GetChildTagNodes().Num() == 0) {
      //Close the social menu, and add the textbox withh the conversation loaded up
      hudManagerRef->AddHUD(static_cast<int>(HUDs::HS_Social));

      FName conversationName = socialWindowRef->GetNPC()->GetConversationName(currentlySelectedTopicNode->GetCompleteTag());

	  //Setup textbox and update any records that keep track of who we talked to
      hudManagerRef->ShowDialogWithSource(conversationName, EDialogBoxCloseCase::finishedNPCConvo);
      CPC->GetGameMode()->GetQuestManager()->OnTalkNPC(socialWindowRef->GetNPC(), currentlySelectedTopicNode->GetCompleteTag());
      socialWindowRef->GetNPC()->AddConversedDialog(conversationName);
      return;
   }

   conversationTopicTagNodes.Empty();

   //If this isn't a root node, show the child nodes on the wheel
   for (TSharedPtr<FGameplayTagNode> it : currentlySelectedTopicNode->GetChildTagNodes()) {
      if (CPC->GetBasePlayer()->GetDialogTopics().HasTag(it->GetSingleTagContainer().First())) conversationTopicTagNodes.Add(it.Get());
   }
   UpdateDialogWheelText();
}

void UDialogWheel::SelectPreviousConversationTopics()
{
   if (currentlySelectedTopicNode->GetSimpleTagName() != "Dialog") {
      currentlySelectedTopicNode = previouslySelectedTopicNode;
      if (previouslySelectedTopicNode->GetSimpleTagName() != "Dialog")
         previouslySelectedTopicNode = previouslySelectedTopicNode->GetParentTagNode().Get();
      else
         previouslySelectedTopicNode = nullptr;
   } else {
      // If these are the root topics, bring the social window back
      socialWindowRef->SetMainView();
      return;
   }

   conversationTopicTagNodes.Empty();
   for (TSharedPtr<FGameplayTagNode> it : currentlySelectedTopicNode->GetChildTagNodes()) {
      if (CPC->GetBasePlayer()->GetDialogTopics().HasTag(it->GetSingleTagContainer().First())) conversationTopicTagNodes.Add(it.Get());
   }
   UpdateDialogWheelText();
}

void UDialogWheel::NativeConstruct()
{
   Super::NativeConstruct();
}

bool UDialogWheel::OnWidgetAddToViewport_Implementation()
{
   currentlySelectedTopicNode  = UGameplayTagsManager::Get().FindTagNode("Dialog").Get();
   previouslySelectedTopicNode = nullptr;
   conversationTopicTagNodes.Empty();

   //Display only the topics of the base dialog topic only if we learned them
   for (TSharedPtr<FGameplayTagNode> it : currentlySelectedTopicNode->GetChildTagNodes()) {
      if (CPC->GetBasePlayer()->GetDialogTopics().HasTag(it->GetSingleTagContainer().First())) conversationTopicTagNodes.Add(it.Get());
   }

   UpdateDialogWheelText();
   return true;
}
