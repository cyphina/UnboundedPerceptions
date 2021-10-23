#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/AnimHudWidget.h"
#include "QuestList.generated.h"

class AQuest;
class UQuestListSlot;
class ARTSGameMode;

DECLARE_EVENT_TwoParams(UQuestListSlot, FOnSubgoalSwitched, AQuest*, int);

/* A widget to display different quests in a list on the side of the UI Menu. */
UCLASS()
class MYPROJECT_API UQuestList : public UAnimHudWidget
{
   GENERATED_BODY()

public:
   /** Add a new widget to quest list when we decide to track a new quest (done in quest journal) */
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
   void AddToQuestList(AQuest* questActor);

   /** Remove quest list widget and its subgoals when we want to stop tracking it (done in quest journal) */
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
   void RemoveFromQuestList(AQuest* questActor);

   /** Hide distance indicator */
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
   void ToggleDistanceIndicatorVisibility(bool shouldSee);

   /** Set distance indicator */
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
   void SetDistanceText(int newDistance);

   /** Gets a quest's quest list slot (what slot is it in the quest list) */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   UQuestListSlot* GetQuestListSlot(AQuest* quest) const;

   AQuest* GetCurrentlySelectedQuest() const { return currentlySelectedQuest; }
   
   FOnSubgoalSwitched& OnSubgoalSwitched() { return OnSubgoalSwitchedEvent; }

protected:
   void NativeOnInitialized() override;
   
   UPROPERTY(BlueprintReadWrite, category = "References")
   AQuest* currentlySelectedQuest = nullptr;
   
   UPROPERTY(BlueprintReadWrite, category = "Data")
   TArray<UQuestListSlot*> questListSlots;
   
private:
   UFUNCTION()
   void OnQuestCompleted(AQuest* questToEnd);

   UFUNCTION()
   void OnQuestStarted(AQuest* questToStart);
   
   void UpdateDistanceToGoal();

   void OnSubgoalUnlocked(AQuest* quest, int unlockedGoalIndex);
   void OnSubgoalCompleted(AQuest* quest, int completedGoalIndex);
   void OnSubgoalSwitchedCallback(AQuest* quest, int goalIndex);
   
   FTimerHandle updateDistanceIndicatorHandle;
   
   FOnSubgoalSwitched OnSubgoalSwitchedEvent;
};
