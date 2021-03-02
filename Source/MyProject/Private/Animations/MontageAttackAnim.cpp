// Created 8/7/20 1:25 PM

#include "MyProject.h"
#include "MontageAttackAnim.h"

#include "RTSUnitAnim.h"
#include "GameFramework/Character.h"

UMontageAttackAnim::UMontageAttackAnim()
{
}

void UMontageAttackAnim::PlayAttackAnimation(ACharacter* characterToPlayMontageOn, float playRate)
{
   if(characterToPlayMontageOn)
   {
      if(const auto attackMontage = GetAttackMontage(characterToPlayMontageOn))
      {
         if(UAnimInstance* animBP = characterToPlayMontageOn->GetMesh()->GetAnimInstance())
         {
            characterToPlayMontageOn->PlayAnimMontage(attackMontage, playRate);
            FOnMontageEnded montageEndedDelegate;
            montageEndedDelegate.BindUObject(this, &UMontageAttackAnim::OnMontageEnded);
            animBP->Montage_SetEndDelegate(montageEndedDelegate, attackMontage);
         }
      }
   }
}

void UMontageAttackAnim::StopAttackAnimation(ACharacter* characterToStopMontageOn)
{
   if(const auto attackMontage = GetAttackMontage(characterToStopMontageOn))
   {
      characterToStopMontageOn->StopAnimMontage(attackMontage);
   }
}

FOnHitNotify* UMontageAttackAnim::OnAttackNotify(ACharacter* characterToNotify)
{
   if(characterToNotify)
   {
      if(URTSUnitAnim* unitAnim = Cast<URTSUnitAnim>(characterToNotify->GetMesh()->GetAnimInstance()))
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

UAnimMontage* UMontageAttackAnim::GetAttackMontage(ACharacter* characterWithMontage) const
{
   if(UAnimInstance* animBP = characterWithMontage->GetMesh()->GetAnimInstance())
   {
      if(URTSUnitAnim* unitAnim = Cast<URTSUnitAnim>(animBP))
      {
         const TSoftObjectPtr<UAnimMontage> animMontageToLoad = unitAnim->GetAnimMontage(FGameplayTag::RequestGameplayTag("Skill.Name.AutoAttack"));
         return animMontageToLoad.LoadSynchronous();
      }
   }
   return nullptr;
}
