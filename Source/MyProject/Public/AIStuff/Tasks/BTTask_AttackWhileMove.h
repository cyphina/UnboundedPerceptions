#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_AttackWhileMove.generated.h"

/**
 * Use only on enemies ai!!!
 * Task that moves a target but continuously attacks (executing the custom attack execution in the unit controller) the enemy if the target is in range while moving.
 * Completely ignores the targeted attack protocol, meaning we need to manually setup delay between attacks.
 */

class AUnitController;

struct FBTAttackMoveMemory {
   AUnitController* AICon = nullptr;
   FTimerHandle attackTime;
};

UCLASS()
class MYPROJECT_API UBTTask_AttackWhileMove : public UBTTask_BlackboardBase
{
   GENERATED_BODY()

   UPROPERTY(EditAnywhere, Meta=(ClampMin ="0", ClampMax="120"))
   float attackDelay = 1.f;

   UPROPERTY(EditAnywhere, Meta=(ClampMin ="0", ClampMax="20"))
   float delayVariance = 0.5f;
   
   UBTTask_AttackWhileMove();

   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
   uint16              GetInstanceMemorySize() const override;
   virtual void        OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess) override;
};
