// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "StunEffectCalc.generated.h"

/**
 * This execution simply sends an AI message to the controller of the stunned unit so the AI system knows to not wait for any current actions but to recalculate
 * the thought process since we probably don't want to m the same decision after we are stunned.
 */
UCLASS()
class MYPROJECT_API UStunEffectCalc : public UGameplayEffectExecutionCalculation
{
   GENERATED_BODY()

   void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams, OUT FGameplayEffectCustomExecutionOutput& outExecutionOutput) const override;
};
