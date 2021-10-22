#pragma once

#include "BTTask_QueryAction.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "BTTask_AttackTarget.generated.h"

/**
 * Attacks a target (requires a targetedattackcomponent).
 * Keeps attacking until that target dies or unit performing this task gets stunned.
 * If no target found with the specified query, then the node fails.
 * To handle X number of attacks, use a Time Limit decorator (requires some knowledge of attack speed).
 */
UCLASS()
class MYPROJECT_API UBTTask_AttackTarget : public UBTTask_QueryAction
{
   GENERATED_BODY()

 public:
   UBTTask_AttackTarget();

 protected:
   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
   virtual void        OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess) override;

 private:
   void OnTargetFindingFinished(TSharedPtr<struct FEnvQueryResult> queryResult, class AUnitController* AICon, UBehaviorTreeComponent* ownerComp);
};
