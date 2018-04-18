// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestListSlot.generated.h"

/* Slots that go inside the quest list showing quest name and the subgoals */

class AQuest;
class UQuestManager;
class USubGoalWidget;

UCLASS()
class MYPROJECT_API UQuestListSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//Creates a subwidget for each subgoal of a quest
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "QuestList UI")
		void GenerateSubWidgets(); 

	//Updates quest entry in our list; updates quest name and color based on its category
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "QuestList UI")
		void UpdateQuestEntry();

	//When a subgoal is selected we'll select it's corresponding quest and change the UI to indicate that
	UFUNCTION(BlueprintCallable, Category = "QuestList UI")
		void SelectSubGoal(int subGoalIndex); 

	//When we select a different quest
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "QuestList UI")
		void OnQuestSelected(); 

	//Add a subgoal widget to this questlist
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "QuestList UI")
		void AddSubGoalWidget(AQuest* questRef, int goalIndex); 

	//Remove a subgoal widget from this questlist
	UFUNCTION(BlueprintCallable, Category = "QuestList UI")
		void RemoveSubGoalWidget(int goalIndex); 

	//is this quest selected
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helper")
		bool IsCurrentQuest();

///---ACCESSORS---
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
		AQuest* GetAssignedQuest() const { return assignedQuest; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
		UQuestManager* GetQuestManagerRef () const { return questManagerRef; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
		int GetSelectedGoalIndex() const { return selectedGoalIndex; }

private:
	//Corresponding quest to this slot
	UPROPERTY(BlueprintReadOnly, Category = "References", Meta = (ExposeOnSpawn = true, AllowPrivateAccess = true))
		AQuest* assignedQuest;

	//Quest Manager Ref
	UPROPERTY(BlueprintReadOnly, Category = "References", Meta = (ExposeOnSpawn = true, AllowPrivateAccess = true))
		UQuestManager* questManagerRef;

	//List of subgoals for our quest
	UPROPERTY(BlueprintReadOnly, Category = "References", Meta = (AllowPrivateAccess = true))
		TArray<USubGoalWidget*> subGoalWidgets;

	//The selected subgoal for this quest slot. Only one can be selected for the entire questList
	UPROPERTY(BlueprintReadOnly, Category = "References", Meta = (AllowPrivateAccess = true))
		USubGoalWidget* selectedSubGoal;

	//Index of goal selected within this widget
	int selectedGoalIndex;
};
