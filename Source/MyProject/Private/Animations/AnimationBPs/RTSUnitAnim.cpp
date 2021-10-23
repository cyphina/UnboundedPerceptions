#include "MyProject.h"
#include "RTSUnitAnim.h"

#include "UpStatComponent.h"
#include "WorldObjects/Unit.h"

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
      movementSpeed = unitRef->GetVelocity().Size();
   }
}

void URTSUnitAnim::NativeBeginPlay()
{
   Super::NativeBeginPlay();
   if(APawn* pawn = TryGetPawnOwner())
   {
      unitRef = Cast<AUnit>(pawn);
   }

   OnPlayMontageNotifyBegin.AddDynamic(this, &URTSUnitAnim::OnAnimationNotify);
}

void URTSUnitAnim::NativeUpdateAnimation(float deltaSeconds)
{
   Super::NativeUpdateAnimation(deltaSeconds);
   UpdateAnimationProperties();
}

void URTSUnitAnim::OnAnimationNotify(FName notifyName, const FBranchingPointNotifyPayload& branchingPointPayload)
{
   if(notifyName == "OnAttack")
   {
      OnHitNotifyEvent.Broadcast();
   }
}
