// Created 3/3/21 06:00 PM

#include "MyProject.h"
#include "Up_CenterOfAlliesEnvContext.h"

#include "Unit.h"
#include "UnitController.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

void UUp_CenterOfAlliesEnvContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
   AUnit*  queryOwnerUnit      = Cast<AUnit>(QueryInstance.Owner.Get());
   FVector centerPointOfAllies = FVector::ZeroVector;

   const auto& allies = queryOwnerUnit->GetAllies();

   for(AUnit* enemy : allies)
   {
      centerPointOfAllies += enemy->GetActorLocation();
   }

   if(allies.Num() != 0)
   {
      centerPointOfAllies /= allies.Num();
   }

   UEnvQueryItemType_Point::SetContextHelper(ContextData, centerPointOfAllies);
}
