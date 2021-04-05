<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttackAnim.h"
#include "Animation/AnimInstance.h"
=======
#pragma once
#include "AttackAnim.h"
#include "Animation/AnimInstance.h"
#include "RTSMontageMap.h"
>>>>>>> componentrefactor
#include "RTSUnitAnim.generated.h"

class AUnit;

/**
<<<<<<< HEAD
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
=======
 * Base class for all Unit animation blueprints for Up.
 * PS: In case you want to handle animation montages using C++, inside the AnimInstance C++ class you have the ability to call functions like
 * Montage_Play / Montage_IsPlaying / Montage_Stop and provide a UAnimMontage* parameter. The character class exposes this API too.
 * We can expose notify functions (as UFUNCTIONs()) to call here as well when the notifies actually get hit in UE4.
 */
UCLASS()
class MYPROJECT_API URTSUnitAnim : public UAnimInstance
{
   GENERATED_BODY()

public:
   FOnHitNotify& OnHitNotify() { return OnHitNotifyEvent; }

   UFUNCTION(BlueprintCallable, BlueprintPure)
   TSoftObjectPtr<UAnimMontage> GetAnimMontage(FGameplayTag tagToQuery) const;

protected:
   // UFUNCTION()
   // void AnimNotify_OnAttack(UAnimNotify* notify);

   /** True means we're currently in the air or falling */
   UPROPERTY(EditAnywhere, BlueprintReadOnly)
   bool bIsFalling;

   /** Holds current movement speed of our character */
   UPROPERTY(EditAnywhere, BlueprintReadOnly)
   float movementSpeed;

   /** Allows us to check if we have a valid montage for the scenario associated with a gameplay tag (usually a spell name) */
   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
   TSubclassOf<URTSMontageMap> montageMap;
>>>>>>> componentrefactor

   void UpdateAnimationProperties();
   void NativeBeginPlay() override final;
   void NativeUpdateAnimation(float deltaSeconds) override final;

<<<<<<< HEAD
 private:
   FOnHitNotify OnAttackNotifyEvent;
=======
private:
   UFUNCTION()
   void OnAnimationNotify(FName notifyName, const FBranchingPointNotifyPayload& branchingPointPayload);
      
   UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
   AUnit* unitRef;

   FOnHitNotify OnHitNotifyEvent;
>>>>>>> componentrefactor
};
