// Created 3/19/21 11:9 PM

#include "MyProject.h"
#include "Unit.h"
#include "StatChangeCalc.h"
#include "RTSManaReplenishCalculation.h"

#include "UpStatComponent.h"

URTSManaReplenishCalculation::URTSManaReplenishCalculation()
{
   RelevantAttributesToCapture.Add(AttStruct().ManaDef);
}

void URTSManaReplenishCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams,
                                                          FGameplayEffectCustomExecutionOutput&           outExecutionOutput) const
{
   UAbilitySystemComponent* targetComponent = executionParams.GetTargetAbilitySystemComponent();

   AUnit* targetUnit = nullptr;

   if(targetComponent)
   {
      targetUnit = Cast<AUnit>(targetComponent->GetAvatarActor());
   }

   const FAggregatorEvaluateParameters evalParams;
   float                               manaReplenishValue;
   executionParams.AttemptCalculateCapturedAttributeMagnitude(AttStruct().ManaDef, evalParams, manaReplenishValue);

   if(targetUnit)
   {
      targetUnit->GetStatComponent()->ModifyStats(manaReplenishValue + targetUnit->GetStatComponent()->GetVitalCurValue(EVitals::Mana), EVitals::Mana);
   }
}
