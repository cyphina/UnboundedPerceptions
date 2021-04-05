#include "MyProject.h"
#include "QuestListSlotGoalEntry.h"
#include "Border.h"
#include "Button.h"
#include "HUDManager.h"
#include "QuestList.h"
#include "QuestListSlot.h"
#include "RTSIngameWidget.h"
#include "TextBlock.h"
#include "UpFunctionLibrary.h"
#include "UpWidgetHelperLibrary.h"

void UQuestListSlotGoalEntry::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   Btn_SelectGoal->OnClicked.AddDynamic(this, &UQuestListSlotGoalEntry::OnBtnSelectGoalClicked);
}

void UQuestListSlotGoalEntry::OnBtnSelectGoalClicked()
{
   if(UQuestList* questList = UUpFunctionLibrary::GetHUDManager(this)->GetIngameHUD()->GetQuestList())
   {
      if(UQuestListSlot* questListSlot = UUpWidgetHelperLibrary::GetUserWidgetParent<UQuestListSlot>(this))
      {
         if(AQuest* questWithThisGoal = questListSlot->GetAssignedQuest())
         {
            questList->OnSubgoalSwitched().Broadcast(questWithThisGoal, GetCorrespondingGoalIndex(questWithThisGoal));
            ToggleEnabled(true);
         }
      }
   }
}

void UQuestListSlotGoalEntry::UpdateText()
{
   if(goal)
   {
      Text_GoalText->SetText(goal->MakeGoalText());
   }
}

void UQuestListSlotGoalEntry::ToggleEnabled(bool bShouldEnable)
{
   if(bShouldEnable)
   {
      Bdr_Goal->SetContentColorAndOpacity(FLinearColor(1, 1, 1, 1));
      SetIsEnabled(false);
   } else
   {
      Bdr_Goal->SetContentColorAndOpacity(FLinearColor(1, 1, 1, 0.5));
      SetIsEnabled(true);
   }
}

int UQuestListSlotGoalEntry::GetCorrespondingGoalIndex(AQuest* questWithThisGoal) const
{
   return questWithThisGoal->GetSubgoals().Find(goal);
}
