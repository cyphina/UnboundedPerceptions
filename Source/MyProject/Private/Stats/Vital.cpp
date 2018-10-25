// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "AttributeSet.h"
#include "Vital.h"

CombatInfo::Vital::Vital(FGameplayAttributeData& att) : CombatInfo::Stat(att)
{
   curValue = 0;
}

CombatInfo::Vital::Vital(FGameplayAttributeData& att, int baseV, ModifyingAttribute mod) : CombatInfo::Stat(att, baseV, mod)
{
   curValue = baseV;
}

CombatInfo::Vital::~Vital()
{
}

int CombatInfo::Vital::GetCurrValue()
{
   if (curValue >= bA.GetCurrentValue()) { curValue = bA.GetCurrentValue(); }
   return curValue;
}
