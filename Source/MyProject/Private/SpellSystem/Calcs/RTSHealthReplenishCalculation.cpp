// Created 3/15/21 5:12 AM

#include "MyProject.h"
#include "RTSHealthReplenishCalculation.h"

#include "DamageDelegateStore.h"
#include "StatChangeCalc.h"
#include "DamageStructs.h"
#include "Unit.h"
#include "UnitController.h"
#include "UpStatComponent.h"

URTSHealthReplenishCalculation::URTSHealthReplenishCalculation()
{
   const AttStruct attributes;
   RelevantAttributesToCapture.Add(attributes.HealthDef);
   RelevantAttributesToCapture.Add(attributes.ManaDef);
   RelevantAttributesToCapture.Add(attributes.HealthRegenModifierDef);

   RelevantAttributesToCapture.Add(attributes.StrengthDef);
   RelevantAttributesToCapture.Add(attributes.UnderstandingDef);
   RelevantAttributesToCapture.Add(attributes.IntelligenceDef);
   RelevantAttributesToCapture.Add(attributes.AgilityDef);
}

void URTSHealthReplenishCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams,
                                                            FGameplayEffectCustomExecutionOutput&           outExecutionOutput) const
{
   UAbilitySystemComponent* ownerComponent  = executionParams.GetSourceAbilitySystemComponent();
   UAbilitySystemComponent* targetComponent = executionParams.GetTargetAbilitySystemComponent();

   AUnit* sourceUnit = nullptr;
   AUnit* targetUnit = nullptr;

   if(ownerComponent)
   {
      sourceUnit = Cast<AUnit>(ownerComponent->GetAvatarActor());
   }
   if(targetComponent)
   {
      targetUnit = Cast<AUnit>(targetComponent->GetAvatarActor());
   }

   if(sourceUnit && targetUnit)
   {
      FUpDamage damage;
      damage.sourceUnit = sourceUnit;
      damage.targetUnit = targetUnit;

      FGameplayTagContainer tags = executionParams.GetOwningSpec().DynamicGrantedTags;
      damage.element             = tags.Filter(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Element"))).GetByIndex(0);
      damage.effects             = tags;

      FAggregatorEvaluateParameters evalParams;
      evalParams.SourceTags = executionParams.GetOwningSpec().CapturedSourceTags.GetAggregatedTags();
      evalParams.TargetTags = executionParams.GetOwningSpec().CapturedTargetTags.GetAggregatedTags();

      const AttStruct attributes;
      float           healingBonus = 0;

      if(const int foundIndex = RelevantAttributesToCapture.Find(attributes.HealthRegenModifierDef); foundIndex != INDEX_NONE)
      {
         const FGameplayEffectAttributeCaptureDefinition& healthRegenBonusCaptureDef = RelevantAttributesToCapture[foundIndex];
         executionParams.AttemptCalculateCapturedAttributeMagnitude(healthRegenBonusCaptureDef, evalParams, healingBonus);
      }

      float healthMult;
      executionParams.AttemptCalculateCapturedAttributeMagnitude(attributes.HealthDef, evalParams, healthMult);
      float strMult;
      executionParams.AttemptCalculateCapturedAttributeMagnitude(attributes.StrengthDef, evalParams, strMult);
      float intMult;
      executionParams.AttemptCalculateCapturedAttributeMagnitude(attributes.IntelligenceDef, evalParams, intMult);
      float agiMult;
      executionParams.AttemptCalculateCapturedAttributeMagnitude(attributes.AgilityDef, evalParams, agiMult);
      float undMult;
      executionParams.AttemptCalculateCapturedAttributeMagnitude(attributes.UnderstandingDef, evalParams, undMult);

      const float healValue = (healthMult + strMult + intMult + agiMult + undMult) / 100 * (100 + healingBonus) / 100;
      damage.damage = healValue;

      HealTarget(damage, tags.Filter(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.DamageEffects"))));
   }
}

void URTSHealthReplenishCalculation::HealTarget(FUpDamage& d, FGameplayTagContainer effects) const
{
   const int damageRange = FMath::RandRange(75, 105);
   d.damage              = d.damage * (damageRange / 100.f);

   ApplyHealingEffects(d, effects);

   if(d.damage <= 0)
   {
      d.damage = 1;
   }

   BroadcastHealingEffects(d);
}

void URTSHealthReplenishCalculation::ApplyHealingEffects(FUpDamage& d, FGameplayTagContainer effects) const
{
}

void URTSHealthReplenishCalculation::BroadcastHealingEffects(const FUpDamage& d) const
{
   // Used for skills like soul assumption (dota)
   DamageEvents::OnHealingEvent.Broadcast(d);

   // Used to do unit specific damage tracking (including death for received)
   d.sourceUnit->OnUnitHealingDealt().Broadcast(d);
   d.targetUnit->OnUnitHealingReceived().Broadcast(d);
}
