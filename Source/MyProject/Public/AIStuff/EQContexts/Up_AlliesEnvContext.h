// Created 3/3/21 12:06 AM

#pragma once
#include "EnvironmentQuery/EnvQueryContext.h"
#include "Up_AlliesEnvContext.generated.h"

UCLASS()
class MYPROJECT_API UUp_AlliesEnvContext : public UEnvQueryContext
{
   GENERATED_BODY()

protected:
   void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
