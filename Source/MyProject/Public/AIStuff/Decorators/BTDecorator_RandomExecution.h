// Created 3/8/21 9:21 AM

#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_RandomExecution.generated.h"

UCLASS()
class MYPROJECT_API UBTDecorator_RandomExecution : public UBTDecorator
{
   GENERATED_BODY()

 public:
   UBTDecorator_RandomExecution();

 protected:
   UPROPERTY(EditAnywhere, Meta = (ClampMin = "0", ClampMax = "100"))
   int chanceToExecute;

   virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

   FString GetStaticDescription() const override;
};
