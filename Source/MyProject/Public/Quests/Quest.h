#pragma once

#include "QuestStructs.h"
#include "QuestEnums.h"
#include "EventSystem/DEPRECATED_Trigger.h"
#include "GameFramework/Info.h"
#include "UpGoal.h"
#include "Quest.generated.h"

class UQuestManager;
class UQuestListSlot;
class UNamedInteractableDecorator;

UCLASS(Blueprintable, BlueprintType, AutoCollapseCategories = (Actor))
class MYPROJECT_API AQuest : public AInfo
{
   GENERATED_BODY()

 public:
   UFUNCTION(BlueprintCallable, BlueprintPure)
   const FQuestInfo& GetQuestInfo() const { return questInfo; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Bookkeeping")
   const TArray<UUpGoal*>& GetSubgoals() const { return questInfo.GetSubgoals(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Bookkeeping")
   TArray<UUpGoal*> GetCurrentGoals() const;

   /**
    *Accessor to the indices of the goals that are currently being handled in the quest)
    */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessor")
   const TArray<int>& GetCurrentGoalIndices() const { return currentGoalIndices; }

   /**
    *Accessor to goals
    */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessor")
   UUpGoal* GetGoalAtIndex(int goalIndex);

   /**
    *Quest state accessor for BP
    */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessor")
   EQuestState GetQuestState() const { return currentState; }

   const FText& GetCurrentDescription() const { return currentDescription; }

   /**
    *Initiates completion of a subgoal
    *@param goalIndex - Index of the goal in quest subgoal array
    */
   void CompleteSubGoal(int goalIndex);

   /** Initiates completing the quest */
   UFUNCTION(BlueprintNativeEvent, Category = "Updating")
   void CompleteQuest();
   void CompleteQuest_Implementation();

   /** Called when starting or restarting a quest, setup what goals will be first */
   UFUNCTION(BlueprintCallable, Category = "Setup")
   void SetupStartingGoals();

 protected:
   UFUNCTION()
   void BeginPlay() override;

   /**
   *Index of what goals are currently in progress
   */
   UPROPERTY(BlueprintReadOnly, Category = "Bookkeeping", meta = (AllowPrivateAccess = true))
   TArray<int> currentGoalIndices;

   /**
   *Struct with information about quest
   */
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Information")
   FQuestInfo questInfo;

 private:
   /**
   * Called at spawn time to find out how many items already obtained for find type goal
   */
   void FindInitialItemAmount(int goalIndex);

   void SetupUnlockedGoals(int completedGoalIndex, ARTSGameMode* gameModeRef);

   UPROPERTY(BlueprintReadOnly, Category = "Bookkeeping", meta = (AllowPrivateAccess = true))
   TArray<int> completedGoalIndices;

   UPROPERTY(BlueprintReadOnly, Category = "Bookkeeping", meta = (AllowPrivateAccess = true))
   EQuestState currentState;

   UPROPERTY(BlueprintReadOnly, Category = "Bookkeeping", meta = (AllowPrivateAccess = true))
   FText currentDescription;
};
