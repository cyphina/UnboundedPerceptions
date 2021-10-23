// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "GameplayTagsManager.h"
#include "DialogWheel.generated.h"

/**
 * Displays the conversation topics so we can navigate through them when choosing what to say
 */

class UDialogUI;

UCLASS()
class MYPROJECT_API UDialogWheel : public UMyUserWidget
{
   GENERATED_BODY()

 public:
   UDialogWheel();

   void NativeConstruct() override;

   UFUNCTION(BlueprintImplementableEvent, Category = "Dialog Wheel Functionality")
   void UpdateDialogWheelText();

   bool OnWidgetAddToViewport_Implementation() override;

 private:
   /**
    * @brief Used to select the next subcategory of topics in the dialog wheel.
    * @param nextIndex The index of the topic we selected in the dialog wheel.
    */
   UFUNCTION(BlueprintCallable, Category = "Dialog Wheel Functionality")
   void SelectNextConversationTopics(int nextIndex);

   /** Call this to get the previous set of conversation topics*/
   UFUNCTION(BlueprintCallable, Category = "Dialog Wheel Functionality")
   void SelectPreviousConversationTopics();

   /** Call this to get the previous set of conversation topics */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialog Wheel Functionality")
   FName GetConversationTopicName(int index) const { return conversationTopicTagNodes[index]->GetSimpleTagName(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialog Wheel Functionality")
   FName GetCurrentConversationTopicName() const { return currentlySelectedTopicNode->GetSimpleTagName(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialog Wheel Functionality")
   int GetCurrentConversationTopicCount() const { return conversationTopicTagNodes.Num(); }

   bool HandleSelectedLeafNode();

   void UpdateDialogWheel();

   UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true), Category = "References")
   UDialogUI* socialWindowRef;

   UPROPERTY()
   class AHUDManager* hudManagerRef;

   static const FName ROOT_DIALOG_NODE_NAME;

   /** These are the topics that can be seen due to being children of the currentlySelectedTopic*/
   TArray<FGameplayTagNode*> conversationTopicTagNodes;

   /** This is the currently selected topic*/
   FGameplayTagNode* currentlySelectedTopicNode;

   /** Last selected topic*/
   FGameplayTagNode* previouslySelectedTopicNode;
};
