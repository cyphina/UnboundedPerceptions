// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestJournalEntry.generated.h"

/*Class for widgets inside a quest journal*/

class AQuest;
class UQuestJournal;

UCLASS()
class MYPROJECT_API UQuestJournalEntry : public UUserWidget
{
   GENERATED_BODY()

 public:
   // if is on, button will be enabled
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "UI")
   void ToggleButtonEnabled(bool isOn);

   // update the color of the suggested level based on hero's level
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "UI")
   void LevelColorUpdate();

   UPROPERTY(BlueprintReadOnly, Category = "References", Meta = (ExposeOnSpawn = true))
   UQuestJournal* questJournal;

   UPROPERTY(BlueprintReadOnly, Category = "References", Meta = (ExposeOnSpawn = true))
   AQuest* assignedQuest;
};
