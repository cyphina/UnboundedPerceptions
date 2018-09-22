#pragma once
#include "GameplayAbilityTargetTypes.h"

class AUnit;

struct UnitTargetData
{
	AActor*								targetActor; //Reference to a target actor (targetUnit, targetInteractable).
	AUnit*								targetUnit; //used to target individual units (via spell cast or right click attack) because targetData has too much info
	FVector								targetLocation; //used to target area (no restrictions on what can be clicked)
	FGameplayAbilityTargetDataHandle	spellTargetData = FGameplayAbilityTargetDataHandle(); //detailed spell targetting information
};