// Created 12/23/20 4:34 AM

#include "MyProject.h"
#include "RTSEventDamageCalculation.h"

#include "BaseCharacter.h"
#include "SpellDataLibrary.h"
#include "Unit.h"
#include "UnitController.h"
#include "UpStatComponent.h"

void URTSEventDamageCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams,
                                                     FGameplayEffectCustomExecutionOutput&           outExecutionOutput) const
{

}

void URTSEventDamageCalculation::DamageTarget(FUpDamage& d, FGameplayTagContainer effects) const
{
   // No need to calculate piercing from the source since it is not a unit
   ApplyEffects(d, effects);
   CalculateDamageReduction(d, effects);

   // Accuracy check
   if(d.accuracy > 100) {
      BroadcastDamageEvents(d);
      return;
   }

   // Clamp damage
   if(d.damage <= 0) d.damage = 1;

   // Kill the unit if its hp falls below 0, else update information
   if(d.targetUnit->GetStatComponent()->GetVitalCurValue(EVitals::Health) <= 0) {
      if(USpellDataLibrary::IsImmortal(d.targetUnit->GetAbilitySystemComponent())) { d.targetUnit->GetUnitController()->Die(); }
   }

   BroadcastDamageEvents(d);
}
