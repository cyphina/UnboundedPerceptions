// Created 3/3/21 11:13 PM

#pragma once
#include "EnvironmentQuery/EnvQueryContext.h"
#include "Up_CenterOfEnemiesContext.generated.h"

UCLASS()
class MYPROJECT_API UUp_CenterOfEnemiesContext : public UEnvQueryContext
{
   GENERATED_BODY()

protected:
   void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
