#pragma once

#include "EnvironmentQuery/EnvQueryContext.h"
#include "Up_VisibleEnemiesCenterEnvContext.generated.h"

class AUnit;

/**
 * Provides to the environment query the center of all the visible enemy units
 */
UCLASS()
class MYPROJECT_API UUp_VisibleEnemyCenterEnvContext : public UEnvQueryContext
{
   GENERATED_BODY()

public:
   void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
