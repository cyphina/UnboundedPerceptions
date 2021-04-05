#pragma once
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Stop.generated.h"

/**
 * Stops a unit from performing an action, but behavior tree continues to run and this node always succeeds if stop succeeds. 
 */
UCLASS()
class MYPROJECT_API UBTTask_Stop : public UBTTaskNode
{
   GENERATED_BODY()

   UBTTask_Stop();
	
   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
};
