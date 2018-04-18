// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RTSGameState.generated.h"

/**
 * Game State, gamewide variables
 */

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
	//explicit ARTSGameState(int sec = 0, int min = 0, int h = 0, int d = 0, int mon = 0, int y = 0); 

	UPROPERTY(BlueprintReadWrite, Category = "Speed")
	float							speedModifier = 1;
	
	/**Collection of callbacks when game speed updated*/
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Pickup")
	FUpdateGameSpeed				UpdateGameSpeedDelegate;
	
	void							Tick(float deltaSeconds) override;
	void							BeginPlay() override;

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
};
