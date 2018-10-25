#include "MyProject.h"
#include "BaseCharacter.h"

FBaseCharacter::FBaseCharacter(const UMyAttributeSet* AttSet)
{
   level  = 1;
   attSet = const_cast<UMyAttributeSet*>(AttSet);

   SetupPrimaryAttributes();
   SetupSkills();
   SetupVitals();
   SetupMechanics();
   StatUpdate();

   // make sure all vitals are full initially
   for (int i = 0; i < vitals.Num(); i++)
      vitals[i].SetCurrValue(vitals[i].GetAdjustedValue());
}

FBaseCharacter::~FBaseCharacter()
{
}

void FBaseCharacter::SetupPrimaryAttributes()
{
   for (FGameplayAttributeData* attData : attSet->GetAtts()) {
      attData->SetBaseValue(MIN_STARTING_ATT_VALUE);
      attData->SetCurrentValue(MIN_STARTING_ATT_VALUE);
      baseAttributes.Add(attData);
   }
}

void FBaseCharacter::SetupSkills()
{
   for (FGameplayAttributeData* skillData : attSet->GetSkills()) {
      skills.Emplace(Stat(*skillData));
   }
   SetupSkillModifiers();
}

void FBaseCharacter::SetupVitals()
{
   for (FGameplayAttributeData* vitData : attSet->GetVitals()) {
      vitals.Emplace(Vital(*vitData));
   }
   SetupVitalModifiers();
}

void FBaseCharacter::SetupMechanics()
{
   for (FGameplayAttributeData* mechData : attSet->GetMechanics()) {
      mechanics.Add(mechData);
   }
   mechanics[static_cast<int>(mech::AttackRange)]->SetBaseValue(BASE_ATTACK_RANGE);
   mechanics[static_cast<int>(mech::MovementSpeed)]->SetBaseValue(BASE_MOVEMENT_SPEED);
   mechanics[static_cast<int>(mech::WeaponPower)]->SetBaseValue(BASE_ATTACK_POWER);
   mechanics[static_cast<int>(mech::GlobalDamageModifier)]->SetBaseValue(0);
   mechanics[static_cast<int>(mech::AttackRange)]->SetCurrentValue(BASE_ATTACK_RANGE);
   mechanics[static_cast<int>(mech::MovementSpeed)]->SetCurrentValue(BASE_MOVEMENT_SPEED);
   mechanics[static_cast<int>(mech::WeaponPower)]->SetCurrentValue(BASE_ATTACK_POWER);
   mechanics[static_cast<int>(mech::GlobalDamageModifier)]->SetCurrentValue(0);
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

void FBaseCharacter::ChangeModifier(sks skillName, atts att, EFF eff)
{
   skills[static_cast<int>(skillName)].ChangeModifier(ModifyingAttribute(baseAttributes[static_cast<int>(att)], eff));
}

void FBaseCharacter::ChangeModifier(vits vitalName, atts att, EFF eff)
{
   vitals[static_cast<int>(vitalName)].ChangeModifier(ModifyingAttribute(baseAttributes[static_cast<int>(att)], eff));
}

void FBaseCharacter::LevelUp()
{
   level++;
}

void FBaseCharacter::StatUpdate()
{
   for (int i = 0; i < vitals.Num(); i++) {
      vitals[i].Update();
   }
   for (int i = 0; i < skills.Num(); i++) {
      skills[i].Update();
   }
}

FGameplayAttributeData* FBaseCharacter::GetAttribute(int index)
{
#if UE_EDITOR
   checkfSlow(index >= 0 && index < baseAttributes.size(), TEXT("ERROR ATT OUT OF RANGE")) // slower check not needed in development or shipping builds
#endif
       return baseAttributes[index];
}

Stat* FBaseCharacter::GetSkill(int index)
{
#if UE_EDITOR
   checkSlow(index >= 0 && index < skills.size())
#endif
       return &skills[index];
}

Vital* FBaseCharacter::GetVital(int index)
{
#if UE_EDITOR
   checkSlow(index >= 0 && index < vitals.size())
#endif
       return &vitals[index];
}

FGameplayAttributeData* FBaseCharacter::GetMechanic(int index)
{
#if UE_EDITOR
   checkSlow(index >= 0 && index < mechanics.size())
#endif
       return mechanics[index];
}
