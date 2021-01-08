#pragma once

#include "Animation/AnimInstance.h"
#include "RTSUnitAnimController.generated.h"

class AUnit;

/**
 * The Animation Controller (aka Animation Blueprint aka AnimationBP) is required on all units to handle its animation depending on
 * a custom state machine defined in blueprints. One off animations are montages, and the notify logic for the one-off montages live in this
 * class. They are functions with the prefix "AnimNotify_".
 */
UCLASS()
class MYPROJECT_API URTSUnitAnimController : public UAnimInstance
{
   GENERATED_BODY()

 protected:
   /** True means we're currently in the air or falling */
   UPROPERTY(EditAnywhere, BlueprintReadOnly)
   bool bIsFalling;

   /** Exposes the unit's movement speed to the animation controller */
   UPROPERTY(EditAnywhere, BlueprintReadOnly)
   float movementSpeed;

   UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
   AUnit* unitRef;

   void UpdateFallFlag();
   void UpdateAnimationProperties();

   void NativeBeginPlay() override final;
   void NativeUpdateAnimation(float deltaSeconds) override final;

 private:

   UFUNCTION()
   void AnimNotify_Hit() const;
};
