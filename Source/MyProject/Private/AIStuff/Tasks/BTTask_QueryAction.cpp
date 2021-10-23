// Created 3/6/21 8:2 PM

#include "MyProject.h"
#include "BTTask_QueryAction.h"

#include "EnvironmentQuery/EnvQuery.h"

UBTTask_QueryAction::UBTTask_QueryAction()
{
   queryRunMode = EEnvQueryRunMode::Type::SingleResult;
}

FString UBTTask_QueryAction::GetQueryName() const
{
   FString queryName = actionQuery->GetName();
   queryName.RemoveFromStart("UpEQS_");
   return queryName;
}

FString UBTTask_QueryAction::GetStaticDescription() const
{
   if(actionQuery)
   {
      return FString::Printf(TEXT("Query:\n%s"), *GetQueryName());
   }
   else
   {
      return FString::Printf(TEXT("Prior Task Target"));
   }
}
