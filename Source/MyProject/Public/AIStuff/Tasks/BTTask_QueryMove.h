#pragma once

#include "AIStuff/Tasks/BTTask_QueryAction.h"
#include "BTTask_QueryMove.generated.h"

/**
 * Runs a query on where to move, and then moves there.
 */
UCLASS()
class MYPROJECT_API UBTTask_QueryMove : public UBTTask_QueryAction
{
   GENERATED_BODY()

 public:
   UBTTask_QueryMove();

 protected:
   /**
    * @brief How close should we stop the move?
    */
   UPROPERTY(EditAnywhere)
   float stopRange;

   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
   virtual void        OnMessage(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, FName message, int32 requestID, bool bSuccess) override;

 private:
   void OnMovePositionFound(TSharedPtr<struct FEnvQueryResult> queryResult, class AUnitController* AICon, UBehaviorTreeComponent* ownerComp);
};
