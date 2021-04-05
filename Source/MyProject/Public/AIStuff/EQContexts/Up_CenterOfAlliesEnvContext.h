// Created 3/3/21 05:53 AM

#pragma once
#include "EnvironmentQuery/EnvQueryContext.h"
#include "Up_CenterOfAlliesEnvContext.generated.h"

UCLASS()
class MYPROJECT_API UUp_CenterOfAlliesEnvContext : public UEnvQueryContext
{
   GENERATED_BODY()

   void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
