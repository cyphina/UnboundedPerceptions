// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Stats/Attribute.h"
#include "GameplayEffectTypes.h"

class UMyAttributeSet;

namespace CombatInfo
{
   // function pointer to a function that maps the attribute to how much it corresponds to the modified stat
   using EFF = int (*)(int);

   // Modifying attribute represents relationship between stats and attribute
   struct ModifyingAttribute {
      FGameplayAttribute* attribute; // Attribute the stat with this modifyingattribute attachment will scale off of
      EFF                 effectRatio;

      /// <param name="att"> BaseAttribute </param>
      /// <param name="eff"> Effect Ratio </param>

      ModifyingAttribute(FGameplayAttribute* att) : attribute(att) { effectRatio = nullptr; }
      ModifyingAttribute(FGameplayAttribute* att, EFF eff) : attribute(att) { effectRatio = eff; }
      ModifyingAttribute& operator=(const ModifyingAttribute& otherModifyingAttribute);
   };

   // Stats are skills that increase with attributes.  From the outside, they look like attributes
   class MYPROJECT_API RTSUnitStat
   {
    public:
      ModifyingAttribute  attMod;
      FGameplayAttribute& attribute; // Since a stat can be directly modified by things like equipment and buffs, we need it to have features of an attribute as well

      RTSUnitStat(FGameplayAttribute& attData);
      /// <param name="attData"> Link to AttSet  </param>
      /// <param name="baseV"> Initial Base Value </param>
      /// <param name="mod"> Mod Value </param>
      RTSUnitStat(FGameplayAttribute& att, int baseV, ModifyingAttribute mod, UMyAttributeSet* attSet);
      ~RTSUnitStat();

      // allowing us to change what modifies what
      void  ChangeModifier(ModifyingAttribute arr);
      void  CalculateModValue(UMyAttributeSet* attSet); // sets up base value for this stat
      float GetBaseValue(UMyAttributeSet* attSet) const;
      void  SetBaseValue(float value, UMyAttributeSet* attSet);
      float GetBuffValue(UMyAttributeSet* attSet) const; // Can be negative if we're debuffed
      void  SetAdjustedValue(float value, UMyAttributeSet* attSet);
      float GetAdjustedValue(UMyAttributeSet* attSet) const;

      void Update(UMyAttributeSet* attSet);
   };
} // namespace CombatInfo