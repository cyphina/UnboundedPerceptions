// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttackAnim.h"
#include "Animation/AnimInstance.h"
#include "RTSUnitAnim.generated.h"

class AUnit;

/**
 * PS: In case you want to handle animation montages using C++, inside the AnimInstance C++ class you have the ability to call functions like
 * Montage_Play / Montage_IsPlaying / Montage_Stop and provide a UAnimMontage* parameter.
 */
UCLASS()
class MYPROJECT_API URTSUnitAnim : public UAnimInstance, public IAttackAnim
{
   GENERATED_BODY()

 public:
   void          PlayAttackAnimation(float playRate) override;
   void          StopAttackAnimation() override;
   FOnHitNotify& OnAttackNotify() override { return OnAttackNotifyEvent; };

 protected:
   /** The attack animation has a notify which triggers this function. */
   UFUNCTION(BlueprintCallable)
   void AttackNotify() override;

   /**True means we're currently in the air or falling*/
   UPROPERTY(EditAnywhere, BlueprintReadOnly)
   bool bIsFalling;

   /**Holds current movement speed of our character*/
   UPROPERTY(EditAnywhere, BlueprintReadOnly)
   float movementSpeed;

   UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
   AUnit* unitRef;

   UPROPERTY(EditAnywhere, Category = "Animations")
   UAnimMontage* moveAnimation = nullptr;

   UPROPERTY(EditAnywhere, Category = "Animations")
   UAnimMontage* attackAnimation = nullptr;

   UPROPERTY(EditAnywhere, Category = "Animations")
   UAnimMontage* castAnimation = nullptr;

   void UpdateAnimationProperties();
   void NativeBeginPlay() override final;
   void NativeUpdateAnimation(float deltaSeconds) override final;

 private:
   FOnHitNotify OnAttackNotifyEvent;
};
