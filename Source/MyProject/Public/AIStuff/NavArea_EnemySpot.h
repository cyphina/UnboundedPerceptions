// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavAreas/NavArea.h"
#include "NavArea_EnemySpot.generated.h"

/**
 * Area class used by enemies so that certain move commands will try to avoid them
 */

UCLASS()
class MYPROJECT_API UNavArea_EnemySpot : public UNavArea
{
   GENERATED_BODY()
};
