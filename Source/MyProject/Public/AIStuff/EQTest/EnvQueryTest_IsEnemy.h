// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_IsEnemy.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UEnvQueryTest_IsEnemy : public UEnvQueryTest
{
	GENERATED_BODY()

	UEnvQueryTest_IsEnemy();

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;

	virtual FText GetDescriptionDetails() const override;
	
};
