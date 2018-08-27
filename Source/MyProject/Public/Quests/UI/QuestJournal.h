// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyDraggableWidget.h"
#include "../Quest.h"
#include "QuestJournal.generated.h"

/* Class for a journal widget that displays more detailed information on quests */

class ABaseHero;
class ARTSGameMode;
class UQuestJournalEntry;

UCLASS()
class MYPROJECT_API UQuestJournal : public UMyDraggableWidget
{
	GENERATED_BODY()

public:

	void						NativeConstruct() override;
	bool						OnWidgetAddToViewport_Implementation() override;

	/** update all details of quest information window*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Quest Journal Updates")
	void						UpdateDetailWindow(); 

	/**add a new entry to the quest journal*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Quest Journal Updates")
	void						AddEntry(AQuest* quest); 

	/**when one of the quests in the journal is selected*/
	UFUNCTION(BlueprintCallable, Category = "Quest Journal Interface")
	void						OnQuestEntryClicked(AQuest* quest, UQuestJournalEntry* questButton); 

	/**remove a quest from the journal*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Quest Journal Interface")
	void						RemoveFromQuestJournal(AQuest* quest); 

	/** Change the color of the suggested level text based on if it's higher or lower level than our party leader*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Quest Journal Interface")
	void						UpdateSuggestedLevelColor(); 

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
	AQuest*						GetSelectedQuest() const { return selectedQuest; } 

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
	UQuestJournalEntry*			GetQuestJournalEntry(AQuest* quest);

	UPROPERTY(BlueprintReadWrite, Category = "References")
	UQuestJournalEntry*			currentQuestWidget;

	UPROPERTY(BlueprintReadWrite, category = "Data")
	TArray<UQuestJournalEntry*> questJournalEntries;
private:

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true), Category = "References")
	ARTSGameMode*				gameModeRef;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true ), Category = "References")
	AQuest*						selectedQuest;


};
