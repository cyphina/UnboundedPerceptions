// Created 3/15/21 5:10 AM

#pragma once
#include "GameplayEffectExecutionCalculation.h"
#include "RTSHealthReplenishCalculation.generated.h"

struct FUpDamage;

/**
 * @brief Handles healing and replenishing mana. Does not handle manaburn nor damage, so don't try to invert the numbers to hack a solution together.
 */
UCLASS()
class MYPROJECT_API URTSHealthReplenishCalculation : public UGameplayEffectExecutionCalculation
{
   GENERATED_BODY()

 public:
   URTSHealthReplenishCalculation();

 protected:
   void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams, FGameplayEffectCustomExecutionOutput& outExecutionOutput) const override;
   void HealTarget(FUpDamage& d, FGameplayTagContainer effects) const;

 private:
   void ApplyHealingEffects(FUpDamage& d, FGameplayTagContainer effects) const;
   void BroadcastHealingEffects(const FUpDamage& d) const;
};
