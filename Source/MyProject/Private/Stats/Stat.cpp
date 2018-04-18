// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Stat.h"


CombatInfo::Stat::Stat(FGameplayAttributeData& attData) : attMod(ModifyingAttribute(&attData)), bA{attData}
{

}

CombatInfo::Stat::Stat(FGameplayAttributeData& attData, int baseV, ModifyingAttribute mod) : attMod(ModifyingAttribute(attMod)), bA{attData}
{
	attData.SetBaseValue(baseV);
}

CombatInfo::Stat::~Stat()
{
}

void CombatInfo::Stat::ChangeModifier(ModifyingAttribute mod) 
{
	attMod = mod;
}

//calculate base value
void CombatInfo::Stat::CalculateModValue()
{
/*
#if UE_EDITOR
	//FString(ANSI_TO_TCHAR(attMod.attribute->GetName()))
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Emerald, FString::FromInt(GetAdjustedValue()) + " " + FString::FromInt(attMod.attribute->GetAdjustedValue()));
#endif */
	bA.SetBaseValue(attMod.effectRatio(attMod.attribute->GetBaseValue()));
	bA.SetCurrentValue(attMod.effectRatio(attMod.attribute->GetBaseValue())); //set the value of the function to the baseValue
}

float CombatInfo::Stat::GetBaseValue() const
{
	return bA.GetBaseValue();
}

void CombatInfo::Stat::SetBaseValue(float value)
{
	bA.SetBaseValue(value);
}

float CombatInfo::Stat::GetBuffValue() const 
{
	return bA.GetCurrentValue() - bA.GetBaseValue();
}

float CombatInfo::Stat::GetAdjustedValue() const
{
	return bA.GetCurrentValue(); 
}

void CombatInfo::Stat::SetBuffValue(float value)
{
	bA.SetCurrentValue(value);
}

void CombatInfo::Stat::Update()
{
	CalculateModValue();
}

