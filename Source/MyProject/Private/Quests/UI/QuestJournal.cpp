#include "MyProject.h"
#include "QuestJournal.h"

#include "BaseHero.h"
#include "Button.h"
#include "HUDManager.h"
#include "PartyDelegateContext.h"
#include "QuestJournalEntry.h"
#include "QuestList.h"
#include "RTSGameMode.h"
#include "RTSIngameWidget.h"
#include "ScrollBox.h"
#include "TextBlock.h"
#include "UpFunctionLibrary.h"
#include "UpStatComponent.h"
#include "UserInput.h"
#include "../QuestManager.h"

void UQuestJournal::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateContext>()->OnHeroLevelUp().AddUObject(this, &UQuestJournal::OnHeroLevelUp);
   if(ARTSGameMode* gameModeRef = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode()))
   {
      gameModeRef->GetQuestManager()->OnQuestStartedDelegate.AddDynamic(this, &UQuestJournal::OnQuestStarted);
      gameModeRef->GetQuestManager()->OnQuestCompletedDelegate.AddDynamic(this, &UQuestJournal::OnQuestCompleted);
      gameModeRef->GetQuestManager()->OnSubgoalCompleted().AddUObject(this, &UQuestJournal::OnSubgoalCompleted);
   }
   Btn_CancelQuest->OnClicked.AddDynamic(this, &UQuestJournal::StopQuestTracking);
   Btn_TrackQuest->OnClicked.AddDynamic(this, &UQuestJournal::TrackQuest);
}

bool UQuestJournal::OnWidgetAddToViewport_Implementation()
{
   // Kind of hacky since this is typically controlled in HUDManager but it's a quick fix for now
   SetVisibility(ESlateVisibility::SelfHitTestInvisible);
   UpdateDetailWindow();
   return true;
}

void UQuestJournal::UpdateDetailWindow()
{
   if(GetVisibility() != ESlateVisibility::Collapsed)
   {
      if(GetSelectedQuest())
      {
         const FQuestInfo& questInfo = GetSelectedQuest()->GetQuestInfo();

         Text_QuestName->SetText(questInfo.name);
         SetupQuestCategoryText(questInfo);

         // TODO: Set this QuestRegion
         Text_QuestRegion->SetText(NSLOCTEXT("Quest", "QuestRegion", "Endless Nightmare"));
         Text_SuggestedLevel->SetText(FText::AsNumber(questInfo.suggestedLvl));

         UpdateSuggestedLevelColor();
         SetupRewardsText(questInfo);
         Text_Description->SetText(GetSelectedQuest()->GetCurrentDescription());
         GenerateSubgoals();
         Scroll_QuestDetailsPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
      } else
      {
         Scroll_QuestDetailsPanel->SetVisibility(ESlateVisibility::Hidden);
      }
   }
}

void UQuestJournal::OnQuestEntryClicked(AQuest* quest, UQuestJournalEntry* questButton)
{
   if(currentQuestWidget)
   {
      currentQuestWidget->ToggleButtonEnabled(true);
   }

   if(IsValid(quest) && IsValid(questButton))
   {
      selectedQuest      = quest;
      currentQuestWidget = questButton;
      questButton->ToggleButtonEnabled(false);
   } else
   {
      selectedQuest      = nullptr;
      currentQuestWidget = nullptr;
   }

   UpdateDetailWindow();
}

UQuestJournalEntry* UQuestJournal::GetQuestJournalEntry(AQuest* quest)
{
   auto      pred  = [&](UQuestJournalEntry* questEntry) { return questEntry->assignedQuest == quest; };
   const int index = questJournalEntries.IndexOfByPredicate(pred);
   if(index != INDEX_NONE)
   {
      return questJournalEntries[index];
   }
   return nullptr;
}

void UQuestJournal::OnHeroLevelUp(ABaseHero* levelingHero)
{
   if(levelingHero == CPC->GetBasePlayer()->GetPartyLeader())
   {
      for(auto& questJournalEntry : questJournalEntries)
      {
         questJournalEntry->LevelColorUpdate();
      }

      if(GetVisibility() != ESlateVisibility::Collapsed)
      {
         UpdateSuggestedLevelColor();
      }
   }
}

void UQuestJournal::OnSubgoalCompleted(AQuest* quest, int subgoalIndex)
{
   if(selectedQuest == quest)
   {
      UpdateDetailWindow();
   }
}

void UQuestJournal::OnQuestCompleted(AQuest* questToEnd)
{
   AddOrReparentEntry(questToEnd);
   OnQuestEntryClicked(nullptr, nullptr);
}

void UQuestJournal::OnQuestStarted(AQuest* questToStart)
{
   AddOrReparentEntry(questToStart);
}

void UQuestJournal::UpdateSuggestedLevelColor()
{
   const int    questSuggestedLevel = GetSelectedQuest()->GetQuestInfo().suggestedLvl;
   const int    unitLevel           = UUpFunctionLibrary::GetHeroes(this)[0]->GetStatComponent()->GetUnitLevel();
   FLinearColor suggestedLevelColor;

   if(questSuggestedLevel > unitLevel)
   {
      suggestedLevelColor = FLinearColor::Red;
   } else if(questSuggestedLevel == unitLevel)
   {
      suggestedLevelColor = FLinearColor::Yellow;
   } else
   {
      suggestedLevelColor = FLinearColor::Red;
   }

   Text_SuggestedLevel->SetColorAndOpacity(suggestedLevelColor);
}

void UQuestJournal::RemoveFromQuestJournal(AQuest* quest)
{
   UQuestJournalEntry* questJournalEntry = GetQuestJournalEntry(quest);
   questJournalEntry->RemoveFromParent();
   questJournalEntries.Remove(questJournalEntry);
}

void UQuestJournal::TrackQuest()
{
   if(GetSelectedQuest() && GetSelectedQuest()->GetQuestState() != EQuestState::completedQuests)
   {
      UQuestList* questList = UUpFunctionLibrary::GetHUDManager(GetWorld())->GetIngameHUD()->GetQuestList();
      if(!questList->GetQuestListSlot(GetSelectedQuest()))
      {
         UUpFunctionLibrary::GetHUDManager(GetWorld())->GetIngameHUD()->GetQuestList()->AddToQuestList(GetSelectedQuest());
      }
   }
}

void UQuestJournal::StopQuestTracking()
{
   if(GetSelectedQuest())
   {
      UQuestList* questList = UUpFunctionLibrary::GetHUDManager(GetWorld())->GetIngameHUD()->GetQuestList();
      if(questList->GetQuestListSlot(GetSelectedQuest()))
      {
         UUpFunctionLibrary::GetHUDManager(GetWorld())->GetIngameHUD()->GetQuestList()->RemoveFromQuestList(GetSelectedQuest());
      }
      
   }
}

void UQuestJournal::SetupRewardsText(const FQuestInfo& questInfo)
{
   if(questInfo.questReward.exp > 0)
   {
      Text_Exp->SetText(FText::Format(NSLOCTEXT("Quest", "QuestRewardExp", "{0} Exp"), questInfo.questReward.exp));
   } else
   {
      Text_Exp->SetText(noRewardsText);
   }
   if(questInfo.questReward.gold > 0)
   {
      Text_Currency->SetText(FText::Format(NSLOCTEXT("Quest", "QuestRewardMoney", "{0} Squeezies"), questInfo.questReward.gold));
   } else
   {
      Text_Currency->SetText(noRewardsText);
   }
}

void UQuestJournal::SetupQuestCategoryText(const FQuestInfo& questInfo)
{
   Text_QuestCategory->SetText(UEnum::GetDisplayValueAsText(questInfo.questCategory));

   FLinearColor questCatColor;
   switch(questInfo.questCategory)
   {
      case EQuestCategory::Event: questCatColor = FLinearColor(0.63, 0.18, 0.57, 1);
      break;
      case EQuestCategory::MainQuest: questCatColor = FLinearColor(0.20, 0.48, 0.57, 1);
      break;
      case EQuestCategory::SideQuest: questCatColor = FLinearColor(0.30, 0.58, 0.29, 1);
      break;
   }

   Text_QuestCategory->SetColorAndOpacity(questCatColor);
}