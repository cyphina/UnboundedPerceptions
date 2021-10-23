// Created 3/3/21 12:07 AM

#include "MyProject.h"
#include "EQContexts/Up_AlliesEnvContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Unit.h"
#include "EnvQueryItemType_Unit.h"
#include "UnitController.h"

void UUp_AlliesEnvContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
   AUnit* queryOwnerUnit = Cast<AUnit>(QueryInstance.Owner.Get());

   if(queryOwnerUnit)
   {
      UEnvQueryItemType_Unit::SetContextHelper(ContextData, queryOwnerUnit->GetAllies());
   }
}
