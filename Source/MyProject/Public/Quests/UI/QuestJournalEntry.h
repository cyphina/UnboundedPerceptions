#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestJournalEntry.generated.h"

class AQuest;
class UQuestJournal;

/* Class for widgets inside a quest journal */
UCLASS()
class MYPROJECT_API UQuestJournalEntry : public UUserWidget
{
   GENERATED_BODY()

 public:
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "UI")
   void ToggleButtonEnabled(bool isOn);

   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "UI")
   void LevelColorUpdate();

   UPROPERTY(BlueprintReadOnly, Category = "References", Meta = (ExposeOnSpawn = true))
   UQuestJournal* questJournal;

   UPROPERTY(BlueprintReadOnly, Category = "References", Meta = (ExposeOnSpawn = true))
   AQuest* assignedQuest;
};
