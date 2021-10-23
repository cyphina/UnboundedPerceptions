// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "InvisibilityExec.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API UInvisibilityExec : public UGameplayEffectExecutionCalculation
{
   GENERATED_BODY()

   void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams, OUT FGameplayEffectCustomExecutionOutput& outExecutionOutput) const override;
};
