// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttackAnim.h"
#include "Animation/AnimInstance.h"
#include "RTSUnitAnim.generated.h"

class AUnit;

/**
 * PS: In case you want to handle animation montages using C++, inside the AnimInstance C++ class you have the ability to call functions like
 * Montage_Play / Montage_IsPlaying / Montage_Stop and provide a UAnimMontage* parameter. The character class exposes this API too.
 * We can expose notify functions (as UFUNCTIONs()) to call here as well when the notifies actually get hit in UE4.
 */
UCLASS()
class MYPROJECT_API URTSUnitAnim : public UAnimInstance
{
   GENERATED_BODY()

 protected:
   void AnimNotify_OnAttack(UAnimNotify* notify);

   /**True means we're currently in the air or falling*/
   UPROPERTY(EditAnywhere, BlueprintReadOnly)
   bool bIsFalling;

   /**Holds current movement speed of our character*/
   UPROPERTY(EditAnywhere, BlueprintReadOnly)
   float movementSpeed;

   UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
   AUnit* unitRef;

   void UpdateAnimationProperties();
   void NativeBeginPlay() override final;
   void NativeUpdateAnimation(float deltaSeconds) override final;

 private:
   FOnHitNotify OnAttackNotifyEvent;
};
