#pragma once

#include "AIStuff/Tasks/BTTask_QueryAction.h"
#include "BTTask_TurnQuery.generated.h"

/**
 * Turns towards a target as denoted by our query.
 */
UCLASS()
class MYPROJECT_API UBTTask_TurnQuery : public UBTTask_QueryAction
{
   GENERATED_BODY()

 public:
   UBTTask_TurnQuery();

 protected:
   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
   void                OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess) override;

   void OnTurnFinished(TSharedPtr<struct FEnvQueryResult> queryResult, class AUnitController* unitController, UBehaviorTreeComponent* ownerComp);
};
