#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_QueryMove.generated.h"

/**
 * Runs a query on where to move, and then moves there.
 */
UCLASS()
class MYPROJECT_API UBTTask_QueryMove : public UBTTaskNode
{
   GENERATED_BODY()

 public:
   UBTTask_QueryMove();

 protected:
   /**
    * @brief Used to choose the location of where to move.
    */
   UPROPERTY(EditAnywhere)
   class UEnvQuery* positionFindingLogic;

   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
   virtual void        OnMessage(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, FName message, int32 requestID, bool bSuccess) override;
   FString             GetStaticDescription() const override;

 private:
   void OnMovePositionFound(TSharedPtr<struct FEnvQueryResult> queryResult, class AUnitController* AICon, UBehaviorTreeComponent* ownerComp);
};
