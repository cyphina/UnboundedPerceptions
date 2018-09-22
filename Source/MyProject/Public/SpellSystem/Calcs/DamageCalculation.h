// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "SpellSystem/DamageStructs.h"
#include "DamageCalculation.generated.h"

class AUnit;

/**Damage calculation is used to calculate the damage.  Grabs information from target and receiving unit to figure out damage amounts, as well as 
 * using information gotten from the effect itself
 */
UCLASS()
class MYPROJECT_API UDamageCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_UCLASS_BODY()
	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams, OUT FGameplayEffectCustomExecutionOutput& outExecutionOutput) const override;

private:

	//Plug in damage to damage formula and modify the damage amount based on crit or piercing
	void CalculateDamage(AUnit* unit, Damage& d, FGameplayTagContainer& effects) const;
	//Damage modification based off effects, buffs, or weapon effects
	void ApplyEffects(AUnit* unit, Damage& d, FGameplayTagContainer& effects) const;
	//Damage modification based off defensive effects.  If USING MAGICAL ARMOR, attacker may also get some debuffs
	void ReceiveEffects(AUnit* unit, Damage& damage, FGameplayTagContainer& effects) const;
	//Damage calculation on receiving end, and possibly dodge check since accuracy is completely calculated at this step. Dodge check at beginning would help optimize, but true accuracy value isn't known
	//Public since AUnit declares this as a friend function so we can access the setter methods for change stats
	void CalculateDamageReduction(AUnit* unit, Damage& damage, FGameplayTagContainer& effects) const;

	//Helper function to quickly calculate attack and defense bonuses due to affinity and resistance
	void CalculatePiercing(AUnit* unit, Damage& d, bool isAtt) const;

public:
	//This is the function that calls everything else in the damage pipeline
	void DamageTarget(AUnit* sourceUnit, AUnit* targetUnit, Damage& d, FGameplayTagContainer effects) const;
};

