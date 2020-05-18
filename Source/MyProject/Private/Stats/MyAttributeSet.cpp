// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "MyAttributeSet.h"
#include "GameplayEffect.h"
#include "WorldObjects/Unit.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "NumericLimits.h"

TArray<FGameplayAttribute> UMyAttributeSet::attList = TArray<FGameplayAttribute>();
TSet<FGameplayAttribute>   UMyAttributeSet::attSet  = TSet<FGameplayAttribute>();

const float UMyAttributeSet::MAX_HEALTH = TNumericLimits<int>().Max();

UMyAttributeSet::UMyAttributeSet()
{
   attList = {StrengthAttribute(),  UnderstandingAttribute(), IntelligenceAttribute(), ExplosivenessAttribute(),
              EnduranceAttribute(), AgilityAttribute(),       LuckAttribute()};
   attSet  = TSet<FGameplayAttribute>(attList);
}

// check after gameplay effect occurs
void UMyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& data)
{
   UAbilitySystemComponent* source = data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();

   // This logic is laready handled by the damage component
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

DEFINE_ATTRIBUTE_FUNCTION(Strength, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Intelligence, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Agility, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Understanding, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Endurance, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Luck, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Explosiveness, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Critical_Chance, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Critical_Damage, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Accuracy, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Dodge, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Attack_Speed, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Cast_Speed, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Physical_Aff, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Fire_Aff, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Water_Aff, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Wind_Aff, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Earth_Aff, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Electric_Aff, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Darkness_Aff, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Light_Aff, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Arcane_Aff, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Chaos_Aff, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Poison_Aff, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Blood_Aff, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Ethereal_Aff, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Physical_Resist, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Fire_Resist, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Water_Resist, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Wind_Resist, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Earth_Resist, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Electric_Resist, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Darkness_Resist, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Light_Resist, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Arcane_Resist, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Chaos_Resist, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Poison_Resist, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Blood_Resist, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Ethereal_Resist, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Health, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Mana, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Psyche, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Moxie, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(Shield, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(MovementSpeed, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(AttackRange, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(WeaponPower, UMyAttributeSet);
DEFINE_ATTRIBUTE_FUNCTION(GlobalDamageModifier, UMyAttributeSet);

TArray<FGameplayAttribute> UMyAttributeSet::GetAtts()
{
   TArray<FGameplayAttribute> atts = {StrengthAttribute(), UnderstandingAttribute(), IntelligenceAttribute(), ExplosivenessAttribute(),
                                     EnduranceAttribute(),  AgilityAttribute(),       LuckAttribute()};
   return atts;
}

TArray<FGameplayAttribute> UMyAttributeSet::GetSkills()
{
   TArray<FGameplayAttribute> skills = {Critical_ChanceAttribute(), Critical_DamageAttribute(), AccuracyAttribute(),     DodgeAttribute(),           Attack_SpeedAttribute(),    Cast_SpeedAttribute(),      Physical_AffAttribute(),
                                       Fire_AffAttribute(),        Water_AffAttribute(),       Wind_AffAttribute(),     Earth_AffAttribute(),       Electric_AffAttribute(),    Darkness_AffAttribute(),    Light_AffAttribute(),
                                       Arcane_AffAttribute(),      Chaos_AffAttribute(),       Poison_AffAttribute(),   Blood_AffAttribute(),       Ethereal_AffAttribute(),    Physical_ResistAttribute(), Fire_ResistAttribute(),
                                       Water_ResistAttribute(),    Wind_ResistAttribute(),     Earth_ResistAttribute(), Electric_ResistAttribute(), Darkness_ResistAttribute(), Light_ResistAttribute(),    Arcane_ResistAttribute(),
                                       Chaos_ResistAttribute(),    Poison_ResistAttribute(),   Blood_ResistAttribute(), Ethereal_ResistAttribute()};
   return skills;
}

TArray<FGameplayAttribute> UMyAttributeSet::GetVitals()
{
   TArray<FGameplayAttribute> vits = {HealthAttribute(), ManaAttribute(), PsycheAttribute(), MoxieAttribute(), ShieldAttribute()};
   return vits;
}

TArray<FGameplayAttribute> UMyAttributeSet::GetMechanics()
{
   TArray<FGameplayAttribute> mechs = {MovementSpeedAttribute(), AttackRangeAttribute(), WeaponPowerAttribute(), GlobalDamageModifierAttribute()};
   return mechs;
}

FGameplayAttribute UMyAttributeSet::IndexAtts(int index)
{
   return attList[index];
}

void UMyAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
   AUnit* unit = Cast<AUnit>(GetOwningActor());
   if(attSet.Contains(Attribute)) {
      unit->UpdateStats(Attribute);
   }
   baseStatUpdatedEvent.Broadcast(Attribute, NewValue, unit);
}

void UMyAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
   // Clamp values and update walk speed if we get movement speed changes
   AUnit* unit = Cast<AUnit>(GetOwningActor());
   if(Attribute == HealthAttribute()) {
      Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.0f, Health.GetBaseValue()));
   } else if(Attribute == MovementSpeedAttribute()) {
      unit->GetCharacterMovement()->MaxWalkSpeed = NewValue;
   } else if(attSet.Contains(Attribute)) {
      unit->UpdateStats(Attribute);
   }

   statUpdatedEvent.Broadcast(Attribute, NewValue, unit);
}
