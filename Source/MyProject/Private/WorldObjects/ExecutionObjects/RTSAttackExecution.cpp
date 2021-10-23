#include "RTSAttackExecution.h"

#include "BrainComponent.h"
#include "UnitController.h"
#include "UnitMessages.h"

void URTSAttackExecution::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
   Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
   const FAIMessage msg(UnitMessages::AIMessage_AttackExecuted, this);
   FAIMessage::Send(Cast<AUnitController>(GetOuter()), msg);
}
