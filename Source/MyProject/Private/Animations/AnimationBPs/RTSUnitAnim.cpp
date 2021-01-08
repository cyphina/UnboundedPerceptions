// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSUnitAnim.h"

#include "UpStatComponent.h"
#include "WorldObjects/Unit.h"

void URTSUnitAnim::UpdateAnimationProperties()
{
   if(unitRef) {
      if(unitRef->GetMovementComponent()) bIsFalling = unitRef->GetMovementComponent()->IsFalling();
      movementSpeed = unitRef->GetVelocity().Size();
   }
}

void URTSUnitAnim::NativeBeginPlay()
{
   Super::NativeBeginPlay();
   APawn* pawn = TryGetPawnOwner();
   if(pawn) { unitRef = Cast<AUnit>(pawn); }
}

void URTSUnitAnim::NativeUpdateAnimation(float deltaSeconds)
{
   Super::NativeUpdateAnimation(deltaSeconds);
   UpdateAnimationProperties();
}

void URTSUnitAnim::AttackNotify()
{
   OnAttackNotifyEvent.Broadcast();
}

void URTSUnitAnim::PlayAttackAnimation(float playRate)
{
   //! Scale the animation time based on our attack speed
   if(!unitRef->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) { unitRef->PlayAnimMontage(attackAnimation, playRate); }
}

void URTSUnitAnim::StopAttackAnimation()
{
   unitRef->StopAnimMontage(attackAnimation);
}
