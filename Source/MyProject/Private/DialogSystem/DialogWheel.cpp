// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "DialogWheel.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "UI/HUDManager.h"
#include "DialogSystem/NPCSocialMenu.h"
#include "DialogSystem/DialogBox.h"
#include "WorldObjects/NPC.h"

#include "RTSGameMode.h"
#include "Quests/QuestManager.h"

#include "NPCDelegateStore.h"

UDialogWheel::UDialogWheel()
{
}

void UDialogWheel::NativeConstruct()
{
   Super::NativeConstruct();
}

bool UDialogWheel::OnWidgetAddToViewport_Implementation()
{
   currentlySelectedTopicNode  = UGameplayTagsManager::Get().FindTagNode("Dialog").Get();
   previouslySelectedTopicNode = nullptr;
   UpdateDialogWheel();
   return true;
}

bool UDialogWheel::HandleSelectedLeafNode()
{
   if(currentlySelectedTopicNode->GetChildTagNodes().Num() == 0) {
      hudManagerRef->AddHUD(static_cast<int>(EHUDs::HS_Social));

      const FName conversationName = socialWindowRef->GetNPC()->GetConversationName(currentlySelectedTopicNode->GetCompleteTag());

      hudManagerRef->ShowDialogWithSource(conversationName, EDialogBoxCloseCase::finishedNPCConvo);
      NPCEvents::OnNPCTalkedEvent.Broadcast(socialWindowRef->GetNPC(), currentlySelectedTopicNode->GetCompleteTag());
      socialWindowRef->GetNPC()->AddConversedDialog(conversationName);
      return true;
   }
   return false;
}

void UDialogWheel::UpdateDialogWheel()
{
   conversationTopicTagNodes.Empty();
   for(TSharedPtr<FGameplayTagNode> it : currentlySelectedTopicNode->GetChildTagNodes()) {
      if(CPC->GetBasePlayer()->GetDialogTopics().HasTag(it->GetSingleTagContainer().First())) conversationTopicTagNodes.Add(it.Get());
   }

   UpdateDialogWheelText();
}

void UDialogWheel::SelectNextConversationTopics(int selectedIndex)
{
   previouslySelectedTopicNode = currentlySelectedTopicNode;
   currentlySelectedTopicNode  = conversationTopicTagNodes[selectedIndex];

   if(HandleSelectedLeafNode()) return;

   UpdateDialogWheel();
}

void UDialogWheel::SelectPreviousConversationTopics()
{
   if(currentlySelectedTopicNode->GetSimpleTagName() != ROOT_DIALOG_NODE_NAME) {
      currentlySelectedTopicNode = previouslySelectedTopicNode;
      if(previouslySelectedTopicNode->GetSimpleTagName() != ROOT_DIALOG_NODE_NAME)
         previouslySelectedTopicNode = previouslySelectedTopicNode->GetParentTagNode().Get();
      else
         previouslySelectedTopicNode = nullptr;
   } else {
      // If this was the root node, and we want to go back, then it means we're going back from this dialog menu back to the different NPC interaction option screen
      socialWindowRef->SetMainView();
      return;
   }

   UpdateDialogWheel();
}
