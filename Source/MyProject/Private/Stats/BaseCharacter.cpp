#include "MyProject.h"
#include "BaseCharacter.h"

#include "Copy.h"

FBaseCharacter::FBaseCharacter(const UMyAttributeSet& AttSet)
{
   level  = 1;
   attSet = &const_cast<UMyAttributeSet&>(AttSet);

   SetupPrimaryAttributes();
   SetupSkills();
   SetupVitals();
   SetupMechanics();
   InitializeAttributeBaseValues();
   InitialStatUpdate();

   // Make sure all vitals are full initially
   for(int i = 0; i < vitals.Num(); i++)
      vitals[i].SetAdjustedValue(vitals[i].GetBaseValue(attSet), attSet);
}

FBaseCharacter::~FBaseCharacter()
{
}

void FBaseCharacter::SetupPrimaryAttributes()
{
   for(const FGameplayAttribute& att : attSet->GetAtts())
   {
      baseAttributes.Add(att);
   }
}

void FBaseCharacter::InitialStatUpdate()
{
   for(auto& skill : skills)
   {
      skill.Update(attSet);
   }
   for(auto& vit : vitals)
   {
      vit.Update(attSet);
   }
}

void FBaseCharacter::InitializeAttributeBaseValues()
{
   static float minVal = 20;
   for(FGameplayAttribute& att : baseAttributes)
   {
      att.GetGameplayAttributeData(attSet)->SetBaseValue(minVal);
      att.SetNumericValueChecked(minVal, attSet);
   }

   for(auto& vital : vitals)
   {
      vital.SetAdjustedValue(vital.GetBaseValue(attSet), attSet);
   }
}

void FBaseCharacter::SetupSkills()
{
   for(FGameplayAttribute skillData : attSet->GetSkills())
   {
      skills.Add(RTSUnitStat(skillData));
   }
   SetupSkillModifiers();
}

void FBaseCharacter::SetupVitals()
{
   for(FGameplayAttribute vitData : attSet->GetVitals())
   {
      vitals.Add(Vital(vitData));
   }
   SetupVitalModifiers();
}

void FBaseCharacter::SetupMechanics()
{
   for(FGameplayAttribute mechData : attSet->GetMechanics())
   {
      mechanics.Add(mechData);
   }

   mechanics[static_cast<int>(mech::AttackRange)].GetGameplayAttributeData(attSet)->SetBaseValue(BASE_ATTACK_RANGE);
   mechanics[static_cast<int>(mech::MovementSpeed)].GetGameplayAttributeData(attSet)->SetBaseValue(BASE_MOVEMENT_SPEED);
   mechanics[static_cast<int>(mech::WeaponPower)].GetGameplayAttributeData(attSet)->SetBaseValue(BASE_ATTACK_POWER);
   mechanics[static_cast<int>(mech::GlobalDamageModifier)].GetGameplayAttributeData(attSet)->SetBaseValue(0);
   mechanics[static_cast<int>(mech::AttackRange)].GetGameplayAttributeData(attSet)->SetCurrentValue(BASE_ATTACK_RANGE);
   mechanics[static_cast<int>(mech::MovementSpeed)].GetGameplayAttributeData(attSet)->SetCurrentValue(BASE_MOVEMENT_SPEED);
   mechanics[static_cast<int>(mech::WeaponPower)].GetGameplayAttributeData(attSet)->SetCurrentValue(BASE_ATTACK_POWER);
   mechanics[static_cast<int>(mech::GlobalDamageModifier)].GetGameplayAttributeData(attSet)->SetCurrentValue(0);
}

void FBaseCharacter::SetupSkillModifiers()
{
   // Understanding
   ChangeModifier(sks::Fire_Aff, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Water_Aff, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Wind_Aff, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Earth_Aff, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Electric_Aff, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Darkness_Aff, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Light_Aff, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Arcane_Aff, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Chaos_Aff, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Poison_Aff, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Blood_Aff, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Ethereal_Aff, atts::Understanding, [](int x) { return 2 * x; });

   ChangeModifier(sks::Fire_Resist, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Water_Resist, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Wind_Resist, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Earth_Resist, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Electric_Resist, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Darkness_Resist, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Light_Resist, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Arcane_Resist, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Chaos_Resist, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Poison_Resist, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Blood_Resist, atts::Understanding, [](int x) { return 2 * x; });
   ChangeModifier(sks::Ethereal_Resist, atts::Understanding, [](int x) { return 2 * x; });

   ChangeModifier(sks::Attack_Speed, atts::Explosiveness, [](int x) { return static_cast<int>(.2f * x); }); // attack speed also depends on BAT which depends on weapon
   ChangeModifier(sks::Cast_Speed, atts::Explosiveness, [](int x) { return static_cast<int>(.2f * x); });
   ChangeModifier(sks::Dodge, atts::Explosiveness, [](int x) { return static_cast<int>(.2f * x); });
   ChangeModifier(sks::Physical_Aff, atts::Explosiveness, [](int x) { return 2 * x; });

   // Endurance

   ChangeModifier(sks::Physical_Resist, atts::Endurance, [](int x) { return 2 * x; });

   // Luck

   ChangeModifier(sks::Critical_Chance, atts::Luck, [](int x) { return static_cast<int>(.1 * x); });
   ChangeModifier(sks::Critical_Damage, atts::Luck, [](int x) { return static_cast<int>(.2 * x); });
   ChangeModifier(sks::Accuracy, atts::Luck, [](int x) { return 2 * x; });
}

void FBaseCharacter::SetupVitalModifiers()
{
   ChangeModifier(vits::Health, atts::Endurance, [](int x) { return 10 * x; });
   ChangeModifier(vits::Mana, atts::Intelligence, [](int x) { return 5 * x; });
   ChangeModifier(vits::Psyche, atts::Understanding, [](int x) { return 3 * x; });
   ChangeModifier(vits::Moxie, atts::Luck, [](int x) { return 100; }); // 100 basevalue - only affected by equips
   ChangeModifier(vits::Shield, atts::Luck, [](int x) { return 0; });  // 0 basevalue - isn't affected by baseAttributes
}

void FBaseCharacter::ChangeModifier(sks skillName, atts att, AttributeModifierFunction eff)
{
   skills[static_cast<int>(skillName)].ChangeModifier(ModifyingAttribute(&baseAttributes[static_cast<int>(att)], eff));
}

void FBaseCharacter::ChangeModifier(vits vitalName, atts att, AttributeModifierFunction eff)
{
   vitals[static_cast<int>(vitalName)].ChangeModifier(ModifyingAttribute(&baseAttributes[static_cast<int>(att)], eff));
}

void FBaseCharacter::StatUpdate(const FGameplayAttribute& updatedStat)
{
   TArray<Vital> updatedVits;
   Algo::CopyIf(vitals, updatedVits, [&updatedStat](const Vital& vit) { return vit.attMod.attribute->GetName() == updatedStat.GetName(); });
   for(auto& foundVit : updatedVits)
      foundVit.Update(attSet);

   TArray<RTSUnitStat> updatedStats;
   Algo::CopyIf(skills, updatedStats, [&updatedStat](const RTSUnitStat& skill) { return skill.attMod.attribute->GetName() == updatedStat.GetName(); });
   for(auto& foundSkill : updatedStats)
      foundSkill.Update(attSet);
}

FGameplayAttributeData* FBaseCharacter::GetAttribute(int index) const
{
#if UE_EDITOR
   checkfSlow(index >= 0 && index < baseAttributes.size(), TEXT("ERROR ATT OUT OF RANGE")) // slower check not needed in development or shipping builds
#endif
       return baseAttributes[index]
           .GetGameplayAttributeData(attSet);
}

FGameplayAttributeData* FBaseCharacter::GetSkill(int index) const
{
#if UE_EDITOR
   checkSlow(index >= 0 && index < skills.size())
#endif
       return skills[index]
           .attribute.GetGameplayAttributeData(attSet);
}

FGameplayAttributeData* FBaseCharacter::GetVital(int index) const
{
#if UE_EDITOR
   checkSlow(index >= 0 && index < vitals.size())
#endif
       return vitals[index]
           .attribute.GetGameplayAttributeData(attSet);
}

FGameplayAttributeData* FBaseCharacter::GetMechanic(int index) const
{
#if UE_EDITOR
   checkSlow(index >= 0 && index < mechanics.size())
#endif
       return mechanics[index]
           .GetGameplayAttributeData(attSet);
}

void FBaseCharacter::SetAttributeAdj(int skillIndex, float newValue)
{
   baseAttributes[skillIndex].SetNumericValueChecked(newValue, attSet);
}

void FBaseCharacter::SetSkillAdj(int skillIndex, float newValue)
{
   skills[skillIndex].SetAdjustedValue(newValue, attSet);
}

void FBaseCharacter::SetVitalAdj(int skillIndex, float newValue)
{
   vitals[skillIndex].SetAdjustedValue(newValue, attSet);
}

void FBaseCharacter::SetMechanicAdj(int skillIndex, float newValue)
{
   mechanics[skillIndex].SetNumericValueChecked(newValue, attSet);
}

void FBaseCharacter::SetAttributeBase(int skillIndex, float newValue)
{
   FGameplayAttributeData* skill     = baseAttributes[skillIndex].GetGameplayAttributeData(attSet);
   const int               valueDiff = newValue - skill->GetBaseValue();
   skill->SetBaseValue(newValue);
   skill->SetCurrentValue(skill->GetCurrentValue() + valueDiff);
}

void FBaseCharacter::SetSkillBase(int skillIndex, float newValue)
{
   RTSUnitStat& skill     = skills[skillIndex];
   const int    valueDiff = newValue - skill.GetBaseValue(attSet);
   skill.SetBaseValue(newValue, attSet);
   skill.SetAdjustedValue(skill.GetAdjustedValue(attSet) + valueDiff, attSet);
}

void FBaseCharacter::SetVitalBase(int skillIndex, float newValue)
{
   Vital&    vital     = vitals[skillIndex];
   const int valueDiff = newValue - vital.GetBaseValue(attSet);
   vital.SetBaseValue(newValue, attSet);
   vital.SetAdjustedValue(vital.GetAdjustedValue(attSet) + valueDiff, attSet);
}

void FBaseCharacter::SetMechanicBase(int skillIndex, float newValue)
{
   FGameplayAttributeData* mechanic  = mechanics[skillIndex].GetGameplayAttributeData(attSet);
   const int               valueDiff = newValue - mechanic->GetBaseValue();
   mechanic->SetBaseValue(newValue);
   mechanic->SetCurrentValue(mechanic->GetCurrentValue() + valueDiff);
}
