// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RTSGameState.generated.h"

/**
 * Game State has information that is replicated to all clients.  Put here things that all players need to know.
 * Keep track of the state of the game (connected players).  1 Game State has many Player States, while each player states reference the 1 Game State
 */

class UMainWidget;
class AEnemy;
class AAlly;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateGameSpeed, float, speedMultiplier);

UCLASS()
class MYPROJECT_API ARTSGameState : public AGameStateBase
{
	GENERATED_BODY()

	static constexpr int			arraySize = 3;
	const float						SECONDS_IN_DAY = 86400;

	//how many seconds in game does one second IRL correspond to when at 1x speed?
	float							defaultGameTimeSpeed = 60;
	//rate at which our clock counts seconds.  By default defaultGameTimeSpeed but can be modified by speed modifiers
	float							timeUnit; 

	//accumulates in-game seconds
	double							clockwork;

	int								seconds{ 0 }, minutes{ 0 }, hours{ 0 };
	int								days{ 0 }, months{ 0 }, years{ 0 };

	TArray<int>						gameTime;
	TArray<int>						gameDate;

	//Let this only update clock
	void							Clock();
	//Let this only update game calendar
	void							Calendar();

public:

	ARTSGameState();
	void							Tick(float deltaSeconds) override;
	void							BeginPlay() override;

	/**Set by CPC*/
	UPROPERTY(BlueprintReadWrite, Category = "References")
	UMainWidget*					mainWidgetRef;

	///---Game Time and Speed---
	UPROPERTY(BlueprintReadWrite, Category = "Speed")
	float							speedModifier = 1;
	
	/**Collection of callbacks when game speed updated*/
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Speed")
	FUpdateGameSpeed				UpdateGameSpeedDelegate;
	

	/**Callback to update our timeunit when we change game speed*/
	UFUNCTION()
	void							UpdateGameSpeed(float speedMultiplier);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Time")
	TArray<int>						GetGameTime() const { return gameTime; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Time")
	TArray<int>						GetGameDate()	const { return gameDate; }

	/**Set the game time to some time*/
	UFUNCTION(BlueprintCallable, Category = "Time")
	void							UpdateGameTime(int second = 0, int minute = 0, int hour = 0);
	/**Set the game date to some date*/
	UFUNCTION(BlueprintCallable, Category = "Time")
	void							UpdateGameDay(int days = 0, int months = 0, int years = 0);
	/**Add game time*/
	UFUNCTION(BlueprintCallable, Category = "Time")
	void							AddGameTime(FDateTime timeToAdd);

	///---Unit Lists and Vision---

	FHitResult								visionHitResult;
	FCollisionObjectQueryParams				queryParamVision;

	FTimerHandle							allyVisionUpdateTimerHandle;
	FTimerHandle							enemyVisionUpdateTimerHandle;

	/**Lists all party members that exist between every player (necessary for computing co op vision)*/
	UPROPERTY(BlueprintReadWrite, Category = "SharedData")
	TSet<AAlly*>					allyList;

	/**Lists of all enemies in the level*/
	UPROPERTY(BlueprintReadWrite, Category = "SharedData")
	TSet<AEnemy*>					enemyList;
	
	/**Lists what enemies are visible so we don't have to keep doing line traces which is an expensive op*/
	UPROPERTY(BlueprintReadOnly, Category = "Vision")
	TSet<AEnemy*>					visibleEnemies;

	/**Lists what allies are visible so we don't have to keep doing line traces which is an expensive op*/
	UPROPERTY(BlueprintReadOnly, Category = "Vision")
	TSet<AAlly*>					visibleAllies;

	/**
	 * Visiblity of enemies is like a state machine which has six states
	 * Enemy enters vision range and we can see it - Add to possible enemies in radius and add to visible units
	 * Enemy enters vision range but is behind a wall - Add to possible enemies in radius but not to visible units
	 * Enemy leaves vision range - Remove from possible enemies in radius and from visible units
	 * Enemy leaves vision range but was behind a wall so we never saw it - Remove from possible enemies in radius but not from visible units
	 * Enemy peaks a wall and is now in vision - Add to visible enemies but not to possible enemies in radius
	 * Enemy walks behind a wall and is not in vision - Remove from visible enemies but not from possible enemies in radius
	 */
	UFUNCTION()
	void							UpdateVisibleEnemies();

	/**For more info look at comments of UpdateVisibleEnemies*/
	UFUNCTION()
	void							UpdateVisibleAllies();
};
