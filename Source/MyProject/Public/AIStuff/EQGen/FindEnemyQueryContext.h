// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "WorldObjects/Unit.h"
#include "FindEnemyQueryContext.generated.h"

/**
 *Context provides results for our test
 * Context returns the enemy seen
 */
UCLASS()
class MYPROJECT_API UFindEnemyQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()

	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
