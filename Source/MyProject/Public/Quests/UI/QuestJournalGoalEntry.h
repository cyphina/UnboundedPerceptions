// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Quests/Quest.h"
#include "QuestJournalGoalEntry.generated.h"

/**
 * Icon inside quest journal the represents the goals (that are completed, failed, or in progress) for a selected quest entry
 * These are recreated as goals get completed/failed, and have to be updated when journal is open similar to SubGoalWidgets.
 */

UCLASS()
class MYPROJECT_API UQuestJournalGoalEntry : public UUserWidget
{
   GENERATED_BODY()

   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
   int goalIndex;

   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
   FGoalInfo goalInfo;

   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
   AQuest* questRef;

 public:
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
   void Update();
};
