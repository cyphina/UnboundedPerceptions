// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSDOTEffect.h"

URTSDOTEffect::URTSDOTEffect()
{
   DurationPolicy = EGameplayEffectDurationType::HasDuration;
   ChanceToApplyToTarget.SetValue(1.f);
}
