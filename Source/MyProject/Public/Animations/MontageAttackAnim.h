// Created 8/7/20 1:24 PM

#pragma once
#include "AttackAnim.h"
#include "MontageAttackAnim.generated.h"

class AUnit;

/**
  * To have a notification event trigger, ensure we have an event named
  * AnimNotify_<EVENTNAME> in our animation BP. We can do this in C++ too - just add
  * it to the UAnimInstance derived type. Ensure our event is a UFUNCTION.
  */
UCLASS()
class MYPROJECT_API UMontageAttackAnim : public UObject, public IAttackAnim
{
   GENERATED_BODY()

 public:
   UFUNCTION()
   void AttackNotify() override {}

   void PlayAttackAnimation(ACharacter* characterToPlayMontageOn, float playRate) override;
   void StopAttackAnimation(ACharacter* characterToStopMontageOn) override;

   FOnHitNotify*          OnAttackNotify(ACharacter* characterToNotify) override;
   FOnAttackAnimFinished* OnAttackAnimFinished() override { return &OnAttackAnimFinishedEvent; }

 private:
   UMontageAttackAnim();

   void OnMontageEnded(UAnimMontage* montage, bool bInterrupted);

   UAnimMontage* GetAttackMontage(ACharacter* characterWithMontage);

   UPROPERTY()
   UAnimMontage* loadedAnimMontage = nullptr;

   FOnHitNotify          OnAttackNotifyEvent;
   FOnAttackAnimFinished OnAttackAnimFinishedEvent;
};
