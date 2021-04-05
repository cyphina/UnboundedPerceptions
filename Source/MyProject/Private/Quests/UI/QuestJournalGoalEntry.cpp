#include "MyProject.h"
#include "QuestJournalGoalEntry.h"
#include "Image.h"
#include "TextBlock.h"

void UQuestJournalGoalEntry::NativeOnInitialized()
{
   Super::NativeOnInitialized();
}

void UQuestJournalGoalEntry::UpdateEntryWidget()
{
   Text_GoalDesc->SetText(goal->MakeGoalText());
   switch(goal->GetGoalState())
   {
      case EGoalState::completedGoal: Image_GoalState->SetBrushFromTexture(Texture2D_CompletedGoal); break;
      case EGoalState::currentGoal: Image_GoalState->SetBrushFromTexture(Texture2D_CurrentGoal); break;
      case EGoalState::lockedGoal: Image_GoalState->SetBrushFromTexture(Texture2D_LockedGoal); break;
   }
}
