// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "MyAnim.h"
#include "WorldObjects/Unit.h"

void UMyAnim::UpdateAnimationProperties()
{
   APawn* pawn = TryGetPawnOwner();
   if (pawn) {
      // Update our falling property
      if (pawn->GetMovementComponent()) bIsFalling = pawn->GetMovementComponent()->IsFalling();
      // Update our movement speed
      movementSpeed = pawn->GetVelocity().Size();
   }
}

void UMyAnim::NativeBeginPlay()
{
   APawn* pawn = TryGetPawnOwner();
   if (pawn) {
      unitRef = Cast<AUnit>(pawn);
   }
}

void UMyAnim::NativeUpdateAnimation(float deltaSeconds)
{
   UpdateAnimationProperties();
}

EUnitState UMyAnim::GetCurrentState()
{
   if(unitRef)
      return unitRef->GetState();
   return EUnitState::STATE_IDLE;
}
