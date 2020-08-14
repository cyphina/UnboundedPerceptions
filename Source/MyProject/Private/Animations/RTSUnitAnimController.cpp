// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSUnitAnimController.h"

#include "UpStatComponent.h"
#include "WorldObjects/Unit.h"

void URTSUnitAnimController::UpdateFallFlag()
{
   if(unitRef->GetMovementComponent())
      bIsFalling = unitRef->GetMovementComponent()->IsFalling();
}

void URTSUnitAnimController::UpdateAnimationProperties()
{
   if(unitRef) {
      UpdateFallFlag();
      movementSpeed = unitRef->GetVelocity().Size();
   }
}

void URTSUnitAnimController::NativeBeginPlay()
{
   Super::NativeBeginPlay();
   APawn* pawn = TryGetPawnOwner();
   if(pawn) {
      unitRef = Cast<AUnit>(pawn);
   }
}

void URTSUnitAnimController::NativeUpdateAnimation(float deltaSeconds)
{
   Super::NativeUpdateAnimation(deltaSeconds);
   UpdateAnimationProperties();
}

void URTSUnitAnimController::AnimNotify_Hit() const
{
   unitRef->OnUnitHit().Broadcast();
}
