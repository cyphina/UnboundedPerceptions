// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSAttributeSet.h"
#include "Vital.h"

CombatInfo::Vital::Vital(FGameplayAttribute att) : CombatInfo::RTSUnitStat(att)
{

}

CombatInfo::Vital::Vital(FGameplayAttribute att, int baseV, ModifyingAttribute mod, URTSAttributeSet* attSet) : CombatInfo::RTSUnitStat(att, baseV, mod, attSet)
{

}

CombatInfo::Vital::~Vital()
{
}

int CombatInfo::Vital::GetCurrValue(URTSAttributeSet* attSet) const
{
   return attribute.GetNumericValue(attSet);
}
