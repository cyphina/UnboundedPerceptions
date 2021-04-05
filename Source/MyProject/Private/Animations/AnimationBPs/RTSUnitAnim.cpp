<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.

=======
>>>>>>> componentrefactor
#include "MyProject.h"
#include "RTSUnitAnim.h"

#include "UpStatComponent.h"
#include "WorldObjects/Unit.h"

<<<<<<< HEAD
void URTSUnitAnim::UpdateAnimationProperties()
{
   if(unitRef) {
      if(unitRef->GetMovementComponent()) bIsFalling = unitRef->GetMovementComponent()->IsFalling();
=======
TSoftObjectPtr<UAnimMontage> URTSUnitAnim::GetAnimMontage(FGameplayTag tagToQuery) const
{
   if(IsValid(montageMap))
   {
      return montageMap.GetDefaultObject()->GetAnimMontage(tagToQuery);
   }
   return nullptr;
}

void URTSUnitAnim::UpdateAnimationProperties()
{
   if(unitRef)
   {
      if(unitRef->GetMovementComponent())
         bIsFalling = unitRef->GetMovementComponent()->IsFalling();
>>>>>>> componentrefactor
      movementSpeed = unitRef->GetVelocity().Size();
   }
}

void URTSUnitAnim::NativeBeginPlay()
{
   Super::NativeBeginPlay();
<<<<<<< HEAD
   APawn* pawn = TryGetPawnOwner();
   if(pawn) { unitRef = Cast<AUnit>(pawn); }
=======
   if(APawn* pawn = TryGetPawnOwner())
   {
      unitRef = Cast<AUnit>(pawn);
   }

   OnPlayMontageNotifyBegin.AddDynamic(this, &URTSUnitAnim::OnAnimationNotify);
>>>>>>> componentrefactor
}

void URTSUnitAnim::NativeUpdateAnimation(float deltaSeconds)
{
   Super::NativeUpdateAnimation(deltaSeconds);
   UpdateAnimationProperties();
}

<<<<<<< HEAD
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
=======
void URTSUnitAnim::OnAnimationNotify(FName notifyName, const FBranchingPointNotifyPayload& branchingPointPayload)
{
   if(notifyName == "OnAttack")
   {
      OnHitNotifyEvent.Broadcast();
   }
>>>>>>> componentrefactor
}
