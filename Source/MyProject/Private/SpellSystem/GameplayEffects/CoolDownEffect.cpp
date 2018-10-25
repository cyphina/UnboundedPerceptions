// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "CoolDownEffect.h"

UCoolDownEffect::UCoolDownEffect(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
   DurationPolicy                      = EGameplayEffectDurationType::HasDuration;
   FSetByCallerFloat f                 = FSetByCallerFloat();
   f.DataName                          = "Duration";
   DurationMagnitude                   = FGameplayEffectModifierMagnitude(f);
   bExecutePeriodicEffectOnApplication = false;
   ChanceToApplyToTarget.SetValue(1.f);
   StackingType                         = EGameplayEffectStackingType::None;
   StackLimitCount                      = 0;
   StackDurationRefreshPolicy           = EGameplayEffectStackingDurationPolicy::RefreshOnSuccessfulApplication;
   StackPeriodResetPolicy               = EGameplayEffectStackingPeriodPolicy::ResetOnSuccessfulApplication;
   bRequireModifierSuccessToTriggerCues = true;

#if WITH_EDITORONLY_DATA
   ShowAllProperties = true;
   Template          = nullptr;
#endif
}
