// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryGenerator.h"
#include "Up_VisibleEnemiesQueryGenerator.generated.h"

/**
 * Generates a list of all the enemies that are visible to all the current player units
 */
UCLASS()
class MYPROJECT_API UUp_VisibleEnemiesQueryGenerator : public UEnvQueryGenerator
{
   GENERATED_BODY()

 public:
   UUp_VisibleEnemiesQueryGenerator();

 protected:
   /** Filters out enemies outside a certain radius. If set < 0, will filter out enemies within vision radius of the owner. */
   UPROPERTY(EditAnywhere, Category = "Generator Settings")
   float filterRadius = -1;

 private:
   virtual void  GenerateItems(FEnvQueryInstance& queryInstance) const override;
   virtual FText GetDescriptionTitle() const override;
   virtual FText GetDescriptionDetails() const override;
};
