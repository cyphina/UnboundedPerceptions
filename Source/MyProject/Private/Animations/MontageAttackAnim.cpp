// Created 8/7/20 1:25 PM

#include "MyProject.h"
#include "MontageAttackAnim.h"
#include "GameFramework/Character.h"

UMontageAttackAnim::UMontageAttackAnim()
{
}

void UMontageAttackAnim::AttackNotify()
{

   OnAttackNotify().Broadcast();
}

void UMontageAttackAnim::PlayAttackAnimation(float playRate)
{
   ACharacter* character = Cast<ACharacter>(GetOuter());
   if(character)
   {
      character->PlayAnimMontage(attackMontage, playRate);
      if(UAnimInstance* animBP = character->GetMesh()->GetAnimInstance())
      {
         FOnMontageEnded montageEndedDelegate;
         montageEndedDelegate.BindUObject(this, &UMontageAttackAnim::OnMontageEnded);
         animBP->Montage_SetEndDelegate(montageEndedDelegate);
      }
   }
}

void UMontageAttackAnim::StopAttackAnimation()
{
   Cast<ACharacter>(GetOuter())->StopAnimMontage(attackMontage);
}

void UMontageAttackAnim::OnMontageEnded(UAnimMontage* montage, bool bInterrupted)
{
   OnAttackAnimFinished().Broadcast();
}
