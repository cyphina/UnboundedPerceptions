#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveRandomPointAroundTarget.generated.h"

/**
 * Task that moves the unit to a random point around a previously found target.
 * Requires the AI to have not cleared out its last target.
 */

class AUnitController;

UCLASS()
class MYPROJECT_API UBTTask_MoveRandomPointAroundTarget : public UBTTaskNode
{
   GENERATED_BODY()

 public:
   UBTTask_MoveRandomPointAroundTarget();

 protected:
   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* nodeMemory) override;
   virtual void        OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess) override;
   void                OnFoundRandomPathablePoint(uint32 QueryID, ENavigationQueryResult::Type QueryType, FNavPathSharedPtr ResultPath, UBehaviorTreeComponent* OwnerComp,
                                                  AUnitController* UnitController, FAIMoveRequest MoveReq);
   virtual FString     GetStaticDescription() const override;

   /** Move radius will default to the attack range of the unit if < 0 */
   UPROPERTY(EditAnywhere)
   float moveRadius = -1;
};
