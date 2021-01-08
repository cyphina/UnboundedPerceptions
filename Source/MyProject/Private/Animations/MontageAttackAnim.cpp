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
   Cast<ACharacter>(GetOuter())->PlayAnimMontage(attackMontage, playRate);
}

void UMontageAttackAnim::StopAttackAnimation()
{
   Cast<ACharacter>(GetOuter())->StopAnimMontage(attackMontage);
}

FOnHitNotify& UMontageAttackAnim::OnAttackNotify()
{
   return OnAttackNotifyEvent;
}
