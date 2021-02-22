// Created 8/7/20 6:21 AM

#pragma once
#include "AttackAnim.generated.h"

DECLARE_EVENT(UAttackAnim, FOnHitNotify);
DECLARE_EVENT(UAttackAnim, FOnAttackAnimFinished);

UINTERFACE(MinimalAPI)
class UAttackAnim : public UInterface
{
   GENERATED_BODY()
};

class IAttackAnim
{
   GENERATED_BODY()

 public:
   virtual void PlayAttackAnimation(float playRate = 1) = 0;

   virtual void StopAttackAnimation() = 0;

   /** Attack animations have a notify (like when the sword swing connects with the enemy) in which this function should fire off. The animation blueprint should trigger this function */
   virtual void AttackNotify() = 0;

   /** Bind to this to set a function in C++ to execute when the animation time corresponding to the damage proc occurs */
   virtual FOnHitNotify* OnAttackNotify() = 0;

   /** When the animation finishes playin (as opposed to when the hit event occurs)  */
   virtual FOnAttackAnimFinished* OnAttackAnimFinished() = 0;
};
