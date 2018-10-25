#pragma once

#include "Stats/Vital.h"
#include "MyAttributeSet.h"

/**
 * BaseCharacter - The Base Class for Combat Oriented Information
 */

namespace CombatInfo
{
   static const int AttCount      = 7;
   static const int StatCount     = 32;
   static const int VitalCount    = 5;
   static const int MechanicCount = 4;
} // namespace CombatInfo

using namespace CombatInfo;

class UAbilitySystemComponent;
#pragma region enums

// Quantities used for determining skill power and stats
UENUM(BlueprintType)
enum class Attributes : uint8 {
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
enum class UnitStats : uint8 {
   Critical_Chance,
   Critical_Damage,
   Accuracy,
   Dodge,
   Attack_Speed,
   Cast_Speed,

   // Affinity
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

   // Resistances
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
enum class Vitals : uint8 {
   Health,
   Mana,
   Psyche,
   Moxie,  // only increasable by items, normal cap of 100
   Shield, // only increasable by items and buffs
};

// Useful game mechanics
UENUM(BlueprintType)
enum class Mechanics : uint8 {
   MovementSpeed, // max walking speed in ue4 is based off of centimeters per second.  Average persom walks 5km in an hour, which is around 138 cm/s which is how max speed is measured
   AttackRange,
   WeaponPower,
   GlobalDamageModifier // damage reduction modifier, 0 = no damage reduction, 100 = no damage recieved
};

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
   void            LevelUp();
   void            StatUpdate(); // Recalculate base values

   FGameplayAttributeData* GetAttribute(int skill);
   Stat*                   GetSkill(int skill);
   Vital*                  GetVital(int skill);
   FGameplayAttributeData* GetMechanic(int skill);

   TArray<FGameplayAttributeData*> GetAttributes() const { return baseAttributes; }
   TArray<Stat>                    GetSkills() const { return skills; }
   TArray<Vital>                   GetVitals() const { return vitals; }
   TArray<FGameplayAttributeData*> GetMechanics() const { return mechanics; }

 private:
   UMyAttributeSet* attSet;
   using atts = Attributes;
   using sks  = UnitStats;
   using vits = Vitals;
   using mech = Mechanics;

   int level                  = 1;
   int MIN_STARTING_ATT_VALUE = 20;

   TArray<FGameplayAttributeData*> baseAttributes;
   TArray<Stat>                    skills; // called them skills here but stats and skills are interchangeable names
   TArray<Vital>                   vitals;
   TArray<FGameplayAttributeData*> mechanics;

   void SetupPrimaryAttributes();
   void SetupSkills();
   void SetupVitals();
   void SetupMechanics();
   void SetupSkillModifiers();
   void SetupVitalModifiers();
   void ChangeModifier(sks skillName, atts att, EFF eff);
   void ChangeModifier(vits skillName, atts att, EFF eff);
};
