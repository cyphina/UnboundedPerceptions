#pragma once

#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Items/Item.h"
#include "QuestManager.generated.h"

enum class EQuestState : uint8;
class ABasePlayer;
class AQuest;
class AUserInput;
class ABaseHero;
class UMyUserWidget;
class UQuestList;
class UQuestJournal;
class AEnemy;
class ANPC;
class AGoalActor;
class AInteractableBase;
class UNamedInteractableDecorator;
struct FBackpackUpdateResult;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestStarted, AQuest*, startedQuest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestCompleted, AQuest*, completedQuest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestFailed, AQuest*, failedQuest);

DECLARE_EVENT_TwoParams(AQuest, FOnSubgoalCompleted, AQuest*, int);

DECLARE_EVENT_TwoParams(AQuest, FOnSubgoalUnlocked, AQuest*, int);

DECLARE_EVENT_TwoParams(AQuest, FOnSubgoalUpdated, AQuest*, int);

UCLASS(Blueprintable)
class MYPROJECT_API UQuestManager : public UObject
{
   GENERATED_BODY()

 public:
   /**Add a new current quest.  Returns true on success, false on failure
   * @param questClassToSpawn - Class of quest actor to spawn
   */
   UFUNCTION(BlueprintCallable, Category = "Quest Managing")
   bool AddNewQuest(TSubclassOf<AQuest> questClassToSpawn);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest Managing")
   AGoalActor* GetGoalActor() const { return currentGoalActor; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest Data")
   const TArray<AQuest*>& GetActiveQuests() const { return activeQuests; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest Data")
   const TArray<AQuest*>& GetCompletedQuests() const { return completedQuests; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest Data")
   const TArray<AQuest*>& GetFailedQuests() const { return failedQuests; }

   /** Find a quest the player has found (a quest already loaded) */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest Helper")
   AQuest* FindObtainedQuestByClass(TSubclassOf<AQuest> QuestToFindClass) const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest Helper")
   AQuest* FindObtainedQuestByQuestStateAndClass(TSubclassOf<AQuest> QuestToFindClass, EQuestState QuestStateFilter) const;

   /**
    * Map of quest GameplayTagName to quest class so we can add new quests via triggers
    */
   UPROPERTY(EditAnywhere)
   TMap<FGameplayTag, TSubclassOf<AQuest>> questClassList;

   UPROPERTY(EditDefaultsOnly, Category = "Quest HUD")
   TSubclassOf<AGoalActor> goalActorClass;

   /**Called when a quest is completed to unlock new quests or such*/
   UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
   FOnQuestStarted OnQuestStartedDelegate;

   /**Called when a quest is completed to unlock new quests or such*/
   UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
   FOnQuestCompleted OnQuestCompletedDelegate;

   FOnSubgoalCompleted& OnSubgoalCompleted() { return OnSubgoalCompletedEvent; }
   FOnSubgoalUnlocked&  OnSubgoalUnlocked() { return OnSubgoalUnlockedEvent; }

   void Init();

 protected:
#if WITH_EDITOR
   void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

   UPROPERTY(BlueprintReadWrite, Category = "References")
   AUserInput* controllerRef;

   UPROPERTY(BlueprintReadWrite, Category = "References")
   UQuestList* questListRef;

   UPROPERTY(BlueprintReadWrite, Category = "References")
   UQuestJournal* questJournalRef;

 private:
   /**
    *Updates map that maps quest classes to quest gameplay tags whenever we recompile the blueprint
    */
   void UpdateQuestClassList();

   /**
    * Callback when enemy dies to check to see if this quest condition is fulfilled
    * Need callbacks here since we specifically need that parameter
    */
   void OnEnemyDie(AUnit* deadUnit);

   /**
    * Changes UI and data to match ending quest, and hands out rewards
    */
   void OnQuestCompleted(AQuest* questToEnd);

   /**
    * @brief Callback when we talk to an NPC
    * @param talkedToNPC - NPC we talked to.
    * @param conversationTopic - If this is the default tag, then it means there was no conversation topic.
    */
   void OnTalkNPC(ANPC* talkedToNPC, FGameplayTag conversationTopic);

   void OnItemPickedUp(const ABaseHero* heroPickingItem, const FBackpackUpdateResult& itemUpdateResult);

   void OnItemPurchased(const ABaseHero* purchasingHero, const FBackpackUpdateResult& addPurchasedItemResult,
                        const TArray<FBackpackUpdateResult>& removePaymentItemsResults);

   /**
    *Callback when Interactable is successfully interacted with
    * @param decoratorName - Name of the interactable with a "Named Decorator"
    */
   void OnInteracted(TSubclassOf<AInteractableBase> interactableClass, const FText& decoratorName);

   void RecalculateItemCountsForGoals(const FMyItem item);

   bool TurnInItemsFromGatherGoal(int gatherItemId, int numItemsToGather);

   void SetupWidgetReferences();

   void CompleteSubgoal(AQuest* quest, int goalIndex);

   void OnSubgoalSwitched(AQuest* quest, int goalIndex);

   bool HasAuthority() const;

   UPROPERTY()
   class AHUDManager* hudManagerRef;

   /**Actor that determines location of the quest*/
   AGoalActor* currentGoalActor;

   int currentDistance;

   /** List of all quests currently happening */
   TArray<AQuest*> activeQuests;

   /** Completed quests */
   TArray<AQuest*> completedQuests;

   /** Failed quests */
   TArray<AQuest*> failedQuests;

   FOnSubgoalCompleted OnSubgoalCompletedEvent;

   FOnSubgoalUnlocked OnSubgoalUnlockedEvent;

   FOnSubgoalUpdated OnSubgoalUpdatedEvent;
};
