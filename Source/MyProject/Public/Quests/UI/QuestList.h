// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/AnimHudWidget.h"
#include "QuestList.generated.h"

/* A widget to display different quests in a list on the side of the UI Menu.  Includes Animations*/

class AQuest;
class UQuestListSlot;
class ARTSGameMode;

UCLASS()
class MYPROJECT_API UQuestList : public UAnimHudWidget
{
   GENERATED_BODY()

 public:
   void NativeConstruct() override;
   bool OnWidgetAddToViewport_Implementation() override;

   /**Add a new widget to quest list when a new quest is added*/
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
   void AddToQuestList(AQuest* questActor);

   /**Remove quest list widget and its subgoals*/
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
   void RemoveFromQuestList(AQuest* questActor);

   /**Hide distance indicator*/
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
   void ToggleDistanceIndicatorVisibility(bool shouldSee);

   /**Set distance indicator*/
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
   void SetDistanceText(int newDistance);

   /**Gets a quest's quest list slot (what slot is it in the quest list)*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   UQuestListSlot* GetQuestListSlot(AQuest* quest) const;

   UPROPERTY(BlueprintReadWrite, category = "References")
   AQuest* currentlySelectedQuest = nullptr;

   UPROPERTY(BlueprintReadWrite, category = "Data")
   TArray<UQuestListSlot*> questListSlots;

 private:
   UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true, ExposeOnSpawn = true), category = "References")
   ARTSGameMode* gameModeRef;
};
