#include "MyProject.h"
#include "Decorators/BTDecorator_AnyVisibleEnemiesInRange.h"
#include "Unit.h"
#include "RTSGameState.h"
#include "RTSVisionComponent.h"
#include "SpellDataLibrary.h"
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

   const TArray<AUnit*>& visibleEnemies = unitController->GetUnitOwner()->GetVisibleEnemies();

   if(!visibleEnemies.Num())
   {
      if(IsInversed())
      {
         return true;
      }
      else
      {
         return false;
      }
   }

   for(const AUnit* enemy : visibleEnemies)
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
   if(IsInversed())
   {
      if(rangeCutoff >= 0)
      {
         return FString::Printf(TEXT("Units over %f units away"), rangeCutoff);
      }
      else
      {
         return FString::Printf(TEXT("Units outside vision radius?"));
      }
   }
   else
   {
      if(rangeCutoff >= 0)
      {
         return FString::Printf(TEXT("Units inside %f unit radius"), rangeCutoff);
      }
      else
      {
         return FString::Printf(TEXT("Units within vision radius?"));
      }
   }
}
