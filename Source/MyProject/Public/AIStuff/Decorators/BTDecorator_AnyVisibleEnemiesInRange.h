#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_AnyVisibleEnemiesInRange.generated.h"

/**
 * Figures out of any enemy units are visible (from ally's perspective)
 */
UCLASS()
class MYPROJECT_API UBTDecorator_AnyVisibleEnemiesInRange : public UBTDecorator
{
   GENERATED_BODY()

 public:
   UBTDecorator_AnyVisibleEnemiesInRange();

 protected:
   virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

   virtual FString GetStaticDescription() const override;

   UPROPERTY(EditAnywhere)
   float rangeCutoff;
};
