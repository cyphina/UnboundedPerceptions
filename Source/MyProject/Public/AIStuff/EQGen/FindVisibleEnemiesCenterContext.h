// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "FindVisibleEnemiesCenterContext.generated.h"

class AUnit;

/**
 * Provides to the environment query the center of all the visible enemy units
 */
UCLASS()
class MYPROJECT_API UFindVisibleEnemiesCenterContext : public UEnvQueryContext
{
   GENERATED_BODY()

 public:
   void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

 private:
   const TSet<AUnit*>& GetVisibleEnemies(const FEnvQueryInstance& queryInstance) const;
};
