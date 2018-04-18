// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "BasePlayer.h"
#include "RTSGameMode.generated.h"

/**
 * 
 */

UCLASS()
class MYPROJECT_API ARTSGameMode : public AGameModeBase
{
	GENERATED_BODY()

	///<summary> Level Names </summary>
	const FString			startingLevelName = "StartMap";
	const FString			theIntroduction = "TheIntroduction";
	
	void BeginPlay() override;
	//list of all our widgets

public:
	
	ARTSGameMode();

	UFUNCTION(BlueprintCallable, Category = "Levels")
	FString					GetStartingLevelName() const { return startingLevelName; }

	UFUNCTION(BlueprintCallable, Category = "Levels")
	FString					GetIntroductionLevelName() const { return theIntroduction; }

};
