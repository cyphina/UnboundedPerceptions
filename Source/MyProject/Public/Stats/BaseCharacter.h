#pragma once

#include "Stats/Vital.h"
#include "StatEnums.h"
<<<<<<< HEAD
#include "MyAttributeSet.h"
=======
#include "RTSAttributeSet.h"
>>>>>>> componentrefactor

class UAbilitySystemComponent;

namespace CombatInfo
{
   static const int     AttCount       = static_cast<int>(EAttributes::Count);
   static const int     StatCount      = static_cast<int>(EUnitScalingStats::Count);
   static const int     VitalCount     = static_cast<int>(EVitals::Count);
   static const int     MechanicCount  = static_cast<int>(EMechanics::Count);
   static constexpr int TotalStatCount = AttCount + StatCount + VitalCount + MechanicCount;
}

using namespace CombatInfo;

/** 
 * A wrapper class around the attribute set to enable some custom scaling on stats
 * We never need to use this class directly. A stat component contains this struct and it will expose this classes useful functionality
 */
USTRUCT(BlueprintType, NoExport)
struct FBaseCharacter
{
   static const int BASE_ATTACK_RANGE   = 500;
   static const int BASE_MOVEMENT_SPEED = 500;
   static const int BASE_ATTACK_POWER   = 100;

<<<<<<< HEAD
   FBaseCharacter(const UMyAttributeSet& AttSet);
   ~FBaseCharacter();
   FBaseCharacter& operator=(const FBaseCharacter& otherChar) = default;
   int             GetLevel() const { return level; }
   void            SetLevel(int newLevel) { level = newLevel; }
   void            InitializeAttributeBaseValues();
=======
   FBaseCharacter(const URTSAttributeSet& AttSet);
   ~FBaseCharacter();
   FBaseCharacter& operator=(const FBaseCharacter& otherChar) = default;

   int GetLevel() const { return level; }

   void SetLevel(int newLevel) { level = newLevel; }

   void InitializeAttributeBaseValues();
>>>>>>> componentrefactor

   /** Recalculate base values of skills from a base attribute change
    * @param updatedStat - Attribute that was modified
    */
   void StatUpdate(const FGameplayAttribute& updatedStat);

   FGameplayAttributeData* GetAttribute(int skill) const;
   FGameplayAttributeData* GetSkill(int skill) const;
   FGameplayAttributeData* GetVital(int skill) const;
   FGameplayAttributeData* GetMechanic(int skill) const;

   void SetAttributeAdj(int skillIndex, float newValue);
   void SetSkillAdj(int skillIndex, float newValue);
   void SetVitalAdj(int skillIndex, float newValue);
   void SetMechanicAdj(int skillIndex, float newValue);

   void SetAttributeBase(int skillIndex, float newValue);
   void SetSkillBase(int skillIndex, float newValue);
   void SetVitalBase(int skillIndex, float newValue);
   void SetMechanicBase(int skillIndex, float newValue);

   const TArray<FGameplayAttribute>& GetAttributes() const { return baseAttributes; }

   const TArray<RTSUnitStat>& GetSkills() const { return skills; }

   const TArray<Vital>& GetVitals() const { return vitals; }

<<<<<<< HEAD
   const TArray<FGameplayAttribute*>& GetAttributes() const { return baseAttributes; }
   const TArray<RTSUnitStat>&         GetSkills() const { return skills; }
   const TArray<Vital>&               GetVitals() const { return vitals; }
   const TArray<FGameplayAttribute*>& GetMechanics() const { return mechanics; }
=======
   const TArray<FGameplayAttribute>& GetMechanics() const { return mechanics; }
>>>>>>> componentrefactor

   URTSAttributeSet* GetAttSet() const { return attSet; }

private:
   URTSAttributeSet* attSet;
   using atts = EAttributes;
   using sks = EUnitScalingStats;
   using vits = EVitals;
   using mech = EMechanics;

   int level = 1;

<<<<<<< HEAD
   TArray<FGameplayAttribute*> baseAttributes;
   TArray<RTSUnitStat>         skills; // called them skills here but stats and skills are interchangeable names
   TArray<Vital>               vitals;
   TArray<FGameplayAttribute*> mechanics;
=======
   // TODO: One day fix so we are not storing multiple copies of these property references...
   TArray<FGameplayAttribute> baseAttributes;
   TArray<RTSUnitStat>        skills; // called them skills here but stats and skills are interchangeable names
   TArray<Vital>              vitals;
   TArray<FGameplayAttribute> mechanics;
>>>>>>> componentrefactor

   void SetupPrimaryAttributes();
   void InitialStatUpdate();
   void SetupSkills();
   void SetupVitals();
   void SetupMechanics();
   void SetupSkillModifiers();
   void SetupVitalModifiers();
   void ChangeModifier(sks skillName, atts att, AttributeModifierFunction eff);
   void ChangeModifier(vits skillName, atts att, AttributeModifierFunction eff);
};
