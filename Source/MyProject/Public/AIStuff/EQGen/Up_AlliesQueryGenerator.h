#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryGenerator.h"
#include "Up_AlliesQueryGenerator.generated.h"

/**
 * Generates the player units that are considered visible to all the enemies.
 */
UCLASS()
class MYPROJECT_API UUp_AlliesQueryGenerator : public UEnvQueryGenerator
{
   GENERATED_BODY()

 public:
   UUp_AlliesQueryGenerator();

 protected:
   /** Filters out friendlies outside a certain radius */
   UPROPERTY(EditAnywhere, Category = "Generator Settings")
   float filterRadius = 2000;

 private:
   virtual void  GenerateItems(FEnvQueryInstance& queryInstance) const override;
   virtual FText GetDescriptionTitle() const override;
   FText         GetDescriptionDetails() const override;
};
