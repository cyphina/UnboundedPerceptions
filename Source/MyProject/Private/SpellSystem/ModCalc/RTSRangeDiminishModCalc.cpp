#include "MyProject.h"
#include "ModCalc/RTSRangeDiminishModCalc.h"
#include "Unit.h"
#include "StatChangeCalc.h"

URTSRangeDiminishModCalc::URTSRangeDiminishModCalc()
{
   const AttStruct attributes;
   RelevantAttributesToCapture.Add(attributes.HealthDef);
   RelevantAttributesToCapture.Add(attributes.ManaDef);
   RelevantAttributesToCapture.Add(attributes.StrengthDef);
   RelevantAttributesToCapture.Add(attributes.UnderstandingDef);
   RelevantAttributesToCapture.Add(attributes.IntelligenceDef);
   RelevantAttributesToCapture.Add(attributes.AgilityDef);
}

float URTSRangeDiminishModCalc::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
   const float rangeBonus = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Combat.Effect.Data.RangeBonus"), true, 0);
   return rangeBonus;
}
