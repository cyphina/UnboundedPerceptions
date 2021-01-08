// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Quest.h"

#include "RTSGameMode.h"
#include "UserInput.h"

#include "UI/HUDManager.h"
#include "QuestManager.h"
#include "Transform.h"
#include "EventSystem/Trigger.h"

#include "UI/QuestJournal.h"
#include "UI/QuestList.h"
#include "UI/QuestListSlot.h"
#include "UI/UserWidgets/RTSIngameWidget.h"

#include "Items/ItemManager.h"
#include "WorldObjects/BaseHero.h"
#include "Interactables/NamedInteractableDecorator.h"

FVector const FGoalInfo::invalidGoalLocation = FVector(-66666, -66666, -66666);

void AQuest::BeginPlay()
{
   SetupGoalAmounts();
}

void AQuest::SetupGoalAmounts()
{
   // Here we setup goal specific data. We setup any count tracking information and information about what things we've already interacted with so we don't complete that goal more than once and
   // then we store that information so that we can look it up by goal index
   int num = 0;
   for(FGoalInfo goal : questInfo.subgoals) {
      if(goal.amount > 1) {
         currentAmounts.Add(num, 0);
         if(goal.goalType == EGoalType::Interact)
            // Used when the goal requires us to interact with multiple copies of the same named object. We use the address of the decorator to check for uniqueness
            interactedActors.Add(num, TArray<const UNamedInteractableDecorator*>());
      }
      ++num;
   }
}

void AQuest::CompleteQuest_Implementation(bool fail)
{
   currentGoalIndices.Empty();
   // currentHuntedAmounts.Empty();
   if (fail) currentState = EQuestState::failedQuests;
   currentState = EQuestState::completedQuests;
   questManagerRef->EndQuest(this);
}

TArray<FGoalInfo> AQuest::GetCurrentGoals() const
{
   TArray<FGoalInfo> currentGoals;
   Algo::Transform(currentGoalIndices, currentGoals, [this](const int goalIndex) { return questInfo.subgoals[goalIndex]; });
   return currentGoals;
}

void AQuest::FindInitialItemAmount(int goalIndex)
{
   // Figure out how much of this item we have so far
   int itemCount = 0;
   const int itemID    = FCString::Atoi(*questInfo.subgoals[goalIndex].additionalNames[0].ToString());

   for (ABaseHero* hero : questManagerRef->controllerRef->GetBasePlayer()->GetHeroes()) {
      itemCount += hero->GetBackpack().FindItemCount(itemID);
   }
   currentAmounts[goalIndex] = itemCount;
}

void AQuest::CompleteSubGoal(int goalIndex, bool fail)
{
   if (currentGoalIndices.Contains(goalIndex)) {
      FGoalInfo&      completedGoal = questInfo.subgoals[goalIndex];
      UQuestListSlot* questListSlot = questManagerRef->questListRef->GetQuestListSlot(this);

      //! Remove traces of this goal in our goal bookkeeping (REMOVE IT BEFORE YOU CHANGE THE GOALSTATE SINCE EQUALITY OPERATOR RELIES ON IT
      questListSlot->RemoveSubGoalWidget(goalIndex);
      currentGoalIndices.Remove(goalIndex);

      // If this goal should update the description, and we didn't fail it...
      if (!fail) {
         if (completedGoals[completedGoals.Add(completedGoal)].shouldUpdateQuestDescription) {
            // Add on the extra description at the end.
            currentDescription = FText::Format(NSLOCTEXT("Quest", "Description", "{0}\n\n{1}"), currentDescription, completedGoal.updatedDescription);
            // If this quest is the one selected in the quest journal, update the journal
            if (questManagerRef->questJournalRef->GetSelectedQuest() == this) questManagerRef->questJournalRef->UpdateDetailWindow();
            URTSIngameWidget::NativeDisplayHelpText(questManagerRef->GetWorld(), NSLOCTEXT("Quest", "DetailsUpdated", "Quest Journal Updated!"));
         } else {
            URTSIngameWidget::NativeDisplayHelpText(questManagerRef->GetWorld(), NSLOCTEXT("Quest", "GoalCompleted", "Quest Goal Completed!"));
         }
         completedGoal.goalState = EGoalState::completedGoal;
      } else
         completedGoal.goalState = EGoalState::failedGoal;

      for (FTriggerData& finishedTriggerActivation : completedGoal.afterGoalTriggers) {
         questManagerRef->controllerRef->GetGameMode()->GetTriggerManager()->ActivateTrigger(finishedTriggerActivation);
      }

      // Add any subgoals that require us to have completed this goal
      int index = 0;

      for (FGoalInfo& goal : questInfo.subgoals) {
         if (goal.goalState == EGoalState::lockedGoal) {
            // Attempt to remove this goal from any goals that required this goal as a prereq.  
            const int numRemoved = goal.requiredSubGoalIndices.Remove(goalIndex);
            // If a goal was removed and there are no more required goals, then this is OUR NEW GOAL~!
            if (numRemoved && !goal.requiredSubGoalIndices.Num()) { 
               currentGoalIndices.Add(index);
               goal.goalState = EGoalState::currentGoal;
               questManagerRef->questListRef->GetQuestListSlot(this)->AddSubGoalWidget(this, index);

               if (goal.goalType == EGoalType::Find) FindInitialItemAmount(index);

               for (FTriggerData& finishedTriggerActivation : goal.prevGoalTriggers) {
                  questManagerRef->controllerRef->GetGameMode()->GetTriggerManager()->ActivateTrigger(finishedTriggerActivation);
               }
            }
         }
         ++index;
      }

      // handle failure or completion
      if (completedGoal.canFailQuest || completedGoal.canCompleteQuest || currentGoalIndices.Num() == 0) {
         CompleteQuest(fail);
         return;
      }

      if (questManagerRef->controllerRef->GetWidgetProvider()->GetIngameHUD()->GetQuestJournal()->GetSelectedQuest() == this) // if our quest selected in quest journal
      {
         questManagerRef->questJournalRef->UpdateDetailWindow();
         if (this == questManagerRef->questListRef->currentlySelectedQuest &&
             questManagerRef->questListRef->GetQuestListSlot(this)->GetSelectedGoalIndex() == goalIndex) // if the subgoal that was finished was selected
         {
            questManagerRef->questListRef->GetQuestListSlot(this)->SelectSubGoal(0);
         }
      }
   }
}

void AQuest::SetupStartingGoals()
{
   currentGoalIndices.Empty();
   for (int i = 0; i < questInfo.subgoals.Num(); ++i) {
      FGoalInfo& goal = questInfo.subgoals[i];
      if (goal.requiredSubGoalIndices.Num() == 0) {
         currentGoalIndices.Add(i);
         goal.goalState = EGoalState::currentGoal;

         if (goal.goalType == EGoalType::Find) FindInitialItemAmount(i);

         for (FTriggerData& finishedTriggerActivation : questInfo.subgoals[i].prevGoalTriggers) {
            questManagerRef->controllerRef->GetGameMode()->GetTriggerManager()->ActivateTrigger(finishedTriggerActivation);
         }
      } else
         goal.goalState = EGoalState::lockedGoal;
   }

   currentDescription = questInfo.desc;
}

FGoalInfo AQuest::GetGoalAtIndex(int goalIndex)
{
   return questInfo.subgoals[goalIndex];
}

FText AQuest::MakeGoalText(AQuest* assignedQuest, FGoalInfo goalInfo, int goalIndex)
{
   if (!goalInfo.isCustomGoal) {
      FFormatOrderedArguments args;
      checkf(goalInfo.additionalNames.Num() > 0, TEXT("%s"), *goalInfo.goalText.ToString());
      switch (goalInfo.goalType) {
         case EGoalType::Hunt:
            args.Add(goalInfo.additionalNames[0]);
            if (goalInfo.amount > 1) {
               args.Add(NSLOCTEXT("Quests", "Suffix", "s"));
               args.Add(assignedQuest->currentAmounts[goalIndex]);
               args.Add(goalInfo.amount);
               return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Hunt {0}{1}: {2}/{3}"), args);
            }
            return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Hunt a {0}"), args);
         case EGoalType::Find:
            args.Add(UItemManager::Get().GetItemInfo(FCString::Atoi(*goalInfo.additionalNames[0].ToString()))->name);
            if (goalInfo.additionalNames.Num() > 1) {
               if (goalInfo.amount > 1) {
                  args.Add(NSLOCTEXT("Quests", "Suffix", "s"));
                  args.Add(assignedQuest->currentAmounts[goalIndex]);
                  args.Add(goalInfo.amount);

                  args.Add(goalInfo.additionalNames[1]);
                  return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Bring: {0}{1} {2}/{3} to {4}!"), args);
               }
               args.Add(goalInfo.additionalNames[1]);
               return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Bring a {0} to {1}"), args);
            }

            args.Add(goalInfo.amount - assignedQuest->currentAmounts[goalIndex]);
            return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Bring {1} {0}(s)"), args);

         case EGoalType::Talk:
            args.Add(goalInfo.additionalNames[0]);
            if (goalInfo.additionalNames.Num() > 1) {
               args.Add(goalInfo.additionalNames[1]);
               return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Talk to {0} about {1}"), args);
            }
            return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Talk to {0}"), args);
         case EGoalType::Interact:
            args.Add(goalInfo.additionalNames[0]);
            if (goalInfo.amount > 1) {
               args.Add(NSLOCTEXT("Quests", "Suffix", "s"));
               args.Add(assignedQuest->currentAmounts[goalIndex]);
               args.Add(goalInfo.amount);
               return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Interact with {0}{1}: {2}/{3}"), args);
            }
            return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Interact with the {0}"), args);
         case EGoalType::Custom: return goalInfo.goalText;
         default: return FText();
      }
   } else
      return goalInfo.goalText;
}