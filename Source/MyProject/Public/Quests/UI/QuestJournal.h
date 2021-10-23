#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyDraggableWidget.h"
#include "../Quest.h"
#include "QuestJournal.generated.h"

class UButton;
class UScrollBox;
class UTextBlock;
class ABaseHero;
class ARTSGameMode;
class UQuestJournalEntry;

/* Class for a journal widget that displays more detailed information on quests */
UCLASS()
class MYPROJECT_API UQuestJournal : public UMyDraggableWidget
{
   GENERATED_BODY()

public:
   /**when one of the quests in the journal is selected*/
   UFUNCTION(BlueprintCallable, Category = "Quest Journal Interface")
   void OnQuestEntryClicked(AQuest* quest, UQuestJournalEntry* questButton);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   AQuest* GetSelectedQuest() const { return selectedQuest; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   UQuestJournalEntry* GetQuestJournalEntry(AQuest* quest);

   /** Update all details of quest information window*/
   void UpdateDetailWindow();

protected:
   /** Add a new entry to the quest journal or recategorizes an existing one*/
   UFUNCTION(BlueprintImplementableEvent, Category = "Quest Journal Updates")
   void AddOrReparentEntry(AQuest* quest);

   UFUNCTION(BlueprintImplementableEvent, Category = "Quest Journal Updates")
   void GenerateSubgoals();

   void NativeOnInitialized() override;
   bool OnWidgetAddToViewport_Implementation() override;
   void SetupQuestCategoryText(const FQuestInfo& questInfo);
   void SetupRewardsText(const FQuestInfo& questInfo);

   UPROPERTY(BlueprintReadOnly, Meta=(BindWidget))
   UScrollBox* Scroll_QuestDetailsPanel;

   UPROPERTY(BlueprintReadOnly, Meta=(BindWidget))
   UTextBlock* Text_SuggestedLevel;

   UPROPERTY(BlueprintReadOnly, Meta=(BindWidget))
   UTextBlock* Text_QuestName;

   UPROPERTY(BlueprintReadOnly, Meta=(BindWidget))
   UTextBlock* Text_QuestCategory;

   UPROPERTY(BlueprintReadOnly, Meta=(BindWidget))
   UTextBlock* Text_QuestRegion;

   UPROPERTY(BlueprintReadOnly, Meta=(BindWidget))
   UTextBlock* Text_Exp;

   UPROPERTY(BlueprintReadOnly, Meta=(BindWidget))
   UTextBlock* Text_Currency;

   UPROPERTY(BlueprintReadOnly, Meta=(BindWidget))
   UTextBlock* Text_Description;

   UPROPERTY(BlueprintReadOnly, Meta=(BindWidget))
   UButton* Btn_TrackQuest;

   UPROPERTY(BlueprintReadOnly, Meta=(BindWidget))
   UButton* Btn_CancelQuest;

   UPROPERTY(BlueprintReadWrite, category = "Data")
   TArray<UQuestJournalEntry*> questJournalEntries;

   UPROPERTY(BlueprintReadWrite, Category = "References")
   UQuestJournalEntry* currentQuestWidget;

private:
   void OnHeroLevelUp(ABaseHero* levelingHero);

   void OnSubgoalCompleted(AQuest* quest, int subgoalIndex);

   UFUNCTION()
   void OnQuestCompleted(AQuest* questToEnd);

   UFUNCTION()
   void OnQuestStarted(AQuest* questToStart);

   UFUNCTION()
   void TrackQuest();

   UFUNCTION()
   void StopQuestTracking();

   /** Change the color of the suggested level text based on if it's higher or lower level than our party leader*/
   void UpdateSuggestedLevelColor();

   void RemoveFromQuestJournal(AQuest* quest);

   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "References")
   AQuest* selectedQuest;

   FText noRewardsText = NSLOCTEXT("Quest", "NoReward", "N/A");
};
