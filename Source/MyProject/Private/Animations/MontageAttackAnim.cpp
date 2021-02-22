// Created 8/7/20 1:25 PM

#include "MyProject.h"
#include "MontageAttackAnim.h"

#include "RTSUnitAnim.h"
#include "GameFramework/Character.h"

UMontageAttackAnim::UMontageAttackAnim()
{
}

void UMontageAttackAnim::PlayAttackAnimation(float playRate)
{
   ACharacter* character = Cast<ACharacter>(GetOuter());
   if(character)
   {
      if(UAnimInstance* animBP = character->GetMesh()->GetAnimInstance())
      {
         FOnMontageEnded montageEndedDelegate;
         montageEndedDelegate.BindUObject(this, &UMontageAttackAnim::OnMontageEnded);
         animBP->Montage_SetEndDelegate(montageEndedDelegate, attackMontage);
      }
      character->PlayAnimMontage(attackMontage, playRate);
   }
}

void UMontageAttackAnim::StopAttackAnimation()
{
   Cast<ACharacter>(GetOuter())->StopAnimMontage(attackMontage);
}

FOnHitNotify* UMontageAttackAnim::OnAttackNotify()
{
   ACharacter* character = Cast<ACharacter>(GetOuter());
   if(character)
   {
      if(URTSUnitAnim* unitAnim = Cast<URTSUnitAnim>(character->GetMesh()->GetAnimInstance()))
      {
         return &unitAnim->OnHitNotify();
      }
   }
   return nullptr;
}

void UMontageAttackAnim::OnMontageEnded(UAnimMontage* montage, bool bInterrupted)
{
   OnAttackAnimFinished()->Broadcast();
}
