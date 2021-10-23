// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Stats/RTSAttributeSet.h"
#include "Calcs/RTSDamageCalculation.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "RTSDamageEffect.h"

#include "RTSUnitDamageCalculation.h"

URTSDamageEffect::URTSDamageEffect()
{
   strength.DataName      = "Strength";
   intelligence.DataName  = "Intelligence";
   agility.DataName       = "Agility";
   understanding.DataName = "Understanding";
   health.DataName =        "Health";

   // When overriding this it doesn't have to necessarily still be instant
   DurationPolicy = EGameplayEffectDurationType::Instant;

   FGameplayEffectExecutionScopedModifierInfo modifierInfo;
   strength.DataTag                                  = FGameplayTag::RequestGameplayTag("Combat.Stats.Strength");
   modifierInfo.CapturedAttribute.AttributeSource    = EGameplayEffectAttributeCaptureSource::Source;
   modifierInfo.CapturedAttribute.AttributeToCapture = URTSAttributeSet::GetStrengthAttribute();
   modifierInfo.ModifierOp                           = EGameplayModOp::Override; // set it to max so nothing happens with the data, we're just using it as params during damage calculation
   modifierInfo.ModifierMagnitude                    = FGameplayEffectModifierMagnitude(strength);

   FGameplayEffectExecutionScopedModifierInfo modifierInfo2;
   intelligence.DataTag                               = FGameplayTag::RequestGameplayTag("Combat.Stats.Intelligence");
   modifierInfo2.CapturedAttribute.AttributeSource    = EGameplayEffectAttributeCaptureSource::Source;
   modifierInfo2.CapturedAttribute.AttributeToCapture = URTSAttributeSet::GetIntelligenceAttribute();
   modifierInfo2.ModifierOp                           = EGameplayModOp::Override; // set it to max so nothing happens with the data, we're just using it as params during damage calculation
   modifierInfo2.ModifierMagnitude                    = FGameplayEffectModifierMagnitude(intelligence);

   FGameplayEffectExecutionScopedModifierInfo modifierInfo3;
   agility.DataTag                                    = FGameplayTag::RequestGameplayTag("Combat.Stats.Agility");
   modifierInfo3.CapturedAttribute.AttributeSource    = EGameplayEffectAttributeCaptureSource::Source;
   modifierInfo3.CapturedAttribute.AttributeToCapture = URTSAttributeSet::GetAgilityAttribute();
   modifierInfo3.ModifierOp                           = EGameplayModOp::Override; // set it to max so nothing happens with the data, we're just using it as params during damage calculation
   modifierInfo3.ModifierMagnitude                    = FGameplayEffectModifierMagnitude(agility);

   FGameplayEffectExecutionScopedModifierInfo modifierInfo4;
   understanding.DataTag                              = FGameplayTag::RequestGameplayTag("Combat.Stats.Understanding");
   modifierInfo4.CapturedAttribute.AttributeSource    = EGameplayEffectAttributeCaptureSource::Source;
   modifierInfo4.CapturedAttribute.AttributeToCapture = URTSAttributeSet::GetUnderstandingAttribute();
   modifierInfo4.ModifierOp                           = EGameplayModOp::Override; // set it to max so nothing happens with the data, we're just using it as params during damage calculation
   modifierInfo4.ModifierMagnitude                    = FGameplayEffectModifierMagnitude(understanding);

   FGameplayEffectExecutionScopedModifierInfo modifierInfo5;
   health.DataTag                                  = FGameplayTag::RequestGameplayTag("Combat.Stats.Health");
   modifierInfo5.CapturedAttribute.AttributeSource    = EGameplayEffectAttributeCaptureSource::Source;
   modifierInfo5.CapturedAttribute.AttributeToCapture = URTSAttributeSet::GetHealthAttribute();
   modifierInfo5.ModifierOp                           = EGameplayModOp::Override; // set it to max so nothing happens with the data, we're just using it as params during damage calculation
   modifierInfo5.ModifierMagnitude                    = FGameplayEffectModifierMagnitude(health);

   FGameplayEffectExecutionDefinition e;
   e.CalculationClass = URTSUnitDamageCalculation::StaticClass();
   e.CalculationModifiers.Append({modifierInfo, modifierInfo2, modifierInfo3, modifierInfo4, modifierInfo5});
   Executions.Add(e);
}
