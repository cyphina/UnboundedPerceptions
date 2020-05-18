// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "SpellSystem/DamageStructs.h"
#include "DamageCalculation.generated.h"

class AUnit;

/**Damage calculation is used to calculate the damage.  Grabs information from target and receiving unit to figure out damage amounts, as well as
 * using information gotten from the effect itself
 */
UCLASS()
class MYPROJECT_API UDamageCalculation : public UGameplayEffectExecutionCalculation
{
   GENERATED_UCLASS_BODY()
   void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams, OUT FGameplayEffectCustomExecutionOutput& outExecutionOutput) const override;
};
