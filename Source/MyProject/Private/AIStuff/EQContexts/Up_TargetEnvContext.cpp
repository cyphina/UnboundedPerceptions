// Created 3/3/21 12:29 AM

#include "MyProject.h"
#include "EQContexts/Up_TargetEnvContext.h"
#include "EnvQueryItemType_Unit.h"
#include "TargetComponent.h"
#include "Unit.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

void UUp_TargetEnvContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
   AUnit* queryOwnerUnit = Cast<AUnit>(QueryInstance.Owner.Get());
   if(queryOwnerUnit)
   {
      if(queryOwnerUnit->GetTargetComponent()->IsTargetingUnit())
      {
         UEnvQueryItemType_Unit::SetContextHelper(ContextData, queryOwnerUnit->GetTargetComponent()->GetTargetUnit());
      }
   }
}
