// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "IUnitState.h"
#include "RTSUnitAnim.generated.h"

class AUnit;

// Event called when the notify occurs for an animation corresponding to the swing of a weapon aka an attack event
DECLARE_EVENT(URTSUnitAnim, OnAnimFinishSwing);

/**
 * PS: In case you want to handle animation montages using C++, inside the AnimInstance C++ class you have the ability to call functions like Montage_Play / Montage_IsPlaying / Montage_Stop and
 * provide a UAnimMontage* parameter.
 */
UCLASS()
class MYPROJECT_API URTSUnitAnim : public UAnimInstance
{
   GENERATED_BODY()

   UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
   AUnit* unitRef;

 protected:

   /*True means we're currently in the air or falling*/
   UPROPERTY(EditAnywhere, BlueprintReadOnly)
   bool bIsFalling;

   /**Holds current mvoement speed of our character*/
   UPROPERTY(EditAnywhere, BlueprintReadOnly)
   float movementSpeed;

   // Updates above properties
   void UpdateAnimationProperties();
   void NativeBeginPlay() override final;
   void NativeUpdateAnimation(float deltaSeconds) override final;

   UFUNCTION(BlueprintCallable)
   void OnAttackNotify();

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GetAnimationProperties")
   EUnitState GetCurrentState();

public:

   // Called when the animation calls an attack event
   OnAnimFinishSwing OnFinishSwingEvent;
};
