// Created 3/15/21 5:12 AM

#include "MyProject.h"
#include "RTSVitalReplenishCalculation.h"
#include "StatChangeCalc.h"
#include "Unit.h"
#include "UpStatComponent.h"

URTSVitalReplenishCalculation::URTSVitalReplenishCalculation()
{
   AttStruct attributes;
   RelevantAttributesToCapture.Add(attributes.HealthDef);
   RelevantAttributesToCapture.Add(attributes.ManaDef);
}

void URTSVitalReplenishCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams,
                                                           FGameplayEffectCustomExecutionOutput&           outExecutionOutput) const
{
   UAbilitySystemComponent* targetComponent = executionParams.GetTargetAbilitySystemComponent();

   AUnit* targetUnit = nullptr;

   if(targetComponent)
   {
      targetUnit = Cast<AUnit>(targetComponent->AvatarActor);
   }

   if(targetUnit)
   {
      const FAggregatorEvaluateParameters evalParams;
      float statValue;
      UUpStatComponent* statComp = targetUnit->GetStatComponent();

      for(FGameplayEffectAttributeCaptureDefinition captureDef : RelevantAttributesToCapture)
      {
         if(executionParams.AttemptCalculateCapturedAttributeMagnitude(captureDef, evalParams, statValue))
         {
            if(captureDef.AttributeToCapture == UMyAttributeSet::GetHealthAttribute())
            {
               statComp->ModifyStats(statComp->GetVitalCurValue(EVitals::Health) + statValue, EVitals::Health);
            }
            else
            {
               targetUnit->GetStatComponent()->ModifyStats(statComp->GetVitalCurValue(EVitals::Mana) + statValue, EVitals::Mana);
            }
         }
      }
   }
}
