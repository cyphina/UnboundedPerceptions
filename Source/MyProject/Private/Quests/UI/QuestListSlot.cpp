#include "MyProject.h"
#include "QuestListSlot.h"
#include "QuestListSlotGoalEntry.h"
#include "TextBlock.h"
#include "../Quest.h"

void UQuestListSlot::NativeOnInitialized()
{
   Super::NativeOnInitialized();

   if(GetAssignedQuest())
   {
      const FQuestInfo& questInfo = GetAssignedQuest()->GetQuestInfo();
      Text_QuestName->SetText(GetAssignedQuest()->GetQuestInfo().name);

      FLinearColor questTitleColor;
      switch(GetAssignedQuest()->GetQuestInfo().questCategory)
      {
         case EQuestCategory::Event: questTitleColor = FLinearColor(0.63, 0.18, 0.57, 1);
            break;
         case EQuestCategory::MainQuest: questTitleColor = FLinearColor(0.20, 0.48, 0.57, 1);
            break;
         case EQuestCategory::SideQuest: questTitleColor = FLinearColor(0.30, 0.58, 0.29, 1);
            break;
      }

      Text_QuestName->SetColorAndOpacity(questTitleColor);
   }
}

void UQuestListSlot::RemoveSubGoalWidget(int goalIndex)
{
   UQuestListSlotGoalEntry* goalEntryToRemove = GetGoalEntryWithGoalIndex(goalIndex);

   if(goalEntryToRemove)
   {
      goalEntryToRemove->RemoveFromParent();
      if(selectedGoalIndex == goalIndex)
      {
         selectedGoalIndex = INDEX_NONE;
      }
   }
}

void UQuestListSlot::OnSubgoalSelected(AQuest* quest, int goalIndex)
{
   if(GetAssignedQuest() == quest)
   {
      if(selectedGoalIndex != INDEX_NONE)
      {
         UQuestListSlotGoalEntry* selectedSubGoal = GetGoalEntryWithGoalIndex(selectedGoalIndex);
         if(selectedSubGoal)
         {
            selectedSubGoal->ToggleEnabled(false);
         }
      }
      selectedGoalIndex = goalIndex;
   }
}

void UQuestListSlot::OnSubgoalCompleted(AQuest* quest, int goalIndex)
{
   if(quest == GetAssignedQuest())
   {
      RemoveSubGoalWidget(goalIndex);
   }
}

UQuestListSlotGoalEntry* UQuestListSlot::GetGoalEntryWithGoalIndex(int goalIndexToSearchFor) const
{
   const auto pred  = [goalIndexToSearchFor, this](UQuestListSlotGoalEntry* goalEntry)
   {
      // Since we only store the goal inside the quest list slot, we have to find what it's goal index is inside the list of subgoals
      const int slotGoalIndex = goalEntry->GetCorrespondingGoalIndex(GetAssignedQuest());
      // Then we try to match it with the index we're looking for
      return slotGoalIndex == goalIndexToSearchFor;
   };
   
   const int index = subgoalWidgets.IndexOfByPredicate(pred);
   
   if(index != INDEX_NONE)
   {
      return subgoalWidgets[index];
   }
   return nullptr;
}
