// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryGenerator.h"
#include "VisibleAlliesQueryGenerator.generated.h"

/**
 * Generates the player units that are considered visible to all the enemies.
 */
UCLASS()
class MYPROJECT_API UVisibleAlliesQueryGenerator : public UEnvQueryGenerator
{
   GENERATED_BODY()

   UVisibleAlliesQueryGenerator();
   virtual void  GenerateItems(FEnvQueryInstance& queryInstance) const override;
   virtual FText GetDescriptionTitle() const override;
};
