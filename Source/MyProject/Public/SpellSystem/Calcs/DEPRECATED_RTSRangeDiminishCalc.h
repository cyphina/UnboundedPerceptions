#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "DEPRECATED_RTSRangeDiminishCalc.generated.h"

class URTSAbilitySystemComponent;

/**
 * DEPRECIATED
 * ! Don't use this. This was used in skills that scale based on how close you were to some actor (look at anaphorica). But I failed miserably on creating this...
 * Instead of using a GameplayEffect to drive the updates use GameplayAbilities and keep them active. 
 * Calculation that runs periodically to check a unit's distance and adjust the power of the effect based on their distance to the caster.
 */
UCLASS(Deprecated)
class MYPROJECT_API UDEPRECATED_RTSRangeDiminishCalc : public UGameplayEffectExecutionCalculation
{
   GENERATED_BODY()

 public:
   UDEPRECATED_RTSRangeDiminishCalc();

   /*void  CacheLastBonus(URTSAbilitySystemComponent* ASC, float newRange) const;
   float GetLastBonus(URTSAbilitySystemComponent* ASC) const;*/

 private:
   void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams, FGameplayEffectCustomExecutionOutput& outExecutionOutput) const override;
};
