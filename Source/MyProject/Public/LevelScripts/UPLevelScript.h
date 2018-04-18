// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/LevelScriptActor.h"
#include "EventSystem/Trigger.h"
#include "UPLevelScript.generated.h"


/**
 * Base script for all levels in UnboundedPreceptions
 * Includes a check to see if when a level script derived from this one is loaded up, to see if player loaded it or naturally transitioned to it, or if
 * moved to the level through gameplay
 * Includes time information for traveling between levels.  Maybe could use streaming instead.  
 */

class AUserInput;
class ARTSGameState;

UCLASS()
class MYPROJECT_API AUPLevelScript : public ALevelScriptActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "DefaultValues")
	bool				shouldTimePass = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "DefaultValues")
	FDateTime			timeToPass;

protected:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "References")
	AUserInput*			controllerRef = nullptr;

public:

	UPROPERTY(BlueprintReadWrite, Category = "OnLevelLoaded")
	FTriggerData		OnLevelLoadedTrigger;

	void				BeginPlay() override;
	
};
