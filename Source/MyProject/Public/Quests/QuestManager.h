// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "QuestManager.generated.h"

class ABasePlayer;
class AUserInput;
class ABaseHero;
class UMyUserWidget;
class UQuestList;
class UQuestJournal;
class AEnemy;
class ANPC;
class AGoalActor;

UCLASS()
class MYPROJECT_API UQuestMap : public UDataAsset
{
	GENERATED_BODY()

public:

	/**Map of classes from which we can activate quests from*/
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, TSubclassOf<AQuest>>		questClassList;
};

//TODO: Add a delegate for if quest failed
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestCompleted);


UCLASS(Blueprintable)
class MYPROJECT_API UQuestManager : public UObject
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(BlueprintReadWrite, Category = "References")
	AUserInput*								controllerRef;

	/**
	 * UI element that lists all the quests in a sidebar
	 */
	UPROPERTY(BlueprintReadWrite, Category = "References") 
	UQuestList*								questListRef;

	UPROPERTY(BlueprintReadWrite, Category = "References")
	UQuestJournal*							questJournalRef;

	/**Map of quest GameplayTagName to quest class*/
	UPROPERTY(EditAnywhere, Category = "Quest Data")
	UQuestMap*								questMap;

	/**actor that determies location of the quest*/
	UPROPERTY(BlueprintReadWrite, Category = "References")
	AGoalActor*								currentGoalActor;
		
	/**list of all quests currently happening*/
	UPROPERTY(BlueprintReadWrite, Category = "Quest Managing")
	TArray<AQuest*>							quests;

	/**distance away from goalactor*/
	UPROPERTY(BlueprintReadWrite, Category = "Quest Managing")
	int										currentDistance;

	/**completed quests*/
	UPROPERTY(BlueprintReadWrite, Category = "Quest Managing")
	TArray<AQuest*>							completedQuests;

	/**failed quests*/
	UPROPERTY(BlueprintReadWrite, Category = "Quest Managing")
	TArray<AQuest*>							failedQuests;
	
	UPROPERTY(EditDefaultsOnly, Category = "Quest HUD")
	TSubclassOf<AGoalActor>					goalActorClass;

	/**Called when a quest is completed to unlock new quests or such*/
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
	FOnQuestCompleted						OnQuestCompletedDelegate;

	void									Init();

	/**Select a new quest in the quest list*/
	UFUNCTION(BlueprintCallable, Category = "Quest Managing")
	void									SelectNewQuest(AQuest* quest);

	/**Add a new current quest*/
	UFUNCTION(BlueprintCallable, Category = "Quest Managing")
	void									AddNewQuest(TSubclassOf<AQuest> questClassToSpawn, bool forcedStart);

	/**Called when switching subgoals to change goal actor location and change UI*/
	UFUNCTION(BlueprintCallable, Category = "Quest Managing")
	void									OnSwitchSubGoal();

	/**Get distance to goal actor (distance marker)*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest Managing")
	int										GetDistanceToGoal(); 

	/**Changes UI and data to match ending quest, and hands out rewards*/
	UFUNCTION(BlueprintCallable, Category = "Quest Managing")
	void									EndQuest(AQuest* questToEnd);

	/**Used to complete all goals that aren't finished when completing a quest*/
	UFUNCTION(BlueprintCallable, Category = "Quest Managing")
	void									CompleteGoals(); 

	/**Called when party leader moves to recalculate distance and move arrow around*/
	UFUNCTION(BlueprintCallable, Category = "Callbacks")
	void									OnPartyLeaderMove();

	/**Callback when enemy dies to check to see if this quest condition is fufilled*/
	UFUNCTION(BlueprintCallable, Category = "Callbacks")
	void									OnEnemyDie(TSubclassOf<AEnemy> enemyClass);

	/**Callback when NPC is talked to to check to see if this quest condition is fufilled*/
	UFUNCTION(BlueprintCallable, Category = "Callbacks")
	void									OnTalkNPC(TSubclassOf<ANPC> NPCClass);
};