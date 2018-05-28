// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelScripts/UPLevelScript.h"
#include "StartLevelScript.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AStartLevelScript : public AUPLevelScript
{
	GENERATED_BODY()
	
public:

	void				BeginPlay() override;
	
};
