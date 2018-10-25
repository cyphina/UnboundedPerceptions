// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "FindVisibleEnemiesCenterContext.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API UFindVisibleEnemiesCenterContext : public UEnvQueryContext
{
   GENERATED_BODY()

   void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
