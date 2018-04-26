// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
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

UCLASS(Blueprintable)
class MYPROJECT_API UQuestManager : public UObject
{
	GENERATED_BODY()
	
public:	
	
	UPROPERTY(BlueprintReadWrite, Category = "References")
	AUserInput*								controllerRef;

	UPROPERTY(BlueprintReadWrite, Category = "References") 
	UQuestList*								questListRef;

	UPROPERTY(BlueprintReadWrite, Category = "References")
	UQuestJournal*							questJournalRef;

	/**we need a ref to the leader to determine how far away it is from the goal actor*/
	UPROPERTY(BlueprintReadWrite, Category = "References")
	ABaseHero*								partyLeader;

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

	/**Map of classes from which we can add quests*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Quest Class List")
	TMap<FName, TSubclassOf<AQuest>>		questClassList;
	
	UPROPERTY(EditDefaultsOnly, Category = "Quest HUD")
	TSubclassOf<AGoalActor>					goalActorClass;

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