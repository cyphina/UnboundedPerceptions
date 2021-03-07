#include "MyProject.h"
#include "BTDecorator_AnyAlliesInRange.h"
#include "RTSVisionComponent.h"
#include "Unit.h"
#include "UnitController.h"

UBTDecorator_AnyAlliesInRange::UBTDecorator_AnyAlliesInRange()
{
   NodeName              = "Any Allies in Range";
   bAllowAbortNone       = false;
   bAllowAbortLowerPri   = false;
   bAllowAbortChildNodes = false;
}

bool UBTDecorator_AnyAlliesInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
   float range = rangeCutoff;

   AUnitController* unitController = Cast<AUnitController>(OwnerComp.GetAIOwner());
   if(range <= 0)
   {
      range = unitController->GetUnitOwner()->GetVisionComponent()->GetVisionRadius();
   }

   const TArray<AUnit*>& visibleEnemies = unitController->GetUnitOwner()->GetAllies();
   for(const AUnit* ally : visibleEnemies)
   {
      if(FVector::DistSquared2D(ally->GetActorLocation(), OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation()) < range * range)
      {
         return true;
      }
   }
   return false;
}

FString UBTDecorator_AnyAlliesInRange::GetStaticDescription() const
{
   return FString::Printf(TEXT("Checks: %f units away"), rangeCutoff);
}
