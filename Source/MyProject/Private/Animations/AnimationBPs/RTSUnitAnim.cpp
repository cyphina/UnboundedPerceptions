#include "MyProject.h"
#include "RTSUnitAnim.h"

#include "UpStatComponent.h"
#include "WorldObjects/Unit.h"

void URTSUnitAnim::AnimNotify_OnAttack(UAnimNotify* notify)
{
   OnHitNotifyEvent.Broadcast();
}

void URTSUnitAnim::UpdateAnimationProperties()
{
   if(unitRef)
   {
      if(unitRef->GetMovementComponent()) bIsFalling = unitRef->GetMovementComponent()->IsFalling();
      movementSpeed = unitRef->GetVelocity().Size();
   }
}

void URTSUnitAnim::NativeBeginPlay()
{
   Super::NativeBeginPlay();
   APawn* pawn = TryGetPawnOwner();
   if(pawn)
   {
      unitRef = Cast<AUnit>(pawn);
   }
}

void URTSUnitAnim::NativeUpdateAnimation(float deltaSeconds)
{
   Super::NativeUpdateAnimation(deltaSeconds);
   UpdateAnimationProperties();
}
