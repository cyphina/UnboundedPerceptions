#include "MyProject.h"
#include "Decorators/BTDecorator_AnyVisibleEnemiesInRange.h"
#include "Unit.h"
#include "RTSGameState.h"
#include "RTSVisionComponent.h"
#include "UnitController.h"

UBTDecorator_AnyVisibleEnemiesInRange::UBTDecorator_AnyVisibleEnemiesInRange()
{
   NodeName              = "Any Visible Enemies";
   bAllowAbortNone       = false;
   bAllowAbortLowerPri   = false;
   bAllowAbortChildNodes = false;
}

bool UBTDecorator_AnyVisibleEnemiesInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
   AUnitController* unitController = Cast<AUnitController>(OwnerComp.GetAIOwner());
   if(rangeCutoff <= 0)
   {
      return unitController->GetUnitOwner()->GetVisibleEnemies().Num() > 0;
   }

   const TArray<AUnit*>& enemies = unitController->GetUnitOwner()->GetEnemies();
   for(const AUnit* enemy : enemies)
   {
      if(FVector::DistSquared2D(enemy->GetActorLocation(), OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation()) < rangeCutoff * rangeCutoff)
      {
         return true;
      }
   }
   return false;
}

FString UBTDecorator_AnyVisibleEnemiesInRange::GetStaticDescription() const
{
   if(rangeCutoff >= 0)
   {
      return FString::Printf(TEXT("Checks: %f units away"), rangeCutoff);
   }
   else
   {
      return FString::Printf(TEXT("Checks for units in vision radius"));
   }
}
