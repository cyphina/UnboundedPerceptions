#pragma once

#include "StatEnums.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_VitalThreshold.generated.h"

/**
 * Checks to see if one of a unit's vital is within a certain threshold
 */
UCLASS()
class MYPROJECT_API UBTDecorator_VitalThreshold : public UBTDecorator
{
   GENERATED_BODY()

 public:
   UBTDecorator_VitalThreshold();

 protected:
   UPROPERTY(EditAnywhere)
   EVitals Vital;

   UPROPERTY(EditAnywhere, Meta = (ClampMIn = "0", ClampMax = "100"))
   float Threshold;

   virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

   FString GetStaticDescription() const override;
};
