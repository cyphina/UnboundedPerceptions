// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "QuestList.h"
#include "QuestListSlot.h"
#include "../QuestManager.h"
#include "UserInput.h"
#include "BasePlayer.h"

void UQuestList::Construct_Implementation()
{
	Super::Construct_Implementation();
	if(questManager)
		questManager->questListRef = this;
}

bool UQuestList::OnWidgetAddToViewport_Implementation()
{
	questManager->partyLeader = CPC->GetBasePlayer()->heroes[0];
	return true;
}

UQuestListSlot* UQuestList::GetQuestListSlot(AQuest* quest) const
{
	auto pred = [&](UQuestListSlot* questSlot) { return questSlot->GetAssignedQuest() == quest; };
	int index = questListSlots.IndexOfByPredicate(pred);
	if(index != INDEX_NONE)
		return questListSlots[index];
	return nullptr;
}


