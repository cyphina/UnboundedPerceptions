#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BroadcastAIMessage.generated.h"

/**
 * Used to send AI messages to units surrounding this one (primitive AI communication).
 */

UCLASS()
class MYPROJECT_API UBTTask_BroadcastAIMessage : public UBTTaskNode
{
   GENERATED_BODY()

   UBTTask_BroadcastAIMessage();

 protected:
   UPROPERTY(EditAnywhere)
   FGameplayTag messageNametag;

   UPROPERTY(EditAnywhere)
   float radius;

   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

 private:
   static const inline FString tagPrefix = "Combat.AI.Message.";
};
