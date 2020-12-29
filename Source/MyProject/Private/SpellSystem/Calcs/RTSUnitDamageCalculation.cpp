// Created 12/23/20 2:36 PM

#include "MyProject.h"
#include "RTSUnitDamageCalculation.h"

#include "BaseCharacter.h"
#include "SpellDataLibrary.h"
#include "Unit.h"
#include "UnitController.h"
#include "UpStatComponent.h"

void URTSUnitDamageCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams,
                                                       FGameplayEffectCustomExecutionOutput&           outExecutionOutput)
{
   FUpDamage damage;

   // Owner component gotten from instigator ability component which gotten from owner we set as our player controller but that isn't an ability system holder
   UAbilitySystemComponent* ownerComponent  = executionParams.GetSourceAbilitySystemComponent();
   UAbilitySystemComponent* targetComponent = executionParams.GetTargetAbilitySystemComponent();
   AUnit *                  sourceUnit = nullptr, *targetUnit = nullptr;

   // TODO: Handle non unit damage (from traps or something)
   // If our components exist
   if(ownerComponent)
      sourceUnit = Cast<AUnit>(
          ownerComponent
              ->AvatarActor); // If our AbilitySystemComponents are valid, we get each their owning actors and put them in variables. This is mostly to prevent crashing
                              // by trying to get the AvatarActor variable from
   if(targetComponent) targetUnit = Cast<AUnit>(targetComponent->AvatarActor); // a null pointer.

   if(sourceUnit && targetUnit) {
      damage.sourceUnit = sourceUnit;
      damage.targetUnit = targetUnit;

      FGameplayTagContainer tags = executionParams.GetOwningSpec().DynamicAssetTags;
      damage.element             = tags.Filter(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Element"))).GetByIndex(0);

      // Start calculating damage based off our attributes and the attributes the ability scales off of
      FAggregatorEvaluateParameters evalParams;
      evalParams.SourceTags = executionParams.GetOwningSpec().CapturedSourceTags.GetAggregatedTags();
      evalParams.TargetTags = executionParams.GetOwningSpec().CapturedTargetTags.GetAggregatedTags();

      float captureVal = 0;

      for(FGameplayEffectAttributeCaptureDefinition captureDef : RelevantAttributesToCapture) {
         if(executionParams.AttemptCalculateCapturedAttributeMagnitude(captureDef, evalParams, captureVal))
            // Multiplies the actual value of our stat by the overriden capture value (allowing us to customize the value in the editor)
            damage.damage += ownerComponent->GetNumericAttribute(captureDef.AttributeToCapture) * captureVal / 100;
      }
      DamageTarget(damage, tags.Filter(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.DamageEffects"))));
   }
}

void URTSUnitDamageCalculation::DamageTarget(FUpDamage& d, FGameplayTagContainer effects) const
{
   CalculateDamage(d, effects);          // Calculate how much damage we do based on all of our stats
   CalculateDamageReduction(d, effects); // Calculate how much our damage is resisted based on target's stats

   // Accuracy check
   if(d.accuracy > 100) {
      BroadcastDamageEvents(d);
      return;
   }

   // Clamp damage to always deal 1 damage even on highly resisted hits
   if(d.damage s <= 0) d.damage = 1;

   // Add lifesteal effects as healing here (since we have to calculate damage reduction first)
   // TODO: Maybe add a stat for lifesteal %
   if(effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Lifesteal")))
      d.sourceUnit->GetStatComponent()->ModifyStats<false>(d.targetUnit->GetStatComponent()->GetVitalCurValue(EVitals::Health) + d.damage, EVitals::Health);

   // Record some statistics about our units which AI can use
   float worldTime = d.sourceUnit->GetStatComponent()->GetWorld()->GetTimeSeconds();

   // Drain or add health depending on healing or damage
   if(UNLIKELY(!d.effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Healing")))) {
      d.damage *= -1;

      // Kill the unit if it's health drops below 0 and stop us from attacking anymore (if we were auto attacking)
      // TODO: Change this to a delegate on UnitController
      if(d.targetUnit->GetStatComponent()->GetVitalCurValue(EVitals::Health) <= 0) {
         if(!USpellDataLibrary::IsImmortal(*d.targetUnit->GetAbilitySystemComponent())) { d.targetUnit->GetUnitController()->Die(); }
      }

      BroadcastDamageEvents(d);
   }
}
