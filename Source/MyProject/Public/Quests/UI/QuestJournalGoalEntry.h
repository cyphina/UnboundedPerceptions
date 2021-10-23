#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Quests/Quest.h"
#include "QuestJournalGoalEntry.generated.h"

class UTextBlock;
class UImage;

/**
* Icon inside quest journal the represents the goals (that are completed, failed, or in progress) for a selected quest entry
* These are recreated as goals get completed/failed, and have to be updated when journal is open similar to SubGoalWidgets.
*/
UCLASS(meta=(DisableNativeTick))
class MYPROJECT_API UQuestJournalGoalEntry : public UUserWidget
{
   GENERATED_BODY()

public:
   UFUNCTION(BlueprintCallable, Category="Widget Creation")
   void UpdateEntryWidget();

protected:
   void NativeOnInitialized() override;
   
   UPROPERTY(BlueprintReadOnly, Meta=(BindWidget))
   UImage* Image_GoalState;

   UPROPERTY(BlueprintReadOnly, Meta=(BindWidget))
   UTextBlock* Text_GoalDesc;

   UPROPERTY(EditDefaultsOnly)
   UTexture2D* Texture2D_LockedGoal;

   UPROPERTY(EditDefaultsOnly)
   UTexture2D* Texture2D_CurrentGoal;

   UPROPERTY(EditDefaultsOnly)
   UTexture2D* Texture2D_CompletedGoal;
   
private:
   
   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
   UUpGoal* goal;
};
