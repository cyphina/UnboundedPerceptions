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
   attList = {StrengthAttribute(), UnderstandingAttribute(), IntelligenceAttribute(), ExplosivenessAttribute(), EnduranceAttribute(), AgilityAttribute(), LuckAttribute()};
   attSet  = TSet<FGameplayAttribute>(attList);
}

// check after gameplay effect occurs
void UMyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& data)
{
   UAbilitySystemComponent* source = data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();

   if (HealthAttribute() == data.EvaluatedData.Attribute) {
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
   }
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

TSet<FGameplayAttributeData*> UMyAttributeSet::GetAtts()
{
   TSet<FGameplayAttributeData*> atts = {&Strength, &Understanding, &Intelligence, &Explosiveness, &Endurance, &Agility, &Luck};
   return atts;
}

TSet<FGameplayAttributeData*> UMyAttributeSet::GetSkills()
{
   TSet<FGameplayAttributeData*> skills = {&Critical_Chance, &Critical_Damage, &Accuracy,     &Dodge,           &Attack_Speed, &Cast_Speed,    &Physical_Aff, &Fire_Aff,
                                           &Water_Aff,       &Wind_Aff,        &Earth_Aff,    &Electric_Aff,    &Darkness_Aff, &Light_Aff,     &Arcane_Aff,   &Chaos_Aff,
                                           &Poison_Aff,      &Blood_Aff,       &Ethereal_Aff, &Physical_Resist, &Fire_Resist,  &Water_Resist,  &Wind_Resist,  &Earth_Resist,
                                           &Electric_Resist, &Darkness_Resist, &Light_Resist, &Arcane_Resist,   &Chaos_Resist, &Poison_Resist, &Blood_Resist, &Ethereal_Resist};
   return skills;
}

TSet<FGameplayAttributeData*> UMyAttributeSet::GetVitals()
{
   TSet<FGameplayAttributeData*> vits = {&Health, &Mana, &Psyche, &Moxie, &Shield};
   return vits;
}

TSet<FGameplayAttributeData*> UMyAttributeSet::GetMechanics()
{
   TSet<FGameplayAttributeData*> mechs = {&MovementSpeed, &AttackRange, &WeaponPower, &GlobalDamageModifier};
   return mechs;
}

FGameplayAttribute UMyAttributeSet::IndexAtts(int index)
{
   return attList[index];
}

void UMyAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
   if (Attribute == HealthAttribute()) {
      Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.0f, Health.GetBaseValue()));
   } else if (Attribute == MovementSpeedAttribute()) {
      AUnit* unit                                = Cast<AUnit>(GetOwningActor());
      unit->GetCharacterMovement()->MaxWalkSpeed = NewValue;
   } else if (attSet.Contains(Attribute)) {
      AUnit* unit = Cast<AUnit>(GetOwningActor());
      unit->UpdateStats();
   }
}
