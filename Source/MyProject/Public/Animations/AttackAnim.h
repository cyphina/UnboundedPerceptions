// Created 8/7/20 6:21 AM

#pragma once
#include "AttackAnim.generated.h"

DECLARE_EVENT(UAttackAnim, FOnHitNotify);

UINTERFACE(MinimalAPI)
class UAttackAnim : public UInterface
{
   GENERATED_UINTERFACE_BODY()
};

class IAttackAnim
{
   GENERATED_IINTERFACE_BODY()

 public:
   virtual void PlayAttackAnimation(float playRate = 1) = 0;

   virtual void StopAttackAnimation() = 0;

   /** Attack animations have a notify (like when the sword swing connects with the enemy) in which this function should fire off */
   virtual void AttackNotify() = 0;

   /** Bind to this to set a function to execute when the animation time corresponding to the damage proc occurs*/
   virtual FOnHitNotify& OnAttackNotify() = 0;
};
