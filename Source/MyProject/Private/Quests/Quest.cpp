// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Quest.h"

#include "RTSGameMode.h"
#include "UserInput.h"

#include "UI/HUDManager.h"
#include "QuestManager.h"
#include "EventSystem/Trigger.h"

#include "UI/QuestJournal.h"
#include "UI/QuestList.h"
#include "UI/QuestListSlot.h"

FVector const FGoalInfo::invalidGoalLocation = FVector(-66666, -66666, -66666);

void AQuest::BeginPlay()
{
	int num = 0;
	for(FGoalInfo goal : questInfo.subgoals)
	{
		if (goal.amount > 0)
		{
			currentAmounts.Add(num,0); 	
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

		//If this goal should update the description, and we didn't fail it...
		if(completedGoals[completedGoals.Add(completedGoal)].shouldUpdateQuestDescription && !fail)
		{
			//Add on the extra description at the end.  
			currentDescription = FText::Format(NSLOCTEXT("Quest", "Description", "{0}\n\n{1}"), currentDescription, completedGoal.updatedDescription);
			if (questManagerRef->questJournalRef->GetSelectedQuest() == this)
				questManagerRef->questJournalRef->UpdateDetailWindow();
		}
		
		currentGoals.RemoveSingle(questInfo.subgoals[goalIndex]);
		questListSlot->RemoveSubGoalWidget(goalIndex);
		currentGoalIndices.Remove(goalIndex);

		for(FTriggerData& trigger : completedGoal.goalTriggers)
		{
			questManagerRef->controllerRef->GetGameMode()->GetTriggerManager()->ActivateTrigger(trigger);	
		}

		//handle failure or completion
		if (completedGoal.canFailQuest || completedGoal.canCompleteQuest)
		{
			CompleteQuest(fail);
			return;
		}

		//Add any subgoals which don't have any requiredSubGoals
		int index = 0;
		for(FGoalInfo goal : questInfo.subgoals)
		{
			if (goal.goalState == EGoalState::lockedGoal)
			{
				int foundGoalReqIndex = goal.requiredSubGoalIndices.Find(goalIndex);
				if (foundGoalReqIndex != INDEX_NONE)
				{
					goal.requiredSubGoalIndices.RemoveAt(foundGoalReqIndex);
					if (!goal.requiredSubGoalIndices.Num())
					{
						currentGoals.Add(goal);
						currentGoalIndices.Add(index);
						questManagerRef->questListRef->GetQuestListSlot(this)->AddSubGoalWidget(this, index);
					}
				}
			}
			++index;
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
	for(int i = 0; i < questInfo.subgoals.Num(); ++i)
	{
		if(questInfo.subgoals[i].requiredSubGoalIndices.Num() == 0)
		{
			currentGoalIndices.Add(i);
			questInfo.subgoals[i].goalState = EGoalState::currentGoal;
		}
	}

	UpdateSubGoals();
	currentDescription = questInfo.desc;
}

FGoalInfo AQuest::GetGoalAtIndex(int goalIndex)
{
	return currentGoals[goalIndex];
}


FText AQuest::MakeGoalText(AQuest* assignedQuest, FGoalInfo goalInfo, int goalIndex)
{
	FFormatOrderedArguments args;
	args.Add(goalInfo.additionalNames[0]);
	checkf(goalInfo.additionalNames.Num() > 0, TEXT("%s"), *goalInfo.goalText.ToString())
	if (!goalInfo.isCustomGoal)
	{
		switch (goalInfo.goalType)
		{
		case EGoalType::Hunt:
			if (goalInfo.amount > 1)
				args.Add(NSLOCTEXT("Quests", "Suffix", "s"));
			else
				args.Add(FText());
			args.Add(assignedQuest->currentAmounts[goalIndex]);
			args.Add(goalInfo.amount);
			return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Hunt {0}{1}: {2}/{3}"), args);
		case EGoalType::Find:
			args.Add(assignedQuest->currentAmounts[goalIndex]);
			args.Add(goalInfo.amount);
			if (goalInfo.amount > 1)
				args.Add(NSLOCTEXT("Quests", "Suffix", "s"));
			else
				args.Add(FText());
			return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Find {1}/{2} {0}{3}!"), args);
		case EGoalType::Talk:
			if (goalInfo.additionalNames.Num() > 1)
			{
				args.Add(goalInfo.additionalNames[1]);
				return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Talk to {0} about {1}"), args);
			}

			return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Talk to {0}"), args);
		case EGoalType::Interact:
			if (goalInfo.amount > 1)
				args.Add(NSLOCTEXT("Quests", "Suffix", "s"));
			else
				args.Add(FText());
			args.Add(assignedQuest->currentAmounts[goalIndex]);
			args.Add(goalInfo.amount);
			return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Interact with {0}{1}: {2}/{3}"), args);
		case EGoalType::Custom:
			return goalInfo.goalText;
		default:
			return FText();
		}
	}
	else
		return goalInfo.goalText;
}