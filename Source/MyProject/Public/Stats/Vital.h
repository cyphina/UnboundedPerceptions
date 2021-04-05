// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Stats/RTSUnitStat.h"

class URTSAttributeSet;

namespace CombatInfo
{
   /**
    * Class for unit vitals (health, mana, shields)
    */
   class MYPROJECT_API Vital : public RTSUnitStat
   {
    public:
      Vital(FGameplayAttribute att);
      Vital(FGameplayAttribute att, int baseV, ModifyingAttribute mod, URTSAttributeSet* attSet);
      ~Vital();

      int GetCurrValue(URTSAttributeSet* attSet) const;
   };
}
