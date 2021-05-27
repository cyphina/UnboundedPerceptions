// Created 12/23/20 2:36 PM

#include "MyProject.h"
#include "RTSUnitDamageCalculation.h"

#include "BaseCharacter.h"
#include "SpellDataLibrary.h"
#include "Unit.h"
#include "UnitController.h"
#include "UpStatComponent.h"

namespace AttackCVars
{
   bool                           bUseOldDamageNumbers = false;
   static FAutoConsoleVariableRef CVarUseOldDamageNumbers(TEXT("Up_DebugUseOldDamageNumbers"), bUseOldDamageNumbers,
                                                          TEXT("Uses text render component instead of widgets to show damage numbers."));
}

void URTSUnitDamageCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams,
                                                       FGameplayEffectCustomExecutionOutput&           outExecutionOutput) const
{
   FUpDamage damage;

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
      damage.sourceUnit = sourceUnit;
      damage.targetUnit = targetUnit;

      FGameplayTagContainer tags = executionParams.GetOwningSpec().DynamicGrantedTags;
      damage.element             = tags.Filter(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Element"))).GetByIndex(0);
      damage.effects             = tags;

      // Start calculating damage based off our attributes and the attributes the ability scales off of
      FAggregatorEvaluateParameters evalParams;
      evalParams.SourceTags = executionParams.GetOwningSpec().CapturedSourceTags.GetAggregatedTags();
      evalParams.TargetTags = executionParams.GetOwningSpec().CapturedTargetTags.GetAggregatedTags();

      float statMultiplier;
      for(FGameplayEffectAttributeCaptureDefinition captureDef : RelevantAttributesToCapture)
      {
         if(executionParams.AttemptCalculateCapturedAttributeMagnitude(captureDef, evalParams, statMultiplier))
         {
            // Multiplies the actual value of our stat by the overriden capture value (allowing us to customize the value in the editor)
            const float ownerStat = ownerComponent->GetNumericAttribute(captureDef.AttributeToCapture);
            damage.damage += ownerStat * statMultiplier / 100;
         }
      }
      DamageTarget(damage, tags.Filter(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.DamageEffects"))));
   }
}

void URTSUnitDamageCalculation::DamageTarget(FUpDamage& d, FGameplayTagContainer effects) const
{
   CalculateDamage(d, effects);          // Calculate how much damage we do based on all of our stats
   CalculateDamageReduction(d, effects); // Calculate how much our damage is resisted based on target's stats

   // Accuracy check
   if(d.accuracy > 100)
   {
      BroadcastDamageEvents(d);
      return;
   }

   // Clamp damage to always deal 1 damage even on highly resisted hits
   if(d.damage <= 0)
   {
      d.damage = 1;
   }

   // Drain or add health depending on healing or damage
   if(d.effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Healing")))
   {
      d.damage *= -1;
   }

   if(AttackCVars::bUseOldDamageNumbers)
   {
      ShowDamageDealt(d);
   }

   BroadcastDamageEvents(d);
}
