// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "BasePlayer.h"
#include "RTSGameMode.generated.h"

/**
 * Game mode only exists on server.  Things we want clients not see goes here.
 */

UCLASS()
class MYPROJECT_API ARTSGameMode : public AGameModeBase
{
	GENERATED_BODY()

	///<summary> Level Names </summary>
	const FString			startingLevelName = "StartMap";
	const FString			theIntroduction = "TheIntroduction";
	
	UPROPERTY(BlueprintReadWrite, Category = "Levels", Meta = (AllowPrivateAccess = "true"))
	FString					currentLevelName;

	void BeginPlay() override;

public:
	
	ARTSGameMode();

	UFUNCTION(BlueprintCallable, Category = Levels)
	FString					GetCurrentLevelName() const { return currentLevelName; }

	UFUNCTION(BlueprintCallable, Category = "Levels")
	FString					GetStartingLevelName() const { return startingLevelName; }

	UFUNCTION(BlueprintCallable, Category = "Levels")
	FString					GetIntroductionLevelName() const { return theIntroduction; }

	/**Stream in a level and put in the loading screen*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "LevelLoading")
	void					StreamLevelAsync(FName levelName);

};
