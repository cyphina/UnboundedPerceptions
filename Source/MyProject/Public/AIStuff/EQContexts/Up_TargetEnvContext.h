// Created 3/3/21 12:27 AM

#pragma once
#include "EnvironmentQuery/EnvQueryContext.h"
#include "Up_TargetEnvContext.generated.h"

UCLASS()
class MYPROJECT_API UUp_TargetEnvContext : public UEnvQueryContext
{
   GENERATED_BODY()

   void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};

