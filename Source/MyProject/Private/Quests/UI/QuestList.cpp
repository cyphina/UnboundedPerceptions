// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "QuestList.h"
#include "QuestListSlot.h"
#include "RTSGameMode.h"
#include "UserInput.h"
#include "../QuestManager.h"

void UQuestList::NativeConstruct()
{
   Super::NativeConstruct();
   gameModeRef = CPC->GetGameMode();
   if (gameModeRef->GetQuestManager()) gameModeRef->GetQuestManager()->questListRef = this;
}

bool UQuestList::OnWidgetAddToViewport_Implementation()
{
   return true;
}

UQuestListSlot* UQuestList::GetQuestListSlot(AQuest* quest) const
{
   auto pred  = [&](UQuestListSlot* questSlot) { return questSlot->GetAssignedQuest() == quest; };
   int  index = questListSlots.IndexOfByPredicate(pred);
   if (index != INDEX_NONE) return questListSlots[index];
   return nullptr;
}
