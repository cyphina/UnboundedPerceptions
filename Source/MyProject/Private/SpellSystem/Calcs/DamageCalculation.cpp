// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "DamageCalculation.h"
#include "WorldObjects/Unit.h"
#include "AIStuff/AIControllers/UnitController.h"
#include "AbilitySystemComponent.h"
#include "GameplayTags.h"
#include "StatChangeCalc.h"
#include "BaseCharacter.h"

UDamageCalculation::UDamageCalculation(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
   AttStruct attributes;
   // RelevantAttributesToCapture.Add(attributes.HealthDef);
   RelevantAttributesToCapture.Add(attributes.StrengthDef);
   RelevantAttributesToCapture.Add(attributes.UnderstandingDef);
   RelevantAttributesToCapture.Add(attributes.IntelligenceDef);
   RelevantAttributesToCapture.Add(attributes.AgilityDef);
}

void UDamageCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams, FGameplayEffectCustomExecutionOutput& outExecutionOutput) const
{
   Damage damage;
   // Owner component gotten from instigator ability component which gotten from owner we set as our player controller but that isn't an ability system holder
   UAbilitySystemComponent* ownerComponent  = executionParams.GetSourceAbilitySystemComponent();
   UAbilitySystemComponent* targetComponent = executionParams.GetTargetAbilitySystemComponent();
   AUnit *                  sourceUnit = nullptr, *targetUnit = nullptr;
   // if our components exist
   if (ownerComponent)
      sourceUnit = Cast<AUnit>(ownerComponent->AvatarActor); // If our AbilitySystemComponents are valid, we get each their owning actors and put them in variables. This is mostly to prevent crashing
                                                             // by trying to get the AvatarActor variable from
   if (targetComponent) targetUnit = Cast<AUnit>(targetComponent->AvatarActor); // a null pointer.

   if (sourceUnit && targetUnit) {
      // Get Element of this Damage
      FGameplayTagContainer tags = executionParams.GetOwningSpec().DynamicAssetTags;
      damage.element             = tags.Filter(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Element"))).GetByIndex(0);

      FAggregatorEvaluateParameters evalParams;
      evalParams.SourceTags = executionParams.GetOwningSpec().CapturedSourceTags.GetAggregatedTags();
      evalParams.TargetTags = executionParams.GetOwningSpec().CapturedTargetTags.GetAggregatedTags();

      AttStruct attributes;
      float     captureVal;
      for (FGameplayEffectAttributeCaptureDefinition captureDef : RelevantAttributesToCapture) {
         captureVal = 0;
         if (executionParams.AttemptCalculateCapturedAttributeMagnitude(captureDef, evalParams, captureVal))
            damage.damage += ownerComponent->GetNumericAttribute(captureDef.AttributeToCapture) * captureVal / 100;
      }
      DamageTarget(sourceUnit, targetUnit, damage, tags.Filter(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.DamageEffects"))));
   }
}

void UDamageCalculation::DamageTarget(AUnit* sourceUnit, AUnit* targetUnit, Damage& d, FGameplayTagContainer effects) const
{
   CalculateDamage(sourceUnit, d, effects);
   CalculateDamageReduction(targetUnit, d, effects);

   if (d.accuracy > 100) {
      targetUnit->ShowDamageDealt(NSLOCTEXT("Combat", "Dodge", "Dodged!"));
      return;
   }

   if (d.damage <= 0) d.damage = 1;

   float worldTime = sourceUnit->GetWorld()->GetTimeSeconds();
   if (!effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Healing"))) {
      targetUnit->baseC->GetVital(static_cast<int>(Vitals::Health))->SetCurrValue(targetUnit->GetVitalCurValue(static_cast<int>(Vitals::Health)) - d.damage);
      sourceUnit->combatParams.damageDealt.Insert(TPair<int, float>(d.damage, worldTime));
      targetUnit->combatParams.damageRecieved.Insert(TPair<int, float>(d.damage, worldTime));
   } else {
      targetUnit->baseC->GetVital(static_cast<int>(Vitals::Health))->SetCurrValue(targetUnit->GetVitalCurValue(static_cast<int>(Vitals::Health)) + d.damage);
      sourceUnit->combatParams.healingRecieved.Insert(TPair<int, float>(d.damage, worldTime));
      targetUnit->combatParams.healingDealt.Insert(TPair<int, float>(d.damage, worldTime));
   }
   targetUnit->ShowDamageDealt(d);

   if (targetUnit->GetVitalCurValue(static_cast<int>(Vitals::Health)) <= 0) {
      if (!targetUnit->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Buff.Immortality"))) {
         targetUnit->Die();
         sourceUnit->GetUnitController()->Stop();
      }
   }
}

void UDamageCalculation::CalculateDamage(AUnit* unit, Damage& d, FGameplayTagContainer& effects) const
{
   int damageRange = FMath::RandRange(75, 105);
   d.damage        = d.damage * (damageRange / 100.f);
   // GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString::FromInt(d.damage));
   d.piercing = 0;
   d.accuracy = unit->GetAttributeAdjValue(static_cast<int>(UnitStats::Accuracy));
   ApplyEffects(unit, d, effects);
   CalculatePiercing(unit, d, true);
}

void UDamageCalculation::ApplyEffects(AUnit* unit, Damage& d, FGameplayTagContainer& effects) const
{
}

void UDamageCalculation::ReceiveEffects(AUnit* unit, Damage& d, FGameplayTagContainer& effects) const
{
   // apply effects from buffs and armor
   //--This tag means this damage is pure, that is it doesn't get reduced by the global damage modifier--  Ex: Iron Maiden
   if (!effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Absolute")))
      d.damage = d.damage * (100 - unit->GetMechanicAdjValue(static_cast<int>(Mechanics::GlobalDamageModifier))) / 100;
   if (effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.NeverMiss"))) d.accuracy = 0;
   if (unit->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Buff.GodMode"))) d.damage = 0;
}

void UDamageCalculation::CalculateDamageReduction(AUnit* unit, Damage& d, FGameplayTagContainer& effects) const
{
   CalculatePiercing(unit, d, false); // Calculate defensive piercing
#if UE_EDITOR
   // GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString("Piercing: ") + FString::FromInt(d.piercing));
#endif
#if UE_EDITOR
   // GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString("Damage b4 piercing: ") + FString::FromInt(d.damage));
#endif

   ///--Apply piercing effects
   /// 1 - Increase base damage by 1% per 35 points of piercing
   float percentageConversion = d.piercing / 35.f;
   d.damage                   = d.damage * (100 + percentageConversion) / 100;
   float critRoll             = FMath::FRandRange(1, 100);
#if UE_EDITOR
   // GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("CritPerc! ") + FString::FromInt(GetSkillAdjValue(static_cast<int>(Stats::Critical_Chance)) + percentageConversion));
#endif
   if (critRoll + percentageConversion > 100 - unit->GetSkillAdjValue(static_cast<int>(UnitStats::Critical_Chance))) // piercing will increase crit chance as well
   {
      d.crit = true;
#if UE_EDITOR
      // GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Crit!") + FString::FromInt(d.damage));
#endif
      d.damage = d.damage * unit->GetSkillAdjValue(static_cast<int>(UnitStats::Critical_Damage));
   } else
      d.crit = false;
      ///--End of piercing application
#if UE_EDITOR
      // GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Damage: ") + FString::FromInt(d.damage));
#endif

   ///--True accuracy is 1% extra per 35 points.
   d.accuracy = d.accuracy / 35.f;
   d.accuracy = FMath::RandRange(0, 100) + unit->GetSkillAdjValue(static_cast<int>(UnitStats::Dodge)) - d.accuracy; // accuracy now becomes dodge roll

   ReceiveEffects(unit, d, effects);
}

void UDamageCalculation::CalculatePiercing(AUnit* unit, Damage& d, bool isAtt) const
{
   // apply effects from buffs and weapons
   if (d.element.GetTagName() == "Combat.Element.Arcane") {
      d.piercing += unit->GetSkillAdjValue(static_cast<int>(UnitStats::Arcane_Aff)) * isAtt - !isAtt * unit->GetSkillAdjValue(static_cast<int>(UnitStats::Arcane_Resist));
   } else if (d.element.GetTagName() == "Combat.Element.Blood") {
      d.piercing += unit->GetSkillAdjValue(static_cast<int>(UnitStats::Blood_Aff)) * isAtt - !isAtt * unit->GetSkillAdjValue(static_cast<int>(UnitStats::Blood_Resist));
   } else if (d.element.GetTagName() == "Combat.Element.Chaos") {
      d.piercing += unit->GetSkillAdjValue(static_cast<int>(UnitStats::Chaos_Aff)) * isAtt - !isAtt * unit->GetSkillAdjValue(static_cast<int>(UnitStats::Chaos_Resist));
   } else if (d.element.GetTagName() == "Combat.Element.Dark") {
      d.piercing += unit->GetSkillAdjValue(static_cast<int>(UnitStats::Darkness_Aff)) * isAtt - !isAtt * unit->GetSkillAdjValue(static_cast<int>(UnitStats::Darkness_Resist));
   } else if (d.element.GetTagName() == "Combat.Element.Earth") {
      d.piercing += unit->GetSkillAdjValue(static_cast<int>(UnitStats::Earth_Aff)) * isAtt - !isAtt * unit->GetSkillAdjValue(static_cast<int>(UnitStats::Earth_Resist));
   } else if (d.element.GetTagName() == "Combat.Element.Electric") {
      d.piercing += unit->GetSkillAdjValue(static_cast<int>(UnitStats::Electric_Aff)) * isAtt - !isAtt * unit->GetSkillAdjValue(static_cast<int>(UnitStats::Electric_Resist));
   } else if (d.element.GetTagName() == "Combat.Element.Ethereal") {
      d.piercing += unit->GetSkillAdjValue(static_cast<int>(UnitStats::Ethereal_Aff)) * isAtt - !isAtt * unit->GetSkillAdjValue(static_cast<int>(UnitStats::Ethereal_Resist));
   } else if (d.element.GetTagName() == "Combat.Element.Fire") {
      d.piercing += unit->GetSkillAdjValue(static_cast<int>(UnitStats::Fire_Aff)) * isAtt - !isAtt * unit->GetSkillAdjValue(static_cast<int>(UnitStats::Fire_Resist));
   } else if (d.element.GetTagName() == "Combat.Element.Force") {
      d.piercing += unit->GetSkillAdjValue(static_cast<int>(UnitStats::Physical_Aff)) * isAtt - !isAtt * unit->GetSkillAdjValue(static_cast<int>(UnitStats::Physical_Resist));
   } else if (d.element.GetTagName() == "Combat.Element.Light") {
      d.piercing += unit->GetSkillAdjValue(static_cast<int>(UnitStats::Light_Aff)) * isAtt - !isAtt * unit->GetSkillAdjValue(static_cast<int>(UnitStats::Light_Resist));
   } else if (d.element.GetTagName() == "Combat.Element.Poison") {
      d.piercing += unit->GetSkillAdjValue(static_cast<int>(UnitStats::Poison_Aff)) * isAtt - !isAtt * unit->GetSkillAdjValue(static_cast<int>(UnitStats::Poison_Resist));
   } else if (d.element.GetTagName() == "Combat.Element.Water") {
      d.piercing += unit->GetSkillAdjValue(static_cast<int>(UnitStats::Water_Aff)) * isAtt - !isAtt * unit->GetSkillAdjValue(static_cast<int>(UnitStats::Water_Resist));
   } else if (d.element.GetTagName() == "Combat.Element.Wind") {
      d.piercing += unit->GetSkillAdjValue(static_cast<int>(UnitStats::Wind_Aff)) * isAtt - !isAtt * unit->GetSkillAdjValue(static_cast<int>(UnitStats::Wind_Resist));
   }
}
