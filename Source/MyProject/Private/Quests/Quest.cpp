// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Quest.h"
#include "UserInput.h"
#include "UI/HUDManager.h"
#include "QuestManager.h"
#include "UI/QuestJournal.h"
#include "UI/QuestList.h"
#include "UI/QuestListSlot.h"

FVector const FGoalInfo::invalidGoalLocation = FVector(-66666, -66666, -66666);

void AQuest::BeginPlay()
{
	int num = 0;
	for(FGoalInfo goal : questInfo.subgoals)
	{
		if ((goal.goalType == EGoalType::Hunt || goal.goalType == EGoalType::Interact) && goal.amountToHunt > 0)
		{
			currentHuntedAmounts.Add(num,0); 	
		}
		++num;
	}
}

void AQuest::CompleteQuest_Implementation(bool fail)
{
	currentGoalIndices.Empty();
	//currentHuntedAmounts.Empty();
	currentGoals.Empty();
	if (fail)
		currentState = EQuestState::failedQuests;
	currentState = EQuestState::completedQuests;
	questManagerRef->EndQuest(this);
}

void AQuest::UpdateSubGoals()
{
	currentGoals.Empty();
	for(int i : currentGoalIndices)
	{
		currentGoals.Add(questInfo.subgoals[i]);
	}
}

void AQuest::CompleteSubGoal(int goalIndex, bool fail)
{
	if(currentGoalIndices.Contains(goalIndex))
	{
		FGoalInfo completedGoal = questInfo.subgoals[goalIndex];
		UQuestListSlot* questListSlot = questManagerRef->questListRef->GetQuestListSlot(this);

		if(completedGoals[completedGoals.Add(completedGoal)].shouldUpdateQuestDescription && fail)
		{
			currentDescription = FText::Format(NSLOCTEXT("Quest", "Description", "{0}{1}"), currentDescription, completedGoal.updatedDescription);
			if (questManagerRef->questJournalRef->GetSelectedQuest() == this)
				questManagerRef->questJournalRef->UpdateDetailWindow();
		}
		
		currentGoals.RemoveSingle(questInfo.subgoals[goalIndex]);
		questListSlot->RemoveSubGoalWidget(goalIndex);
		currentGoalIndices.Remove(goalIndex);

		//handle failure or completion
		if (completedGoal.canFailQuest || completedGoal.canCompleteQuest)
		{
			CompleteQuest(fail);
			return;
		}

		//else handle adding new subgoals
		for(int followingSubGoalIndex : completedGoal.followingSubGoalIndices)
		{
			currentGoalIndices.Add(followingSubGoalIndex);
			currentGoals.Add(questInfo.subgoals[followingSubGoalIndex]);
			questManagerRef->questListRef->GetQuestListSlot(this)->AddSubGoalWidget(this, followingSubGoalIndex);
		}

		if(questManagerRef->controllerRef->GetHUDManager()->GetQuestJournal()->GetSelectedQuest() == this) //if our quest selected in quest journal
		{
			questManagerRef->questJournalRef->UpdateDetailWindow();
			if(this == questManagerRef->questListRef->currentlySelectedQuest && questManagerRef->questListRef->GetQuestListSlot(this)->GetSelectedGoalIndex() == goalIndex) //if the subgoal that was finished was selected
			{
				questManagerRef->questListRef->GetQuestListSlot(this)->SelectSubGoal(0);
			}
		}
	}
}

void AQuest::SetupStartingGoals()
{
	currentGoalIndices.Empty();
	currentGoalIndices = startingGoalIndices;
	UpdateSubGoals();
	currentDescription = questInfo.desc;
}


FGoalInfo AQuest::GetGoalAtIndex(int goalIndex)
{
	return currentGoals[goalIndex];
}
