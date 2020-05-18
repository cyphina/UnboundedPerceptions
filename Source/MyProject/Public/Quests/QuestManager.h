// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Items/Item.h"
#include "QuestManager.generated.h"

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
class UNamedInteractableDecorator;

UCLASS()
class MYPROJECT_API UQuestMap : public UDataAsset
{
   GENERATED_BODY()

 public:
   /**Map of classes from which we can activate quests from*/
   UPROPERTY(EditAnywhere)
   TMap<FGameplayTag, TSubclassOf<AQuest>> questClassList;
};

// TODO: Add a delegate for if quest failed
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestCompleted);

UCLASS(Blueprintable)
class MYPROJECT_API UQuestManager : public UObject
{
   GENERATED_BODY()

 public:
 #if WITH_EDITOR
   void PostEditChangeProperty( struct FPropertyChangedEvent& PropertyChangedEvent) override;
 #endif

   /**Updates map that maps quest classes to quest gameplaytags*/
   void UpdateQuestClassList();

   UPROPERTY(BlueprintReadWrite, Category = "References")
   AUserInput* controllerRef;

   UPROPERTY()
   class AHUDManager* hudManagerRef;

   /**
    * UI element that lists all the quests in a sidebar
    */
   UPROPERTY(BlueprintReadWrite, Category = "References")
   UQuestList* questListRef;

   UPROPERTY(BlueprintReadWrite, Category = "References")
   UQuestJournal* questJournalRef;

   /**
    *Map of quest GameplayTagName to quest class so we can add new quests via triggers
    */
   UPROPERTY(EditAnywhere)
   TMap<FGameplayTag, TSubclassOf<AQuest>> questClassList;

   /**Actor that determies location of the quest*/
   UPROPERTY(BlueprintReadWrite, Category = "References")
   AGoalActor* currentGoalActor;

   /**List of all quests currently happening*/
   UPROPERTY(BlueprintReadWrite, Category = "Quest Managing")
   TArray<AQuest*> quests;

   /**Distance away from goalactor*/
   UPROPERTY(BlueprintReadWrite, Category = "Quest Managing")
   int currentDistance;

   /**Completed quests*/
   UPROPERTY(BlueprintReadWrite, Category = "Quest Managing")
   TArray<AQuest*> completedQuests;

   /**Failed quests*/
   UPROPERTY(BlueprintReadWrite, Category = "Quest Managing")
   TArray<AQuest*> failedQuests;

   UPROPERTY(EditDefaultsOnly, Category = "Quest HUD")
   TSubclassOf<AGoalActor> goalActorClass;

   /**Called when a quest is completed to unlock new quests or such*/
   UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
   FOnQuestCompleted OnQuestCompletedDelegate;

   void Init();

   /**Select a new quest in the quest list*/
   UFUNCTION(BlueprintCallable, Category = "Quest Managing")
   void SelectNewQuest(AQuest* quest);

   /**Add a new current quest.  Returns true on success, false on failure
    * @param questClassToSpawn - Class of quest actor to spawn
    * @param forcedStart - Forces this quest to be selected in the quest journal
    */
   UFUNCTION(BlueprintCallable, Category = "Quest Managing")
   bool AddNewQuest(TSubclassOf<AQuest> questClassToSpawn, bool forcedStart);

   /**Called when switching subgoals to change goal actor location and change UI*/
   UFUNCTION(BlueprintCallable, Category = "Quest Managing")
   void OnSwitchSubGoal();

   /**Get distance to goal actor (distance marker)*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest Managing")
   int GetDistanceToGoal();

   /**Changes UI and data to match ending quest, and hands out rewards*/
   UFUNCTION(BlueprintCallable, Category = "Quest Managing")
   void EndQuest(AQuest* questToEnd);

   /**Used to complete all goals that aren't finished when completing a quest*/
   UFUNCTION(BlueprintCallable, Category = "Quest Managing")
   void CompleteGoals();

   /**Called when party leader moves to recalculate distance and move arrow around*/
   UFUNCTION(BlueprintCallable, Category = "Callbacks")
   void OnPartyLeaderMove();

   /**Callback when enemy dies to check to see if this quest condition is fufilled
    * Need callbacks here since we specifically need that parameter
    */
   UFUNCTION(BlueprintCallable, Category = "Callbacks")
   void OnEnemyDie(const AEnemy* enemyClass);

   /**Callback when NPC is talked to to check to see if this quest condition is fufilled
    * @param nPCClass - Class of the NPC that we're talking to
    * @param conversationTopic - Gameplay tag representing conversationTopic.  If conversationTopic is default tag, then it means no conversation topic was used (default conversation)
    */
   UFUNCTION(BlueprintCallable, Category = "Callbacks")
   void OnTalkNPC(const ANPC* talkedToNPC, FGameplayTag conversationTopic);

   /**Callback when Item is picked up to see if this quest condition is fulfilled*/
   UFUNCTION(BlueprintCallable, Category = "Callbacks")
   void OnItemPickup(const FMyItem& newItem);

   /**Callback when Interactable is sucessfully interacted with*/
   UFUNCTION(BlueprintCallable, Category = "Callbacks")
   void OnInteracted(const UNamedInteractableDecorator* finishedInteractableDialog);
};