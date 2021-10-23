// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSAttributeSet.h"
#include "GameplayEffect.h"
#include "WorldObjects/Unit.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "NumericLimits.h"
#include "UpStatComponent.h"

TSet<FGameplayAttribute> URTSAttributeSet::attSet = TSet<FGameplayAttribute>();

URTSAttributeSet::URTSAttributeSet()
{
   attSet = {GetStrengthAttribute(),  GetUnderstandingAttribute(), GetIntelligenceAttribute(), GetExplosivenessAttribute(),
             GetEnduranceAttribute(), GetAgilityAttribute(),       GetLuckAttribute()};
}

void URTSAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& data)
{
   UAbilitySystemComponent* source = data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();

   // This logic is already handled by the damage component
   /*if (HealthAttribute() == data.EvaluatedData.Attribute) {
      AActor*      damagedActor      = nullptr;
      AController* damagedController = nullptr;
      if (data.Target.AbilityActorInfo.IsValid() && data.Target.AbilityActorInfo->AvatarActor.IsValid()) { damagedActor = data.Target.AbilityActorInfo->AvatarActor.Get(); }
      AActor* AttackingActor = nullptr;
      if (source && source->AbilityActorInfo.IsValid() && source->AbilityActorInfo->AvatarActor.IsValid()) { AttackingActor = source->AbilityActorInfo->AvatarActor.Get(); }

      Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.0f, MAX_HEALTH));
      if (Health.GetCurrentValue() <= 0) {
         AUnit* unit = Cast<AUnit>(damagedActor);
         unit->Die();
      }
   }*/
}

const TArray<FGameplayAttribute>& URTSAttributeSet::GetAtts()
{
   static TArray<FGameplayAttribute> atts = {GetStrengthAttribute(),  GetUnderstandingAttribute(), GetIntelligenceAttribute(), GetExplosivenessAttribute(),
                                             GetEnduranceAttribute(), GetAgilityAttribute(),       GetLuckAttribute()};
   return atts;
}

TArray<FGameplayAttribute> URTSAttributeSet::GetSkills()
{
   static TArray<FGameplayAttribute> skills = {
       GetCritical_ChanceAttribute(), GetCritical_DamageAttribute(), GetAccuracyAttribute(),        GetDodgeAttribute(),         GetAttack_SpeedAttribute(),
       GetCast_SpeedAttribute(),      GetPhysical_AffAttribute(),    GetFire_AffAttribute(),        GetWater_AffAttribute(),     GetWind_AffAttribute(),
       GetEarth_AffAttribute(),       GetElectric_AffAttribute(),    GetDarkness_AffAttribute(),    GetLight_AffAttribute(),     GetArcane_AffAttribute(),
       GetChaos_AffAttribute(),       GetPoison_AffAttribute(),      GetBlood_AffAttribute(),       GetEthereal_AffAttribute(),  GetCosmic_AffAttribute(),
       GetPhysical_ResistAttribute(), GetFire_ResistAttribute(),     GetWater_ResistAttribute(),    GetWind_ResistAttribute(),   GetEarth_ResistAttribute(),
       GetElectric_ResistAttribute(), GetDarkness_ResistAttribute(), GetLight_ResistAttribute(),    GetArcane_ResistAttribute(), GetChaos_ResistAttribute(),
       GetPoison_ResistAttribute(),   GetBlood_ResistAttribute(),    GetEthereal_ResistAttribute(), GetCosmic_ResistAttribute()};
   return skills;
}

TArray<FGameplayAttribute> URTSAttributeSet::GetVitals()
{
   static TArray<FGameplayAttribute> vits = {GetHealthAttribute(), GetManaAttribute(), GetPsycheAttribute(), GetMoxieAttribute(), GetShieldAttribute()};
   return vits;
}

TArray<FGameplayAttribute> URTSAttributeSet::GetMechanics()
{
   static TArray<FGameplayAttribute> mechs = {GetMovementSpeedAttribute(), GetAttackRangeAttribute(), GetWeaponPowerAttribute(), GetGlobalDamageModifierAttribute(),
                                              GetHealthRegenModifierAttribute()};
   return mechs;
}

FGameplayAttribute URTSAttributeSet::IndexAtts(int index)
{
   return GetAtts()[index];
}

void URTSAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
   AUnit* unit = Cast<AUnit>(GetOwningActor());
   if(attSet.Contains(Attribute))
   {
      unit->GetStatComponent()->UpdateStats(Attribute);
   }

   baseStatUpdatedEvent.Broadcast(Attribute, NewValue, unit);
}

void URTSAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
   if(AUnit* unit = Cast<AUnit>(GetOwningActor()))
   {
      if(Attribute == GetHealthAttribute() || Attribute == GetManaAttribute())
      {
         FGameplayAttributeData* ChangedVital = Attribute.GetGameplayAttributeData(this);
         NewValue                             = FMath::Clamp(NewValue, 0.0f, ChangedVital->GetBaseValue());
         ChangedVital->SetCurrentValue(NewValue);
      }
      else if(attSet.Contains(Attribute))
      { // Needs to be run before the broadcast to get proper values to the stat widget in the character menu
         unit->GetStatComponent()->UpdateStats(Attribute);
      }
      if(Attribute == GetMovementSpeedAttribute())
      {
         unit->GetCharacterMovement()->MaxWalkSpeed = NewValue;
      }
      statUpdatedEvent.Broadcast(Attribute, NewValue, unit);
   }
}
