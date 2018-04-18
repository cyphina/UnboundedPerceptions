// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Stats/MyAttributeSet.h"
#include "StatChangeCalc.generated.h"

/**
 * For instantaneous stat effects, use a period of 999 and set "Execute periodic effect on application"
 */
UCLASS()
class MYPROJECT_API UStatChangeCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_UCLASS_BODY()
	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams, OUT FGameplayEffectCustomExecutionOutput& outExecutionOutput) const override;
};

struct AttStruct
{
	///Declare attributes to be captured
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health); //declares two variables (pointer to UProperty that will be captured and GameplayEffectAttributeCapture)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength); 
	DECLARE_ATTRIBUTE_CAPTUREDEF(Understanding); 
	DECLARE_ATTRIBUTE_CAPTUREDEF(Intelligence);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Explosiveness); 
	DECLARE_ATTRIBUTE_CAPTUREDEF(Endurance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Agility); 
	DECLARE_ATTRIBUTE_CAPTUREDEF(Luck);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Critical_Chance); 
	DECLARE_ATTRIBUTE_CAPTUREDEF(Critical_Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Accuracy); 
	DECLARE_ATTRIBUTE_CAPTUREDEF(Dodge); 
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack_Speed);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Cast_Speed); 
	DECLARE_ATTRIBUTE_CAPTUREDEF(MovementSpeed);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackRange); 
	DECLARE_ATTRIBUTE_CAPTUREDEF(WeaponPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(GlobalDamageModifier);

	AttStruct()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, Health, Source, false); //sets up property and definition.  Last parameter means if we snapshot value at time of this definition
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, Strength, Source, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, Understanding, Source, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, Intelligence, Source, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, Explosiveness, Source, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, Endurance, Source, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, Agility, Source, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, Luck, Source, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, Critical_Chance, Source, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, Critical_Damage, Source, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, Accuracy, Source, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, Dodge, Source, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, Attack_Speed, Source, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, Cast_Speed, Source, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, MovementSpeed, Source, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, AttackRange, Source, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, WeaponPower, Source, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, GlobalDamageModifier, Source, false); 
	}
};
