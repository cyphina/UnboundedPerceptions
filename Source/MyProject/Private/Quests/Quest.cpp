#include "MyProject.h"
#include "Quest.h"
#include "RTSGameMode.h"
#include "UserInput.h"
#include "QuestManager.h"
#include "Transform.h"
#include "UpFunctionLibrary.h"
#include "EventSystem/Trigger.h"
#include "UI/QuestJournal.h"
#include "UI/QuestListSlot.h"
#include "UI/UserWidgets/RTSIngameWidget.h"
#include "WorldObjects/BaseHero.h"

void AQuest::BeginPlay()
{
   Super::BeginPlay();
}

void AQuest::CompleteQuest_Implementation()
{
   currentGoalIndices.Empty();
   currentState = EQuestState::completedQuests;

   if(ARTSGameMode* gameModeRef = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode()))
   {
      gameModeRef->questManager->OnQuestCompletedDelegate.Broadcast(this);
   }

   // UUpFunctionLibrary::ActivateTrigger(onQuestCompletedTrigger, this);
}

TArray<UUpGoal*> AQuest::GetCurrentGoals() const
{
   TArray<UUpGoal*>                                                   currentGoals;
   Algo::Transform(currentGoalIndices, currentGoals, [this](const int goalIndex) { return questInfo.GetSubgoals()[goalIndex]; });
   return currentGoals;
}

void AQuest::FindInitialItemAmount(int goalIndex)
{
   int            itemCount     = 0;
   UUpGatherGoal* gatheringGoal = Cast<UUpGatherGoal>(questInfo.GetSubgoals()[goalIndex]);
   const int      itemID        = gatheringGoal->GetItemToGatherId();

   if(AUserInput* controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()))
   {
      for(ABaseHero* hero : controllerRef->GetBasePlayer()->GetHeroes())
      {
         itemCount += hero->GetBackpack().FindItemCount(itemID);
      }
   }
   gatheringGoal->SetNumCurrentlyGathered(itemCount);
}

void AQuest::SetupUnlockedGoals(int completedGoalIndex, ARTSGameMode* gameModeRef)
{
   int index = 0;
   for(UUpGoal* goal : questInfo.GetSubgoals())
   {
      if(goal->GetGoalState() == EGoalState::lockedGoal)
      {
         const int numRemoved = goal->ClearPrereqGoal(completedGoalIndex);

         if(numRemoved && !goal->GetRequiredSubgoalIndices().Num())
         {
            currentGoalIndices.Add(index);
            goal->SetGoalState(EGoalState::currentGoal);

            if(Cast<UUpGatherGoal>(goal))
            {
               FindInitialItemAmount(index);
            }

            gameModeRef->GetQuestManager()->OnSubgoalUnlocked().Broadcast(this, index);
            
            for(FTriggerData& finishedTriggerActivation : goal->prevGoalTriggers)
            {
               gameModeRef->GetTriggerManager()->ActivateTrigger(finishedTriggerActivation);
            }
         }
      }
      ++index;
   }
}

void AQuest::CompleteSubGoal(int goalIndex)
{
   ARTSGameMode* gameModeRef = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode());
   if(!gameModeRef)
   {
      return;
   }

   if(currentGoalIndices.Contains(goalIndex))
   {
      currentGoalIndices.Remove(goalIndex);
      completedGoalIndices.Add(goalIndex);

      UUpGoal* completedGoal = questInfo.GetSubgoals()[goalIndex];

      if(completedGoal->GetGoalInfo().ShouldUpdateDescription())
      {
         currentDescription = FText::Format(NSLOCTEXT("Quest", "Description", "{0}\n\n{1}"), currentDescription, completedGoal->GetGoalInfo().updatedDescription);
         URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Quest", "DetailsUpdated", "Quest Journal Updated!"));
      } else
      {
         URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Quest", "GoalCompleted", "Quest Goal Completed!"));
      }

      completedGoal->SetGoalState(EGoalState::completedGoal);

      gameModeRef->GetQuestManager()->OnSubgoalCompleted().Broadcast(this, goalIndex);

      SetupUnlockedGoals(goalIndex, gameModeRef);

      for(FTriggerData& finishedTriggerActivation : completedGoal->afterGoalTriggers)
      {
         gameModeRef->GetTriggerManager()->ActivateTrigger(finishedTriggerActivation);
      }
      
      if(completedGoal->GetGoalInfo().canCompleteQuest || currentGoalIndices.Num() == 0)
      {
         CompleteQuest();
         return;
      }
   }
}

void AQuest::SetupStartingGoals()
{
   currentGoalIndices.Empty();
   for(int i = 0; i < questInfo.GetSubgoals().Num(); ++i)
   {
      UUpGoal* goal = questInfo.GetSubgoals()[i];
      if(goal->GetRequiredSubgoalIndices().Num() == 0)
      {
         currentGoalIndices.Add(i);
         goal->SetGoalState(EGoalState::currentGoal);

         if(Cast<UUpGatherGoal>(goal))
         {
            FindInitialItemAmount(i);
         }

         for(FTriggerData& finishedTriggerActivation : goal->prevGoalTriggers)
         {
            UUpFunctionLibrary::ActivateTrigger(finishedTriggerActivation, this);
         }
      } else
      {
         goal->SetGoalState(EGoalState::lockedGoal);
      }
   }

   currentDescription = questInfo.desc;
}

UUpGoal* AQuest::GetGoalAtIndex(int goalIndex)
{
   return questInfo.GetSubgoals()[goalIndex];
}