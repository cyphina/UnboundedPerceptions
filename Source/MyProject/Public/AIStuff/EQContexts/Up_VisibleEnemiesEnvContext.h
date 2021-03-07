#pragma once
#include "EnvironmentQuery/EnvQueryContext.h"
#include "Up_VisibleEnemiesEnvContext.generated.h"

/**
 * Context that returns all the units considered enemies for a specific unit.
 */
UCLASS()
class MYPROJECT_API UUp_VisibleEnemiesEnvContext : public UEnvQueryContext
{
   GENERATED_BODY()

protected:
   void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
