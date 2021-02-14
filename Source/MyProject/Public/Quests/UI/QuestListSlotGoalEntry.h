#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Quest.h"
#include "QuestListSlotGoalEntry.generated.h"

class UBorder;
class UTextBlock;
class UButton;
class UQuestListSlot;

/**
* Class for goal widgets inside the quest list. Similar to those inside the quest journal
*/
UCLASS(meta=(DisableNativeTick))
class MYPROJECT_API UQuestListSlotGoalEntry : public UUserWidget
{
   GENERATED_BODY()

public:
   /**Update display text on this goal whenever things change like we hunt something, or a goal is completed*/
   UFUNCTION(BlueprintCallable, Category = "Quest Slot Creation")
   void UpdateText();

   /**Changes if button is enabled and how goal looks when selected or deselected*/
   void ToggleEnabled(bool bShouldEnable);

   int GetCorrespondingGoalIndex(AQuest* questWithThisGoal) const;
   
protected:
   void NativeOnInitialized() override;

   UPROPERTY(BlueprintReadOnly, Meta=(BindWidget))
   UBorder* Bdr_Goal;
   
   UPROPERTY(BlueprintReadOnly, Meta=(BindWidget))
   UButton* Btn_SelectGoal;

   UPROPERTY(BlueprintReadOnly, Meta=(BindWidget))
   UTextBlock* Text_GoalText;
   
private:
   UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true), Category = "Info")
   UUpGoal* goal;

   UFUNCTION()
   void OnBtnSelectGoalClicked();
};
