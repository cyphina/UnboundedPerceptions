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
#include "UI/UserWidgets/MainWidget.h"

#include "Items/ItemManager.h"
#include "WorldObjects/BaseHero.h"
#include "Interactables/NamedInteractableDecorator.h"

FVector const FGoalInfo::invalidGoalLocation = FVector(-66666, -66666, -66666);

void AQuest::BeginPlay()
{
   int num = 0;
   for (FGoalInfo goal : questInfo.subgoals) {
      if (goal.amount > 1) {
         currentAmounts.Add(num, 0);
         if (goal.goalType == EGoalType::Interact) interactedActors.Add(num, TArray<UNamedInteractableDecorator*>());
      }
      ++num;
   }
}

void AQuest::CompleteQuest_Implementation(bool fail)
{
   currentGoalIndices.Empty();
   // currentHuntedAmounts.Empty();
   currentGoals.Empty();
   if (fail) currentState = EQuestState::failedQuests;
   currentState = EQuestState::completedQuests;
   questManagerRef->EndQuest(this);
}

void AQuest::UpdateSubGoals()
{
   currentGoals.Empty();
   for (int i : currentGoalIndices) {
      currentGoals.Add(questInfo.subgoals[i]);
   }
}

void AQuest::FindInitialItemAmount(int goalIndex)
{
   // Figure out how much of this item we have so far
   int itemCount = 0;
   int itemID    = FCString::Atoi(*questInfo.subgoals[goalIndex].additionalNames[0].ToString());

   for (ABaseHero* hero : questManagerRef->controllerRef->GetBasePlayer()->heroes) {
      itemCount += hero->backpack->FindItemCount(itemID);
   }
   currentAmounts[goalIndex] = itemCount;
}

void AQuest::CompleteSubGoal(int goalIndex, bool fail)
{
   if (currentGoalIndices.Contains(goalIndex)) {
      FGoalInfo&      completedGoal = questInfo.subgoals[goalIndex];
      UQuestListSlot* questListSlot = questManagerRef->questListRef->GetQuestListSlot(this);

      // remove traces of this goal in our goal bookkeeping (REMOVE IT BEFORE YOU CAHNGE THE GOALSTATE SINCE EQUALITY OPERATOR RELIES ON IT and currentGoals are only copies of subgoal's content)
      currentGoals.RemoveSingle(completedGoal);
      questListSlot->RemoveSubGoalWidget(goalIndex);
      currentGoalIndices.Remove(goalIndex);

      // If this goal should update the description, and we didn't fail it...
      if (!fail) {
         if (completedGoals[completedGoals.Add(completedGoal)].shouldUpdateQuestDescription) {
            // Add on the extra description at the end.
            currentDescription = FText::Format(NSLOCTEXT("Quest", "Description", "{0}\n\n{1}"), currentDescription, completedGoal.updatedDescription);
            if (questManagerRef->questJournalRef->GetSelectedQuest() == this) questManagerRef->questJournalRef->UpdateDetailWindow();
            questManagerRef->controllerRef->GetHUDManager()->GetMainHUD()->DisplayHelpText(NSLOCTEXT("Quest", "DetailsUpdated", "Quest Journal Updated!"));
         } else {
            questManagerRef->controllerRef->GetHUDManager()->GetMainHUD()->DisplayHelpText(NSLOCTEXT("Quest", "GoalCompleted", "Quest Goal Completed!"));
         }
         completedGoal.goalState = EGoalState::completedGoal;
      } else
         completedGoal.goalState = EGoalState::failedGoal;

      for (FTriggerData& trigger : completedGoal.afterGoalTriggers) {
         questManagerRef->controllerRef->GetGameMode()->GetTriggerManager()->ActivateTrigger(trigger);
      }

      // Add any subgoals which don't have any requiredSubGoals
      int index = 0;

      for (FGoalInfo& goal : questInfo.subgoals) {
         if (goal.goalState == EGoalState::lockedGoal) {
            int numRemoved = goal.requiredSubGoalIndices.Remove(goalIndex);
            if (numRemoved && !goal.requiredSubGoalIndices.Num()) {
               currentGoals.Add(goal);
               currentGoalIndices.Add(index);
               questManagerRef->questListRef->GetQuestListSlot(this)->AddSubGoalWidget(this, index);

               if (goal.goalType == EGoalType::Find) FindInitialItemAmount(index);

               for (FTriggerData& trigger : goal.prevGoalTriggers) {
                  questManagerRef->controllerRef->GetGameMode()->GetTriggerManager()->ActivateTrigger(trigger);
               }
            }
         }
         ++index;
      }

      // handle failure or completion
      if (completedGoal.canFailQuest || completedGoal.canCompleteQuest || currentGoals.Num() == 0) {
         CompleteQuest(fail);
         return;
      }

      if (questManagerRef->controllerRef->GetHUDManager()->GetQuestJournal()->GetSelectedQuest() == this) // if our quest selected in quest journal
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

         for (FTriggerData& trigger : questInfo.subgoals[i].prevGoalTriggers) {
            questManagerRef->controllerRef->GetGameMode()->GetTriggerManager()->ActivateTrigger(trigger);
         }
      } else
         goal.goalState = EGoalState::lockedGoal;
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
   if (!goalInfo.isCustomGoal) {
      FFormatOrderedArguments args;
      checkf(goalInfo.additionalNames.Num() > 0, TEXT("%s"), *goalInfo.goalText.ToString()) switch (goalInfo.goalType)
      {
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
            if (goalInfo.amount > 1) {
               args.Add(NSLOCTEXT("Quests", "Suffix", "s"));
               args.Add(assignedQuest->currentAmounts[goalIndex]);
               args.Add(goalInfo.amount);
               args.Add(goalInfo.additionalNames[1]);
               return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Bring: {0}{1} {2}/{3} to {4}!"), args);
            }
            args.Add(goalInfo.additionalNames[1]);
            return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Bring a {0} to {1}"), args);
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