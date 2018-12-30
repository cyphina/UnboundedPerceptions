#pragma once
#include "GameplayAbilityTargetTypes.h"

class AUnit;

struct UnitTargetData {
   UPROPERTY(BlueprintReadOnly)
   AActor* targetActor; // Reference to a target actor (targetUnit, targetInteractable).

   UPROPERTY(BlueprintReadOnly)
   AUnit* targetUnit; // used to target individual units (via spell cast or right click attack) because targetData has too much info

   UPROPERTY(BlueprintReadOnly)
   FVector targetLocation; // used to target area (no restrictions on what can be clicked)

   UPROPERTY(BlueprintReadOnly)
   FGameplayAbilityTargetDataHandle spellTargetData = FGameplayAbilityTargetDataHandle(); // detailed spell targetting information

   UPROPERTY(BlueprintReadOnly)
   AUnit* followTarget; // A target that we can follow.  We can still target something else to attack it while following this unit perhaps?
};