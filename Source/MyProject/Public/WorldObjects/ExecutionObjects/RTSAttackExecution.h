#pragma once

#include "RTSExecution.h"
#include "RTSAttackExecution.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
<<<<<<< HEAD
class URTSAttackExecution : public UObject, public IRTSExecution
{
   GENERATED_BODY()

 public:
   void Execute_Implementation() override;
=======
class URTSAttackExecution : public URTSExecution
{
   GENERATED_BODY()

 protected:
   /**
    * Call after the attack ended to let the AI system know
    * Some attacks are actually a series of animations, so we need to know when it's done by calling this
    */
   void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
>>>>>>> componentrefactor
};
