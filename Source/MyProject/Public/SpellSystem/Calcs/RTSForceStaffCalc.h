// Created 2/28/21 07:42 AM

#pragma once
#include "GameplayEffectExecutionCalculation.h"
#include "RTSForceStaffCalc.generated.h"

UCLASS()
class MYPROJECT_API URTSForceStaffCalc : public UGameplayEffectExecutionCalculation
{
   GENERATED_UCLASS_BODY()

public:
   
protected:
   void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams, FGameplayEffectCustomExecutionOutput& outExecutionOutput) const override;

};
