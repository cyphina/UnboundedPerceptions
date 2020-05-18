// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UpDamageComponent.h"

#include "Unit.h"
#include "DamageStructs.h"

#include "GameplayTagContainer.h"

#include "AbilitySystemComponent.h"
#include "AIStuff/AIControllers/UnitController.h"

DECLARE_LOG_CATEGORY_CLASS(UpDamageSystem, Verbose, All);

static TAutoConsoleVariable<bool> CVARDamageDebugging(TEXT("debug.DamageDebugging"), false, TEXT("Used to toggle on or off logging for damage system."),
                                                      ECVF_SetByConsole);

UUpDamageComponent::UUpDamageComponent()
{
   PrimaryComponentTick.bCanEverTick = false;
   bNeverNeedsRenderUpdate           = false;
   bCanEverAffectNavigation          = false;
}

void UUpDamageComponent::BeginPlay()
{
   Super::BeginPlay();
}

// See comments for UnitDamageTarget below
void UUpDamageComponent::DamageTarget(FUpDamage& d, FGameplayTagContainer effects) const
{
   // No need to calculate piercing from the source since it is not a unit
   ApplyEffects(d, effects);
   CalculateDamageReduction(d, effects);

   // Accuracy check
   if(d.accuracy > 100) {
      d.targetUnit->damageComponent->OnDamageTaken.Broadcast(d);
      return;
   }

   // Clamp damage
   if(d.damage <= 0)
      d.damage = 1;

   // Store timestamp of when damage was taken for informative purposes
   float worldTime = d.targetUnit->GetWorld()->GetTimeSeconds();
   if(!effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Healing"))) {
      d.targetUnit->ModifyStats<EVitals::Health>(d.targetUnit->GetVitalCurValue(EVitals::Health) - d.damage);
      d.targetUnit->combatParams.damageRecieved.Insert(TPair<int, float>(d.damage, worldTime));
   } else {
      d.targetUnit->ModifyStats<EVitals::Health>(d.targetUnit->GetVitalCurValue(EVitals::Health) + d.damage);
      d.targetUnit->combatParams.healingRecieved.Insert(TPair<int, float>(d.damage, worldTime));
   }

   // Kill the unit if its hp falls below 0, else update information
   if(d.targetUnit->GetVitalCurValue(EVitals::Health) <= 0) {
      if(!d.targetUnit->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Buff.Immortality"))) {
         d.targetUnit->Die();
      }
   }

   d.targetUnit->damageComponent->OnDamageTaken.Broadcast(d);
}

void UUpDamageComponent::UnitDamageTarget(FUpDamage& d, FGameplayTagContainer effects) const
{
   CalculateDamage(d, effects);          // Calculate how much damage we do based on all of our stats
   CalculateDamageReduction(d, effects); // Calculate how much our damage is resisted based on target's stats

   // Accuracy check
   if(d.accuracy > 100) {
      d.targetUnit->damageComponent->OnDamageTaken.Broadcast(d);
      return;
   }

   // Clamp damage to always deal 1 damage even on highly resisted hits
   if(d.damage <= 0)
      d.damage = 1;

   // Add lifesteal effects as healing here (since we have to calculate damage reduction first)
   // TODO: Maybe add a stat for lifesteal %
   if(effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Lifesteal")))
      d.sourceUnit->ModifyStats<EVitals::Health>(d.targetUnit->GetVitalCurValue(EVitals::Health) + d.damage);

   // Record some statistics about our units which AI can use
   float worldTime = d.sourceUnit->GetWorld()->GetTimeSeconds();

   if(LIKELY(!effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Healing")))) {
      d.targetUnit->ModifyStats<EVitals::Health>(d.targetUnit->GetVitalCurValue(EVitals::Health) - d.damage);
      d.sourceUnit->combatParams.damageDealt.Insert(TPair<int, float>(d.damage, worldTime));
      d.targetUnit->combatParams.damageRecieved.Insert(TPair<int, float>(d.damage, worldTime));
   } else {
      d.targetUnit->ModifyStats<EVitals::Health>(d.targetUnit->GetVitalCurValue(EVitals::Health) + d.damage);
      d.sourceUnit->combatParams.healingDealt.Insert(TPair<int, float>(d.damage, worldTime));
      d.targetUnit->combatParams.healingRecieved.Insert(TPair<int, float>(d.damage, worldTime));
   }

   // Kill the unit if it's health drops below 0 and stop us from attacking anymore (if we were autoattacking)
   if(d.targetUnit->GetVitalCurValue(EVitals::Health) <= 0) {
      if(!d.targetUnit->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Buff.Immortality"))) {
         d.targetUnit->Die();
         d.sourceUnit->GetUnitController()->Stop();
      }
   }

   d.targetUnit->damageComponent->OnDamageTaken.Broadcast(d);
}

void UUpDamageComponent::CalculateDamage(FUpDamage& d, FGameplayTagContainer& effects) const
{
   const int damageRange = FMath::RandRange(75, 105);
   d.damage              = d.damage * (damageRange / 100.f);

   if(CVARDamageDebugging.GetValueOnGameThread()) {
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString::Printf(TEXT("Printing information for unit %s"), *d.sourceUnit->GetName()));
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString::Printf(TEXT("Rolled %d in damage roll"), damageRange));
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString::Printf(TEXT("Unit %s did %d damage!"), *d.sourceUnit->GetGameName().ToString(), d.damage));
   }

   d.piercing = 0;
   d.accuracy = d.sourceUnit->GetSkillAdjValue(EUnitScalingStats::Accuracy);

   ApplyEffects(d, effects);
   CalculatePiercing(d.sourceUnit, d, true); // Caclulate offensive piercing
}

void UUpDamageComponent::ApplyEffects(FUpDamage& d, FGameplayTagContainer& effects) const
{
}

void UUpDamageComponent::ReceiveEffects(FUpDamage& d, FGameplayTagContainer& effects) const
{
   // Apply effects from buffs and armor
   // --This tag means this damage is pure, that is it doesn't get reduced by the global damage modifier--  Ex: Iron Maiden
   if(!effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Absolute")))
      d.damage = d.damage * (100 - d.targetUnit->GetMechanicAdjValue(EMechanics::GlobalDamageModifier)) / 100;

   // This tag that means the ability triggering this damage event can never miss
   if(effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.NeverMiss")))
      d.accuracy = 0;

   // This tag means that the target unit is a ward and can only take 1 damage maximum
   if(d.targetUnit->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Ward")))
      d.damage = -1;

   // This tag means that
   if(d.targetUnit->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Buff.GodMode")))
      d.damage = 0;
}

void UUpDamageComponent::CalculateDamageReduction(FUpDamage& d, FGameplayTagContainer& effects) const
{
   CalculatePiercing(d.targetUnit, d, false); // Calculate defensive piercing

   if(CVARDamageDebugging.GetValueOnGameThread()) {
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString("Piercing value: ") + FString::FromInt(d.piercing));
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString("Damage b4 piercing: ") + FString::FromInt(d.damage));
   }

   ///--After we successfully calculate piercing, apply piercing effects
   /// 1 - Increase base damage by 1% per 35 points of piercing
   float percentageConversion = d.piercing / 35.f;
   d.damage                   = d.damage * (100 + percentageConversion) / 100;
   float critRoll             = FMath::FRandRange(1, 100);

   if(CVARDamageDebugging.GetValueOnGameThread()) {
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,
                                       FString("Calculated CritPerc! ") +
                                           FString::FromInt(d.sourceUnit->GetSkillAdjValue(EUnitScalingStats::Critical_Chance) + percentageConversion));
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Crit roll perc = ") + FString::SanitizeFloat(critRoll + percentageConversion));
   }

   /// 2 - Increase critical chance
   if(critRoll + percentageConversion > 100 - d.sourceUnit->GetSkillAdjValue(EUnitScalingStats::Critical_Chance)) // piercing will increase crit chance as well
   {
      d.crit = true;

      // GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Crit!") + FString::FromInt(d.damage));

      d.damage = d.damage * d.sourceUnit->GetSkillAdjValue(EUnitScalingStats::Critical_Damage);
   } else
      d.crit = false;
   ///--End of piercing application

   if(CVARDamageDebugging.GetValueOnGameThread())
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Final Damage: ") + FString::FromInt(d.damage));

   /// 3 - Calculate accuracy based on our accuracy and the opponent's dodge --.
   d.accuracy = d.accuracy / 35.f;
   d.accuracy = FMath::RandRange(0, 100) + d.sourceUnit->GetSkillAdjValue(EUnitScalingStats::Dodge) - d.accuracy; // accuracy now becomes dodge roll

   /// 4 - Apply any effects that need everything properly calculated
   ReceiveEffects(d, effects);
}

void UUpDamageComponent::CalculatePiercing(AUnit* unit, FUpDamage& d, bool isAtt) const
{
   // Apply effects from buffs and weapons
   if(d.element.GetTagName() == "Combat.Element.Arcane") {
      d.piercing += unit->GetSkillAdjValue(EUnitScalingStats::Arcane_Aff) * isAtt - !isAtt * unit->GetSkillAdjValue(EUnitScalingStats::Arcane_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Blood") {
      d.piercing += unit->GetSkillAdjValue(EUnitScalingStats::Blood_Aff) * isAtt - !isAtt * unit->GetSkillAdjValue(EUnitScalingStats::Blood_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Chaos") {
      d.piercing += unit->GetSkillAdjValue(EUnitScalingStats::Chaos_Aff) * isAtt - !isAtt * unit->GetSkillAdjValue(EUnitScalingStats::Chaos_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Dark") {
      d.piercing += unit->GetSkillAdjValue(EUnitScalingStats::Darkness_Aff) * isAtt - !isAtt * unit->GetSkillAdjValue(EUnitScalingStats::Darkness_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Earth") {
      d.piercing += unit->GetSkillAdjValue(EUnitScalingStats::Earth_Aff) * isAtt - !isAtt * unit->GetSkillAdjValue(EUnitScalingStats::Earth_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Electric") {
      d.piercing += unit->GetSkillAdjValue(EUnitScalingStats::Electric_Aff) * isAtt - !isAtt * unit->GetSkillAdjValue(EUnitScalingStats::Electric_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Ethereal") {
      d.piercing += unit->GetSkillAdjValue(EUnitScalingStats::Ethereal_Aff) * isAtt - !isAtt * unit->GetSkillAdjValue(EUnitScalingStats::Ethereal_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Fire") {
      d.piercing += unit->GetSkillAdjValue(EUnitScalingStats::Fire_Aff) * isAtt - !isAtt * unit->GetSkillAdjValue(EUnitScalingStats::Fire_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Force") {
      d.piercing += unit->GetSkillAdjValue(EUnitScalingStats::Physical_Aff) * isAtt - !isAtt * unit->GetSkillAdjValue(EUnitScalingStats::Physical_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Light") {
      d.piercing += unit->GetSkillAdjValue(EUnitScalingStats::Light_Aff) * isAtt - !isAtt * unit->GetSkillAdjValue(EUnitScalingStats::Light_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Poison") {
      d.piercing += unit->GetSkillAdjValue(EUnitScalingStats::Poison_Aff) * isAtt - !isAtt * unit->GetSkillAdjValue(EUnitScalingStats::Poison_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Water") {
      d.piercing += unit->GetSkillAdjValue(EUnitScalingStats::Water_Aff) * isAtt - !isAtt * unit->GetSkillAdjValue(EUnitScalingStats::Water_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Wind") {
      d.piercing += unit->GetSkillAdjValue(EUnitScalingStats::Wind_Aff) * isAtt - !isAtt * unit->GetSkillAdjValue(EUnitScalingStats::Wind_Resist);
   }
}
