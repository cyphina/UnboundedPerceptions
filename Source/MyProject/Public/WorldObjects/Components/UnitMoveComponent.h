// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UnitMoveComponent.generated.h"

/**
 * A component used for units that perform basic walk movement.
 * All units in this game walk, even ones that dig get a "phased" effect and then walk quickly underground (aka while invisible)
 * Flying units will just have a flapping animation or something and maybe some capabilities to pass through walls
 *
 * Now since we've established that the capabilities we are looking for are already contained within the character movement component, what is the reason we override?
 * 1. Replicated move speed updates (https://www.youtube.com/watch?v=Of8SGBa3WvU)
 * 2. Learn about the component and eventually learn to create a more generic pawn component to remove unnecessary features like swimming/flying/etc.
 *
 * Actual movement is triggered by a pawn's AIController. It uses MoveToLocatoin which will eventually use code within the attached movement component on a pawn
 */

struct FGameplayAttribute;
class AUnit;

UCLASS()
class MYPROJECT_API UUnitMoveComponent : public UCharacterMovementComponent
{
   GENERATED_BODY()

   void OnMoveSpeedChanged(const FGameplayAttribute& att, float newVal, AUnit* unitRef);

protected:
  void BeginPlay() override;
};
