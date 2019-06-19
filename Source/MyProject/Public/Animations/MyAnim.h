// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "IUnitState.h"
#include "MyAnim.generated.h"


/**
 * PS: In case you want to handle animation montages using C++, inside the AnimInstance C++ class you have the ability to call functions like Montage_Play / Montage_IsPlaying / Montage_Stop and
 * provide a UAnimMontage* parameter.
 */
class AUnit;

UCLASS()
class MYPROJECT_API UMyAnim : public UAnimInstance
{
   GENERATED_BODY()

   AUnit* unitRef;

 protected:
   /*True means we're currently in the air or falling*/
   UPROPERTY(EditAnywhere, BlueprintReadOnly)
   bool bIsFalling;

   // Holds current speed of our character
   UPROPERTY(EditAnywhere, BlueprintReadOnly)
   float movementSpeed;

   // Updates above properties
   UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
   void UpdateAnimationProperties();
   void NativeBeginPlay() override;
   void NativeUpdateAnimation(float deltaSeconds) override;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GetAnimationProperties")
   EUnitState GetCurrentState();
};
