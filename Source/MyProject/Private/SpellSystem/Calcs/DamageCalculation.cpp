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
   // Any damage effect that relies on this calculation should override all these stats to make sure they are modified properly
   AttStruct attributes;
   RelevantAttributesToCapture.Add(attributes.HealthDef); // Some spells deal damage based on how much health we have
   RelevantAttributesToCapture.Add(attributes.StrengthDef);
   RelevantAttributesToCapture.Add(attributes.UnderstandingDef);
   RelevantAttributesToCapture.Add(attributes.IntelligenceDef);
   RelevantAttributesToCapture.Add(attributes.AgilityDef);
}

void UDamageCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams,
                                                FGameplayEffectCustomExecutionOutput&           outExecutionOutput) const
{
   FUpDamage damage;
   // Owner component gotten from instigator ability component which gotten from owner we set as our player controller but that isn't an ability system holder
   UAbilitySystemComponent* ownerComponent  = executionParams.GetSourceAbilitySystemComponent();
   UAbilitySystemComponent* targetComponent = executionParams.GetTargetAbilitySystemComponent();
   AUnit *                  sourceUnit = nullptr, *targetUnit = nullptr;
   // if our components exist
   if(ownerComponent)
      sourceUnit = Cast<AUnit>(
          ownerComponent
              ->AvatarActor); // If our AbilitySystemComponents are valid, we get each their owning actors and put them in variables. This is mostly to prevent crashing
                              // by trying to get the AvatarActor variable from
   if(targetComponent)
      targetUnit = Cast<AUnit>(targetComponent->AvatarActor); // a null pointer.

   if(sourceUnit && targetUnit) {
      // Setup targteting
      damage.sourceUnit = sourceUnit;
      damage.targetUnit = targetUnit;

      // Get Element of this Damage
      FGameplayTagContainer tags = executionParams.GetOwningSpec().DynamicAssetTags;
      damage.element             = tags.Filter(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Element"))).GetByIndex(0);

      // Start calculating damage based off our attributes and the attributes the abiliy scales off of
      FAggregatorEvaluateParameters evalParams;
      evalParams.SourceTags = executionParams.GetOwningSpec().CapturedSourceTags.GetAggregatedTags();
      evalParams.TargetTags = executionParams.GetOwningSpec().CapturedTargetTags.GetAggregatedTags();

      AttStruct attributes;
      float     captureVal;
      for(FGameplayEffectAttributeCaptureDefinition captureDef : RelevantAttributesToCapture) {
         captureVal = 0;
         if(executionParams.AttemptCalculateCapturedAttributeMagnitude(captureDef, evalParams, captureVal))
            // Multiplies the actual value of our stat by the overriden capture value (allowing us to customize the value in the editor)
            damage.damage += ownerComponent->GetNumericAttribute(captureDef.AttributeToCapture) * captureVal / 100;
      }
      sourceUnit->damageComponent->UnitDamageTarget(damage, tags.Filter(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.DamageEffects"))));
   }
}
