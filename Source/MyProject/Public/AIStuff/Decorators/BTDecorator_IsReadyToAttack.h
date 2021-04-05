
#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsReadyToAttack.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API UBTDecorator_IsReadyToAttack : public UBTDecorator
{
   GENERATED_BODY()

   UBTDecorator_IsReadyToAttack();
   virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
