// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSUnitStat.h"

#include "MyAttributeSet.h"

CombatInfo::ModifyingAttribute& CombatInfo::ModifyingAttribute::operator=(const ModifyingAttribute& otherModifyingAttribute)
{
   attribute = otherModifyingAttribute.attribute;
   effectRatio = otherModifyingAttribute.effectRatio;
   return *this;
}

CombatInfo::RTSUnitStat::RTSUnitStat(FGameplayAttribute attData) : attMod(ModifyingAttribute(&attribute)), attribute{attData}
{
}

CombatInfo::RTSUnitStat::RTSUnitStat(FGameplayAttribute att, int baseV, ModifyingAttribute mod, UMyAttributeSet* attSet) : attMod{mod}, attribute{att}
{
   auto attData = att.GetGameplayAttributeData(attSet);
   float newBaseV = baseV;
   attData->SetBaseValue(newBaseV);
   attSet->PreAttributeBaseChange(att, newBaseV);
}

CombatInfo::RTSUnitStat::~RTSUnitStat()
{
}

void CombatInfo::RTSUnitStat::ChangeModifier(ModifyingAttribute mod)
{
   attMod = mod;
}

void CombatInfo::RTSUnitStat::CalculateModValue(UMyAttributeSet* attSet)
{
   // When recalculating the modified value due to an attribute change, we don't recalculate the buff values (so you can get larger buffs if you increase your base value first)
   const float currentBaseValue = GetBaseValue(attSet);
   float scaledValue = attMod.effectRatio(attMod.attribute->GetNumericValue(attSet));
   float adjustedDifference =  attribute.GetNumericValue(attSet) + scaledValue - currentBaseValue;

   attribute.GetGameplayAttributeData(attSet)->SetBaseValue(scaledValue);
   attSet->PreAttributeBaseChange(attribute, scaledValue);
   attribute.SetNumericValueChecked(adjustedDifference, attSet);
}

float CombatInfo::RTSUnitStat::GetBaseValue(UMyAttributeSet* attSet) const
{
   return attribute.GetGameplayAttributeData(attSet)->GetBaseValue();
}

void CombatInfo::RTSUnitStat::SetBaseValue(float value, UMyAttributeSet* attSet)
{
   auto attData = attribute.GetGameplayAttributeData(attSet);
   const float oldBaseVal = attData->GetBaseValue();
   attData->SetBaseValue(value);
   attSet->PreAttributeBaseChange(attribute, value);
   float newCurrentValue = attribute.GetNumericValue(attSet) + value - oldBaseVal;

   // Add to adjusted value with by the difference in how much our base was changed 
   attribute.SetNumericValueChecked(newCurrentValue, attSet);
}

float CombatInfo::RTSUnitStat::GetBuffValue(UMyAttributeSet* attSet) const
{
   const auto attData = attribute.GetGameplayAttributeData(attSet);
   const float baseVal = attData->GetBaseValue();
   return attribute.GetNumericValue(attSet) - baseVal;
}

float CombatInfo::RTSUnitStat::GetAdjustedValue(UMyAttributeSet* attSet) const
{
   return attribute.GetNumericValue(attSet);
}

void CombatInfo::RTSUnitStat::SetAdjustedValue(float value, UMyAttributeSet* attSet)
{
   attribute.SetNumericValueChecked(value, attSet);
}

void CombatInfo::RTSUnitStat::Update(UMyAttributeSet* attSet)
{
   CalculateModValue(attSet);
}
