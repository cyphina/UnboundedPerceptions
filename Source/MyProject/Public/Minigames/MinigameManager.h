// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MinigameTypes.h"
#include "MinigameManager.generated.h"

class AUserInput;
class UUserWidget;
class AMinigamePawn;

/**
 * Class used as an interface for any other part of the game that has to deal with minigames starting and stopping minigames.  These minigames are so small that we typically
 * store all their data in their respective pawns.  They are also all singleplayer - a multiplayer minigame will probably require a new level.
 */

UCLASS(Blueprintable)
class MYPROJECT_API UMinigameManager : public UObject
{
	GENERATED_BODY()
	
public:

	void Init(); 

	UPROPERTY(BlueprintReadWrite, Category = "References")
	AUserInput* controllerRef;

	UPROPERTY(BlueprintReadWrite, Category = "References")
	AMinigamePawn* minigamePawn;

	UPROPERTY(EditDefaultsOnly, Category = "References")
	TSubclassOf<AMinigamePawn> drawingPawn;

	UPROPERTY(EditDefaultsOnly, Category = "References")
	TSubclassOf<AMinigamePawn> nurikabePawn;
	
	UFUNCTION(BlueprintCallable, Category = "Minigame Starter")
	void StartMiniGame(EMinigameType minigameType, const FMinigameData& minigameData);

	//UFUNCTION(BlueprintNativeEvent, Category = "Minigame Starter")
	//void StartNurikabe(FMinigameData& minigameData);

private:

	void StartDrawingGame(const FMinigameData& minigameData);

	/**Most minigames end in a similar manner.  If the player beat the minigame, then activate some triggers.  If the player fails, activate
	* other trigger, or destroy the Destroy the pawn, possess the main pawn again 
	*/ 
	UFUNCTION(BlueprintCallable, Category = "Minigame Helper")
	void EndMiniGame();

	/**Quit the game early*/
	UFUNCTION(BlueprintCallable, Category = "Minigame Helper")
	void AbortMinigame();
};
