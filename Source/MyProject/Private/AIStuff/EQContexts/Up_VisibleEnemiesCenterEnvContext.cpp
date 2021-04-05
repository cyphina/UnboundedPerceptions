#include "MyProject.h"
#include "EQContexts/Up_VisibleEnemiesCenterEnvContext.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "UnitController.h"
#include "RTSGameState.h"
#include "Unit.h"

void UUp_VisibleEnemyCenterEnvContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
   AUnit*  queryOwnerUnit              = Cast<AUnit>(QueryInstance.Owner.Get());
   FVector centerPointOfVisibleEnemies = FVector::ZeroVector;

   const auto& visibleEnemies = queryOwnerUnit->GetVisibleEnemies();

   for(AUnit* enemy : visibleEnemies)
   {
      centerPointOfVisibleEnemies += enemy->GetActorLocation();
   }

   if(visibleEnemies.Num() != 0)
   {
      centerPointOfVisibleEnemies /= visibleEnemies.Num();
   }

   UEnvQueryItemType_Point::SetContextHelper(ContextData, centerPointOfVisibleEnemies);
}
