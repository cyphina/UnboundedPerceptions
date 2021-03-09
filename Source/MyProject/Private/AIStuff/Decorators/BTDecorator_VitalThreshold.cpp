#include "MyProject.h"
#include "BTDecorator_VitalThreshold.h"
#include "Unit.h"
#include "UnitController.h"
#include "UpStatComponent.h"

UBTDecorator_VitalThreshold::UBTDecorator_VitalThreshold()
{
   NodeName              = "Check Vital Rule";
   bAllowAbortNone       = false;
   bAllowAbortLowerPri   = false;
   bAllowAbortChildNodes = false;
}

bool UBTDecorator_VitalThreshold::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
   AUnitController* unitController = Cast<AUnitController>(OwnerComp.GetAIOwner());
   if(unitController)
   {
      if(UUpStatComponent* statComp = unitController->GetUnitOwner()->GetStatComponent())
      {
         const float curVitalValue = statComp->GetVitalCurValue(Vital);
         const float maxVitalValue = statComp->GetVitalBaseValue(Vital);

         if((curVitalValue * 100 / maxVitalValue) < Threshold)
         {
            return true;
         }
      }
   }
   return false;
}

FString UBTDecorator_VitalThreshold::GetStaticDescription() const
{
   FString description = FString::Printf(TEXT("%s threshold: %f"), *UEnum::GetDisplayValueAsText(Vital).ToString(), Threshold);
   if(IsInversed())
   {
      description += "\n(Inversed)";
   }
   return description;
}
