// Created 8/7/20 1:24 PM

#pragma once
#include "AttackAnim.h"
#include "MontageAttackAnim.generated.h"

class AUnit;

/**
  * ! CALL SETUP BEFORE USING !
  * To have a notification event trigger, ensure we have an event named
  * AnimNotify_<EVENTNAME> in our animation BP. We can do this in C++ too - just add
  * it to the UAnimInstance derived type. Ensure our event is a UFUNCTION.
  * In our standard RTSUnitAnim
  */
UCLASS()
class MYPROJECT_API UMontageAttackAnim : public UObject, public IAttackAnim
{
   GENERATED_BODY()

 public:
   UMontageAttackAnim();

   void          PlayAttackAnimation(float playRate) override;
   void          StopAttackAnimation() override;
   FOnHitNotify& OnAttackNotify() override;

 protected:
   AUnit* unitRef;

   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Meta = (AllowPrivateAccess = true))
   UAnimMontage* attackMontage;

 private:
   AUnit*       animatedUnit;
   FOnHitNotify OnAttackNotifyEvent;
};
