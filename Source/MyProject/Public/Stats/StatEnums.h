#pragma once

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

   Count UMETA(Hidden)
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

   Count UMETA(Hidden)
};

// Battle resources
UENUM(BlueprintType)
enum class EVitals : uint8 {
   Health,
   Mana,
   Psyche,
   Moxie,  // only increasable by items, normal cap of 100
   Shield, // only increasable by items and buffs

   Count UMETA(Hidden)

};

// Useful game mechanics
UENUM(BlueprintType)
enum class EMechanics : uint8 {
   MovementSpeed, // max walking speed in ue4 is based off of centimeters per second.  Average person walks 5km in an hour, which is around 138 cm/s which is how max speed is measured
   AttackRange,
   WeaponPower,
   GlobalDamageModifier, // damage reduction modifier, 0 = no damage reduction, 100 = no damage received

   Count UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EAttributes, EAttributes::Count);
ENUM_RANGE_BY_COUNT(EUnitScalingStats, EUnitScalingStats::Count);
ENUM_RANGE_BY_COUNT(EVitals, EUnitScalingStats::Count);
ENUM_RANGE_BY_COUNT(EMechanics, EUnitScalingStats::Count);
