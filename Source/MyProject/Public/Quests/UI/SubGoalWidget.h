// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Quest.h"
#include "SubGoalWidget.generated.h"

/**
 * Class for goal widgets inside the quest slot widgets (questJournal)
 */

class UQuestListSlot;

UCLASS()
class MYPROJECT_API USubGoalWidget : public UUserWidget
{
	GENERATED_BODY()

	/**Information about the goal the widget is displaying*/
	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true), Category = "Info")
		FGoalInfo goalInfo;
	
	/**Quest this goal belongs to*/
	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true), Category = "Info")
		AQuest* assignedQuest;

	/**Corresponding questSlotWidget this goal widget is a part of*/
	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true), Category = "Info")
		UQuestListSlot* questSlotWidget;

	/**Index of what subgoal this is in the quest*/
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true), Category = "Info")
		int goalIndex;

public:

	void NativeConstruct() override;

	/**Update display text on this goal whenever things change like we hunt something, or a goal is completed*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI")
		void UpdateText();

	/**Changes if button is enabled and how goal looks when selected or deselected*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI")
		void ToggleEnabled(bool shouldEnable);

	///---Accessors---
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accesors")
		FGoalInfo GetGoalInfo() const { return goalInfo; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accesors")
		AQuest* GetAssignedQuest() const { return assignedQuest; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accesors")
		UQuestListSlot* GetQuestSlotWidget() const { return questSlotWidget; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accesors")
		int GetGoalIndex() const { return goalIndex; }


};
