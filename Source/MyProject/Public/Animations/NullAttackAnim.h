// Created 8/6/20 9:22 PM

#pragma once
#include "AttackAnim.h"
#include "Animation/AnimInstance.h"
#include "NullAttackAnim.generated.h"

/** Null object pattern
 * When there's no attack animation on a unit, plays this timeline to fake an animation which
 * triggers a notify sometime during the timeline representing the hit notify.
 * If the animation represents a swing of the sword, the hit notify represents when the sword touches the enemy.
 */
UCLASS()
class MYPROJECT_API UNullAttackAnim : public UObject, public IAttackAnim
{
   GENERATED_BODY()

 public:
   /** By default the parent class looks for a mesh if we don't have one so we should be fine to set this */
   UNullAttackAnim();

   void PlayAttackAnimation(float playRate) override;
   void StopAttackAnimation() override;

   FOnHitNotify&          OnAttackNotify() override { return OnAttackNotifyEvent; }
   FOnAttackAnimFinished& OnAttackAnimFinished() override { return OnAttackAnimFinishedEvent; }

   float GetMockAnimationLength() const;

   void SetMockAnimationLength(float newLength);

   /** Requires us to create a new timeline since there's nothing in FTimeline's API to delete an event*/
   void SetAttackEventTime(float newTime);

 protected:
   /**
    * Mock notification function broadcasts the hit delegate delegate
    */
   UFUNCTION()
   void AttackNotify() override;

   void SetupHitTimer();

 private:
   FOnHitNotify          OnAttackNotifyEvent;
   FOnAttackAnimFinished OnAttackAnimFinishedEvent;

   FTimerHandle mockAnimationTimer;
   FTimerHandle hitNotifyTimer;

   float hitNotifyTime;
   float timelineLength;
};
