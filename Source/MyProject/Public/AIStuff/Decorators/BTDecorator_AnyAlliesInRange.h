#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_AnyAlliesInRange.generated.h"

/**
 * Figures out of any enemy units are visible (from enemy's perspective)
 */
UCLASS()
class MYPROJECT_API UBTDecorator_AnyAlliesInRange : public UBTDecorator
{
   GENERATED_BODY()

 public:
   UBTDecorator_AnyAlliesInRange();

 protected:
   virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

   FString GetStaticDescription() const override;

   UPROPERTY(EditAnywhere)
   float rangeCutoff;
};
