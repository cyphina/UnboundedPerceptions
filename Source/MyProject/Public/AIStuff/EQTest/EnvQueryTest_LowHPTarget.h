// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_LowHPTarget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UEnvQueryTest_LowHPTarget : public UEnvQueryTest
{
	GENERATED_BODY()

	UEnvQueryTest_LowHPTarget();

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;

	virtual FText GetDescriptionDetails() const override;
};
