// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "QuestListSlot.h"
#include "QuestList.h"
#include "RTSGameMode.h"
#include "../QuestManager.h"
#include "SubGoalWidget.h"
#include "../Quest.h"

void UQuestListSlot::SelectSubGoal(int subGoalIndex)
{
   if (subGoalIndex >= 0 && subGoalIndex < subGoalWidgets.Num() && IsValid(subGoalWidgets[subGoalIndex])) {
      selectedSubGoal = subGoalWidgets[subGoalIndex];
      selectedSubGoal->ToggleEnabled(true);
      selectedGoalIndex = selectedSubGoal->GetGoalIndex();
      gameModeRef->GetQuestManager()->OnSwitchSubGoal();
   } else // if this index is not an index of a current subgoal of our quest (like if you forget to setup starting subgoals)
   {
      selectedSubGoal->ToggleEnabled(false);
      selectedSubGoal   = nullptr;
      selectedGoalIndex = -1;
   }
}

void UQuestListSlot::RemoveSubGoalWidget(int goalIndex)
{
   for (USubGoalWidget* subGoalWidg : subGoalWidgets) {
      if (subGoalWidg->GetGoalIndex() == goalIndex) {
         subGoalWidg->RemoveFromParent();
         return;
      }
   }
}

bool UQuestListSlot::IsCurrentQuest()
{
   if (assignedQuest == gameModeRef->GetQuestManager()->questListRef->currentlySelectedQuest) return true;
   return false;
}

UQuestManager* UQuestListSlot::GetQuestManagerRef() const
{
   return gameModeRef->GetQuestManager();
}
