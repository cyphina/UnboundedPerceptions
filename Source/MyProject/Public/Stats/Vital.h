// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Stats/Stat.h"
namespace CombatInfo
{
   /**
    *
    */
   class MYPROJECT_API Vital : public Stat
   {
      // vital num that represents current value (0-adjustedVal)
      int curValue;

    public:
      Vital(FGameplayAttributeData& att);
      Vital(FGameplayAttributeData& att, int baseV, ModifyingAttribute mod);
      ~Vital();

      int GetCurrValue();

      void SetCurrValue(int value) { curValue = value; }
   };
} // namespace CombatInfo
