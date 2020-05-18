// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryGenerator.h"
#include "VisibleEnemiesQueryGenerator.generated.h"

/**
 * Generates a list of all the enemies that are visible to all the current player units
 */
UCLASS()
class MYPROJECT_API UVisibleEnemiesQueryGenerator : public UEnvQueryGenerator
{
   GENERATED_BODY()

   UVisibleEnemiesQueryGenerator();
   virtual void  GenerateItems(FEnvQueryInstance& queryInstance) const override;
   virtual FText GetDescriptionTitle() const override;
};
