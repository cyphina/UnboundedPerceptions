#include "MyProject.h"
#include "EQContexts/Up_VisibleEnemiesEnvContext.h"

#include "EnvQueryItemType_Unit.h"
#include "Unit.h"
#include "UnitController.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

void UUp_VisibleEnemiesEnvContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
   AUnit* queryOwnerUnit = Cast<AUnit>(QueryInstance.Owner.Get());

   if(queryOwnerUnit)
   {
      UEnvQueryItemType_Unit::SetContextHelper(ContextData, queryOwnerUnit->GetVisibleEnemies());
   }
}
