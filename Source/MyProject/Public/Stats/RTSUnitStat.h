// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Stats/Attribute.h"
#include "GameplayEffectTypes.h"

class UMyAttributeSet;

namespace CombatInfo
{
   using AttributeModifierFunction = int (*)(int);

   /**
    * @brief Modifying attribute represents relationship between stats and attribute. When we increase an attribute like strength,
    * the stat physical affinity and physical defense will increase.
    */
   struct ModifyingAttribute {
      FGameplayAttribute*       attribute;
      AttributeModifierFunction effectRatio;

      ModifyingAttribute(FGameplayAttribute* att) : attribute(att) { effectRatio = nullptr; }
      ModifyingAttribute(FGameplayAttribute* att, AttributeModifierFunction amf) : attribute(att) { effectRatio = amf; }
      ModifyingAttribute& operator=(const ModifyingAttribute& otherModifyingAttribute);
   };

   /**
    * @brief Stats are skills that increase with attributes. Their values can be queried like attributes
    */
   class MYPROJECT_API RTSUnitStat
   {
    public:
      ModifyingAttribute  attMod;
      FGameplayAttribute attribute; // Since a stat can be directly modified by things like equipment and buffs, we need it to have features of an attribute as well

      RTSUnitStat(FGameplayAttribute attData);
      /// <param name="attData"> Link to AttSet  </param>
      /// <param name="baseV"> Initial Base Value </param>
      /// <param name="mod"> Mod Value </param>
      RTSUnitStat(FGameplayAttribute att, int baseV, ModifyingAttribute mod, UMyAttributeSet* attSet);
      ~RTSUnitStat();

      void ChangeModifier(ModifyingAttribute arr);
      void CalculateModValue(UMyAttributeSet* attSet); // Sets up base value for this stat

      float GetBaseValue(UMyAttributeSet* attSet) const;
      void  SetBaseValue(float value, UMyAttributeSet* attSet);

      float GetBuffValue(UMyAttributeSet* attSet) const; // Can be negative if we're debuffed

      void  SetAdjustedValue(float value, UMyAttributeSet* attSet);
      float GetAdjustedValue(UMyAttributeSet* attSet) const;

      void Update(UMyAttributeSet* attSet);
   };
} // namespace CombatInfo
