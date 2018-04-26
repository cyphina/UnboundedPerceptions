// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "QuestJournal.h"
#include "QuestJournalEntry.h"
#include "UserInput.h"
#include "../QuestManager.h"
#include "BasePlayer.h"


void UQuestJournal::Construct_Implementation()
{
	Super::Construct_Implementation();
	if(questManagerRef)
	{
		questManagerRef->questJournalRef = this;
		questManagerRef->partyLeader = CPC->GetBasePlayer()->heroes[0];
	}
}

bool UQuestJournal::OnWidgetAddToViewport_Implementation()
{
	questManagerRef->partyLeader = CPC->GetBasePlayer()->heroes[0];
	return true;
}

void UQuestJournal::OnQuestEntryClicked(AQuest* quest, UQuestJournalEntry* questButton)
{
	if (currentQuestWidget)
		currentQuestWidget->ToggleButtonEnabled(true);

	if (IsValid(quest) && IsValid(questButton))
	{
		selectedQuest = quest;
		currentQuestWidget = questButton;
		questButton->ToggleButtonEnabled(false);
	}
	else //reset everything
	{
		selectedQuest = nullptr;
		currentQuestWidget = nullptr;
	}

	UpdateDetailWindow();
}

UQuestJournalEntry* UQuestJournal::GetQuestJournalEntry(AQuest* quest)
{
	auto pred = [&](UQuestJournalEntry* questEntry) { return questEntry->assignedQuest == quest; };
	int index = questJournalEntries.IndexOfByPredicate(pred);
	if (index != INDEX_NONE)
		return questJournalEntries[questJournalEntries.IndexOfByPredicate(pred)];
	return nullptr;
}


