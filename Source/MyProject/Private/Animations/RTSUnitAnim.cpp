// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSUnitAnim.h"
#include "WorldObjects/Unit.h"

void URTSUnitAnim::UpdateAnimationProperties()
{
   if (unitRef) {
      // Update our falling property
      if (unitRef->GetMovementComponent()) bIsFalling = unitRef->GetMovementComponent()->IsFalling();
      // Update our movement speed
      movementSpeed = unitRef->GetVelocity().Size();
   }
}

void URTSUnitAnim::NativeBeginPlay()
{
   Super::NativeBeginPlay();
   APawn* pawn = TryGetPawnOwner();
   if (pawn) {
      unitRef = Cast<AUnit>(pawn);
   }
}

void URTSUnitAnim::NativeUpdateAnimation(float deltaSeconds)
{
   Super::NativeUpdateAnimation(deltaSeconds);
   UpdateAnimationProperties();
}

void URTSUnitAnim::OnAttackNotify()
{
   OnFinishSwingEvent.Broadcast();
}

EUnitState URTSUnitAnim::GetCurrentState()
{
   if(unitRef)
      return unitRef->GetState();
   return EUnitState::STATE_IDLE;
}
