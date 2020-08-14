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
   UFUNCTION(BlueprintCallable)
   virtual void PlayAttackAnimation(float playRate = 1) = 0;

   UFUNCTION(BlueprintCallable)
   virtual void StopAttackAnimation() = 0;

   /** Bind to this to set a function to execute when the animation time corresponding to the damage proc occurs*/
   UFUNCTION(BlueprintCallable)
   virtual FOnHitNotify& OnAttackNotify() = 0;
};
