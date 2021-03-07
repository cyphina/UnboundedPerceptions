// Created 3/6/21 8:2 PM

#include "MyProject.h"
#include "BTTask_QueryAction.h"

#include "EnvironmentQuery/EnvQuery.h"

UBTTask_QueryAction::UBTTask_QueryAction()
{
   queryRunMode = EEnvQueryRunMode::Type::SingleResult;
}

FString UBTTask_QueryAction::GetStaticDescription() const
{
   if(actionQuery)
   {
      return FString::Printf(TEXT("Using query\n%s to find target"), *actionQuery->GetQueryName().ToString());
   }
   else
   {
      return FString::Printf(TEXT("Using target from prior task"));
   }
}
