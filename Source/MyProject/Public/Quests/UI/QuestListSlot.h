#pragma once

#include "Blueprint/UserWidget.h"
#include "QuestListSlot.generated.h"

class UTextBlock;
class AQuest;
class UQuestListSlotGoalEntry;

/* Slots that go inside the quest list widget so the player can easily see what things they are partaking in */
UCLASS()
class MYPROJECT_API UQuestListSlot : public UUserWidget
{
   GENERATED_BODY()

public:
   /** Creates a widget for each starting subgoal of a quest */
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "QuestList UI")
   void GenerateSubWidgets();

   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "QuestList UI")
   void AddSubGoalWidget(AQuest* questRef, int goalIndex);

   UFUNCTION(BlueprintCallable, Category = "QuestList UI")
   void RemoveSubGoalWidget(int goalIndex);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   FORCEINLINE AQuest* GetAssignedQuest() const { return assignedQuest; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   FORCEINLINE int GetSelectedGoalIndex() const { return selectedGoalIndex; }

   void SetSelectedGoalIndex(int newGoalIndex) { selectedGoalIndex = newGoalIndex; }

   /** Given a goalIndex, we try to find a goal inside this quest list slot that matches it */
   UQuestListSlotGoalEntry* GetGoalEntryWithGoalIndex(int goalIndexToSearchFor) const;
   
   void OnSubgoalSelected(AQuest* quest, int goalIndex);

protected:
   void NativeOnInitialized() override;

   UPROPERTY(BlueprintReadOnly, Meta=(BindWidget))
   UTextBlock* Text_QuestName;
   
private:
   void OnSubgoalCompleted(AQuest* quest, int goalIndex);

   UPROPERTY(BlueprintReadOnly, Category = "References", Meta = (ExposeOnSpawn = true, AllowPrivateAccess = true))
   AQuest* assignedQuest;

   UPROPERTY(BlueprintReadOnly, Category = "References", Meta = (AllowPrivateAccess = true))
   TArray<UQuestListSlotGoalEntry*> subgoalWidgets;

   int selectedGoalIndex;
};
