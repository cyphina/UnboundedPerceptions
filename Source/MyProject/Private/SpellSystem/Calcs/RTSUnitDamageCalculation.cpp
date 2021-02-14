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
   static FAutoConsoleVariableRef CVarUseOldDamageNumbers(TEXT("useOldDamageNumbers"), bUseOldDamageNumbers,
                                                          TEXT("Uses text render component instead of widgets to show damage numbers."));
}

void URTSUnitDamageCalculation::Execute_Implementation
(const FGameplayEffectCustomExecutionParameters& executionParams,
 FGameplayEffectCustomExecutionOutput&           outExecutionOutput) const
{
   FUpDamage damage;

   // Owner component gotten from instigator ability component which gotten from owner we set as our player controller but that isn't an ability system holder
   UAbilitySystemComponent* ownerComponent  = executionParams.GetSourceAbilitySystemComponent();
   UAbilitySystemComponent* targetComponent = executionParams.GetTargetAbilitySystemComponent();
   AUnit*                   sourceUnit      = nullptr,* targetUnit = nullptr;

   // TODO: Handle non unit damage (from traps or something)
   // If our components exist
   if(ownerComponent)
   {
      sourceUnit = Cast<AUnit>(ownerComponent->AvatarActor);
   }
   if(targetComponent)
   {
      targetUnit = Cast<AUnit>(targetComponent->AvatarActor); // a null pointer.
   }

   if(sourceUnit && targetUnit)
   {
      damage.sourceUnit = sourceUnit;
      damage.targetUnit = targetUnit;

      FGameplayTagContainer tags = executionParams.GetOwningSpec().DynamicAssetTags;
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

   // Add lifesteal effects as healing here (since we have to calculate damage reduction first)
   // TODO: Maybe add a stat for lifesteal %
   if(effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Lifesteal")))
   {
      d.sourceUnit->GetStatComponent()->ModifyStats<false>(d.targetUnit->GetStatComponent()->GetVitalCurValue(EVitals::Health) + d.damage, EVitals::Health);
   }

   // Record some statistics about our units which AI can use
   const float worldTime = d.sourceUnit->GetStatComponent()->GetWorld()->GetTimeSeconds();

   // Drain or add health depending on healing or damage
   if(UNLIKELY(d.effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Healing"))))
   {
      d.damage *= -1;
   }

   if(AttackCVars::bUseOldDamageNumbers)
   {
      ShowDamageDealt(d);
   }

   BroadcastDamageEvents(d);
}
