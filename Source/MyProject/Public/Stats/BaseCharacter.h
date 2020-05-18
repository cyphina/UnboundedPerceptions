#pragma once

#include "Stats/Vital.h"
#include "MyAttributeSet.h"

/**
 * BaseCharacter - The Base Class for Combat Oriented Information
 */

namespace CombatInfo
{
   static const int     AttCount       = 7;
   static const int     StatCount      = 32;
   static const int     VitalCount     = 5;
   static const int     MechanicCount  = 4;
   static constexpr int TotalStatCount = AttCount + StatCount + VitalCount + MechanicCount;
} // namespace CombatInfo

using namespace CombatInfo;

class UAbilitySystemComponent;
#pragma region enums

// Quantities used for determining skill power and stats
UENUM(BlueprintType)
enum class EAttributes : uint8 {
   // Skill Damage
   Strength = 0,
   // Elemental Affinity, Elemental Resistance, PPC Shield
   Understanding,
   // Spell Damage
   Intelligence,
   // Physical Affinity, Dodge Ratio, AttackSpeed
   Explosiveness,
   // Physical Resistance, Hitpoints
   Endurance,
   // Skill Damage
   Agility,
   // Hit Ratio, Critical Ratio, Critical Strike Damage
   Luck,
};

// Impactful qualities
UENUM(BlueprintType)
enum class EUnitScalingStats : uint8 {
   Critical_Chance,
   Critical_Damage,
   Accuracy,
   Dodge,
   Attack_Speed,
   Cast_Speed,

   // Affinity (these suffixes are used in statgraph so be weary of changing them)
   Physical_Aff,
   Fire_Aff,
   Water_Aff,
   Wind_Aff,
   Earth_Aff,
   Electric_Aff,
   Darkness_Aff,
   Light_Aff,
   Arcane_Aff,
   Chaos_Aff,
   Poison_Aff,
   Blood_Aff,
   Ethereal_Aff,

   // Resistances (these suffixes are used in statgraph so be weary of changing them)
   Physical_Resist,
   Fire_Resist,
   Water_Resist,
   Wind_Resist,
   Earth_Resist,
   Electric_Resist,
   Darkness_Resist,
   Light_Resist,
   Arcane_Resist,
   Chaos_Resist,
   Poison_Resist,
   Blood_Resist,
   Ethereal_Resist,
};

// Battle resources
UENUM(BlueprintType)
enum class EVitals : uint8 {
   Health,
   Mana,
   Psyche,
   Moxie,  // only increasable by items, normal cap of 100
   Shield, // only increasable by items and buffs
};

// Useful game mechanics
UENUM(BlueprintType)
enum class EMechanics : uint8 {
   MovementSpeed, // max walking speed in ue4 is based off of centimeters per second.  Average persom walks 5km in an hour, which is around 138 cm/s which is how max speed is measured
   AttackRange,
   WeaponPower,
   GlobalDamageModifier // damage reduction modifier, 0 = no damage reduction, 100 = no damage recieved
};

ENUM_RANGE_BY_COUNT(EAttributes, CombatInfo::AttCount);
ENUM_RANGE_BY_COUNT(EUnitScalingStats, CombatInfo::StatCount);
ENUM_RANGE_BY_COUNT(EVitals, CombatInfo::VitalCount);
ENUM_RANGE_BY_COUNT(EMechanics, CombatInfo::MechanicCount);

#pragma endregion

USTRUCT(BlueprintType, NoExport)
struct FBaseCharacter {
   static const int BASE_ATTACK_RANGE   = 500;
   static const int BASE_MOVEMENT_SPEED = 500;
   static const int BASE_ATTACK_POWER   = 100;
   // how many elements in each enum

   FBaseCharacter(const UMyAttributeSet* AttSet);
   ~FBaseCharacter();
   FBaseCharacter& operator=(const FBaseCharacter& otherChar) = default;
   int             GetLevel() const { return level; }
   void            InitializeAttributeBaseValues();
   void            LevelUp();
   void            StatUpdate(const FGameplayAttribute& updatedStat); // Recalculate base values

   FGameplayAttributeData* GetAttribute(int skill) const;
   FGameplayAttributeData* GetSkill(int skill) const;
   FGameplayAttributeData* GetVital(int skill) const;
   FGameplayAttributeData* GetMechanic(int skill) const;

   void SetAttributeAdj(int skill, float newValue);
   void SetSkillAdj(int skill, float newValue);
   void SetVitalAdj(int skill, float newValue);
   void SetMechanicAdj(int skill, float newValue);

   void SetAttributeBase(int skill, float newValue);
   void SetSkillBase(int skill, float newValue);
   void SetVitalBase(int skill, float newValue);
   void SetMechanicBase(int skill, float newValue);

   TArray<FGameplayAttribute> GetAttributes() const { return baseAttributes; }
   TArray<RTSUnitStat>        GetSkills() const { return skills; }
   TArray<Vital>              GetVitals() const { return vitals; }
   TArray<FGameplayAttribute> GetMechanics() const { return mechanics; }

   UMyAttributeSet* GetAttSet() const { return attSet; }

 private:
   UMyAttributeSet* attSet;
   using atts = EAttributes;
   using sks  = EUnitScalingStats;
   using vits = EVitals;
   using mech = EMechanics;

   int level                  = 1;
   int MIN_STARTING_ATT_VALUE = 20;

   TArray<FGameplayAttribute> baseAttributes;
   TArray<RTSUnitStat>        skills; // called them skills here but stats and skills are interchangeable names
   TArray<Vital>              vitals;
   TArray<FGameplayAttribute> mechanics;

   void SetupPrimaryAttributes();
   void InitialStatUpdate();
   void SetupSkills();
   void SetupVitals();
   void SetupMechanics();
   void SetupSkillModifiers();
   void SetupVitalModifiers();
   void ChangeModifier(sks skillName, atts att, EFF eff);
   void ChangeModifier(vits skillName, atts att, EFF eff);
};
