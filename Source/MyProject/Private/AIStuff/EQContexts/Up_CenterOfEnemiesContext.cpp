// Created 3/3/21 11:17 PM

#include "MyProject.h"
#include "Up_CenterOfEnemiesContext.h"

#include "Unit.h"
#include "UnitController.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

void UUp_CenterOfEnemiesContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
   AUnit*           queryOwnerUnit              = Cast<AUnit>(QueryInstance.Owner.Get());
   FVector          centerPointOfVisibleEnemies = FVector::ZeroVector;

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
