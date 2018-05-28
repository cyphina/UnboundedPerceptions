// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UserWidget.h"
#include "BasePlayer.h"
#include "UI/HUDManager.h"
#include "UI/Minimap.h"
#include "WorldObjects/BaseHero.h"
#include "QuestManager.h"
#include "UI/QuestList.h"
#include "UI/QuestListSlot.h"
#include "UI/QuestJournal.h"
#include "Quest.h"
#include "WorldObjects/Enemies/Enemy.h"
#include "WorldObjects/NPC.h"
#include "UserInput.h"
#include "GoalActor.h"

void UQuestManager::SelectNewQuest(AQuest* quest)
{
	if(IsValid(quest))
	{
		questListRef->currentlySelectedQuest = quest;
		questListRef->GetQuestListSlot(questListRef->currentlySelectedQuest)->OnQuestSelected(); //this goes first or else OnQuestSelected can't remove the old quest slots selected goal
	}
}

void UQuestManager::AddNewQuest(TSubclassOf<AQuest> questClassToSpawn, bool forcedStart)
{
	auto condition = [questClassToSpawn](AQuest* quest) { return questClassToSpawn == quest->GetClass(); };
	if(IsValid(questClassToSpawn) && !quests.ContainsByPredicate(condition))
	{
		AQuest* quest = questListRef->GetWorld()->SpawnActorDeferred<AQuest>(questClassToSpawn.Get(), FTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
		quest->SetQuestManagerRef(this);
		UGameplayStatics::FinishSpawningActor(quest, FTransform());
		quests.Add(quest); 
		quest->SetupStartingGoals();
		questListRef->AddToQuestList(quest);
		questListRef->GetQuestListSlot(quest)->UpdateQuestEntry();
		questJournalRef->AddEntry(quest);
		if(forcedStart)
		{
			SelectNewQuest(quest);
			questListRef->GetQuestListSlot(questListRef->currentlySelectedQuest)->SelectSubGoal(0);
		}
		if(questListRef->GetVisibility() == ESlateVisibility::Hidden)
		{
			controllerRef->GetHUDManager()->AddHUD(static_cast<int>(HUDs::HS_QuestList));
		}
	}
}

void UQuestManager::OnSwitchSubGoal()
{
	if(questListRef->currentlySelectedQuest)
	{
		if(currentGoalActor)
		{
			currentGoalActor->Destroy();
		}
	}

	if(questListRef->currentlySelectedQuest->questInfo.subgoals[questListRef->GetQuestListSlot(questListRef->currentlySelectedQuest)->GetSelectedGoalIndex()].goalLocation != FGoalInfo::invalidGoalLocation)
	{
		currentGoalActor = controllerRef->GetWorld()->SpawnActorAbsolute<AGoalActor>(goalActorClass, FTransform(questListRef->currentlySelectedQuest->questInfo.subgoals[questListRef->GetQuestListSlot(questListRef->currentlySelectedQuest)->GetSelectedGoalIndex()].goalLocation), FActorSpawnParameters());
	}

	currentDistance = GetDistanceToGoal();
	questListRef->SetDistanceText(currentDistance);
	controllerRef->GetHUDManager()->GetMinimap()->UpdateDirectionArrow(FMath::RadiansToDegrees(FMath::Acos(FVector::ForwardVector.CosineAngle2D(partyLeader->GetActorLocation() - currentGoalActor->GetActorLocation()))));
	questListRef->ToggleDistanceIndicatorVisibility(true);
	if (currentDistance > 10)
		controllerRef->GetHUDManager()->GetMinimap()->ToggleDirectionArrowVisibility(true);
	else
		controllerRef->GetHUDManager()->GetMinimap()->ToggleDirectionArrowVisibility(false);
}

int UQuestManager::GetDistanceToGoal()
{
	FVector partyLeaderLocationXY = FVector(partyLeader->GetActorLocation().X, partyLeader->GetActorLocation().Y, 0);
	FVector goalActorLocationXY = FVector(currentGoalActor->GetActorLocation().X, currentGoalActor->GetActorLocation().Y, 0);
	return FMath::RoundToInt((partyLeaderLocationXY - goalActorLocationXY).Size2D() / 100);
}

void UQuestManager::EndQuest(AQuest* questToEnd)
{
	quests.Remove(questToEnd);
	switch(questToEnd->GetQuestState())
	{
		case EQuestState::currentQuests: quests.Add(questToEnd);
		case EQuestState::failedQuests: quests.Add(questToEnd);
		case EQuestState::completedQuests: quests.Add(questToEnd);
	}

	questJournalRef->AddEntry(questToEnd); //add our quest to the quest journal under its new category

	if (questListRef->GetQuestListSlot(questToEnd)) //remove from quest list if its in quest list
		questListRef->RemoveFromQuestList(questToEnd);

	if(questToEnd == questListRef->currentlySelectedQuest) //if it is selected in the questlistwidget
	{
		questListRef->currentlySelectedQuest = nullptr;
		
		if(IsValid(currentGoalActor))
		{
			currentGoalActor->Destroy();
			questListRef->ToggleDistanceIndicatorVisibility(false);
			controllerRef->GetHUDManager()->GetMinimap()->ToggleDirectionArrowVisibility(false);
		}
	}

	//questJournalRef->RemoveFromQuestJournal(questToEnd);

	if (IsValid(quests[0]))
		SelectNewQuest(quests[0]);

	if(questJournalRef->GetSelectedQuest() == questToEnd)
		questJournalRef->OnQuestEntryClicked(nullptr, nullptr);

	if(questToEnd->GetQuestState() == EQuestState::completedQuests)
	{
		controllerRef->GetBasePlayer()->UpdateEXP(questToEnd->questInfo.questReward.exp);
		controllerRef->GetBasePlayer()->UpdateGold(questToEnd->questInfo.questReward.gold);
	}

}

void UQuestManager::CompleteGoals()
{
	for(int i : questListRef->currentlySelectedQuest->GetCurrentGoalIndices())
	{
		questListRef->currentlySelectedQuest->CompleteSubGoal(i, false);
	}
}

void UQuestManager::OnPartyLeaderMove()
{
	if(IsValid(currentGoalActor))
	{
		currentDistance = GetDistanceToGoal();
		questListRef->SetDistanceText(currentDistance);
		if(currentDistance < 10)
		{
			controllerRef->GetHUDManager()->GetMinimap()->ToggleDirectionArrowVisibility(false);
		}
		else
		{
			controllerRef->GetHUDManager()->GetMinimap()->UpdateDirectionArrow(FMath::RadiansToDegrees(FMath::Acos(FVector::ForwardVector.CosineAngle2D(partyLeader->GetActorLocation() - currentGoalActor->GetActorLocation()))));
			controllerRef->GetHUDManager()->GetMinimap()->ToggleDirectionArrowVisibility(true);
		}
	}
}

void UQuestManager::OnEnemyDie(TSubclassOf<AEnemy> enemyClass)
{
	int index = 0;
	for(AQuest* quest : quests)
	{
		index = 0;
		for (FGoalInfo goal : quest->currentGoals)
		{
			if (goal.goalType == EGoalType::Hunt)
			{
				if (enemyClass.GetDefaultObject()->GetGameName().EqualTo(goal.additionalName))
				{
					if (quest->currentHuntedAmounts[quest->GetCurrentGoalIndices()[index]] + 1 >= goal.amountToHunt)
					{
						++quest->currentHuntedAmounts[quest->GetCurrentGoalIndices()[index]];
						quest->CompleteSubGoal(quest->GetCurrentGoalIndices()[index], false);
					}
					else
					{
						++quest->currentHuntedAmounts[quest->GetCurrentGoalIndices()[index]];
						questListRef->GetQuestListSlot(quest)->UpdateQuestEntry();
					}

				}	
			}
			++index;
		}

		if (quest == questJournalRef->GetSelectedQuest())
		{
			questJournalRef->UpdateDetailWindow();
		}
	}
}

void UQuestManager::OnTalkNPC(TSubclassOf<ANPC> NPCClass)
{
	int goalIndex = 0;
	for (AQuest* quest : quests)
	{
		for (FGoalInfo goal : quest->currentGoals)
		{
			if(goal.goalType == EGoalType::Talk && NPCClass.GetDefaultObject()->GetGameName().EqualTo(goal.additionalName))
			{
				quest->CompleteSubGoal(quest->GetCurrentGoalIndices()[goalIndex], false);
				if (goal.canCompleteQuest)
				{
					quest->CompleteQuest(false);
				}
			}

			if (quest == questJournalRef->GetSelectedQuest())
			{
				questJournalRef->UpdateDetailWindow();
			}
			++goalIndex;
		}
	}
}



