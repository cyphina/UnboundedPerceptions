#include "MyProject.h"
#include "QuestList.h"
#include "Quest.h"
#include "QuestListSlot.h"
#include "QuestManager.h"
#include "RTSGameMode.h"
#include "UserInput.h"

void UQuestList::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   // TODO: Fix this? - GetWorld()->GetTimerManager().SetTimer(updateDistanceIndicatorHandle, this, &UQuestList::UpdateDistanceToGoal, 0.1f, true, 0.f);
   OnSubgoalSwitched().AddUObject(this, &UQuestList::OnSubgoalSwitchedCallback);
   if(ARTSGameMode* gameModeRef = GetWorld()->GetAuthGameMode<ARTSGameMode>())
   {
      gameModeRef->GetQuestManager()->OnQuestStartedDelegate.AddDynamic(this, &UQuestList::OnQuestStarted);
      gameModeRef->GetQuestManager()->OnQuestCompletedDelegate.AddDynamic(this, &UQuestList::OnQuestCompleted);
      gameModeRef->GetQuestManager()->OnSubgoalCompleted().AddUObject(this, &UQuestList::OnSubgoalCompleted);
      gameModeRef->GetQuestManager()->OnSubgoalUnlocked().AddUObject(this, &UQuestList::OnSubgoalUnlocked);
   }
}

UQuestListSlot* UQuestList::GetQuestListSlot(AQuest* quest) const
{
   const auto pred  = [quest](UQuestListSlot* questSlot) { return questSlot->GetAssignedQuest() == quest; };
   const int  index = questListSlots.IndexOfByPredicate(pred);
   if (index != INDEX_NONE)
   {
      return questListSlots[index];
   }
   return nullptr;
}

void UQuestList::OnQuestCompleted(AQuest* questToEnd)
{
   if(GetQuestListSlot(questToEnd))
   {
      if(questToEnd == currentlySelectedQuest)
      {
         currentlySelectedQuest = nullptr;
      }
      RemoveFromQuestList(questToEnd);
   }
}

void UQuestList::OnQuestStarted(AQuest* questToStart)
{
   AddToQuestList(questToStart);
}

void UQuestList::UpdateDistanceToGoal()
{
   // TODO: Implement this
}

void UQuestList::OnSubgoalUnlocked(AQuest* quest, int unlockedGoalIndex)
{
   if(UQuestListSlot* unlockedGoalListSlot = GetQuestListSlot(quest))
   {
      unlockedGoalListSlot->AddSubGoalWidget(quest, unlockedGoalIndex);
   }
}

void UQuestList::OnSubgoalCompleted(AQuest* quest, int completedGoalIndex)
{
   if(UQuestListSlot* completedGoalListSlot = GetQuestListSlot(quest))
   {
      completedGoalListSlot->RemoveSubGoalWidget(completedGoalIndex);
      if(completedGoalListSlot->GetSelectedGoalIndex() == completedGoalIndex)
      {
         completedGoalListSlot->SetSelectedGoalIndex(INDEX_NONE);
      }
   }
}

void UQuestList::OnSubgoalSwitchedCallback(AQuest* quest, int goalIndex)
{
   if(quest->GetSubgoals()[goalIndex]->GetGoalStyleInfo().goalLocation != FGoalStyleInfo::invalidGoalLocation)
   {
      ToggleDistanceIndicatorVisibility(true);
   }

   GetQuestListSlot(quest)->OnSubgoalSelected(quest, goalIndex);
}
