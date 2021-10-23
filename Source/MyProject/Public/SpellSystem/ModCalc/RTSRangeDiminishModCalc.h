#pragma once

#include "GameplayModMagnitudeCalculation.h"
#include "RTSRangeDiminishModCalc.generated.h"

/**
 * Just returns the value of a range set by caller magnitude which we can scale using coefficients that are built into mod mag calcs.
 */
UCLASS()
class MYPROJECT_API URTSRangeDiminishModCalc : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	URTSRangeDiminishModCalc();
	
private:
	float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
