#pragma once

#include "EnvironmentQuery/EnvQueryContext.h"
#include "Up_VisibleEnemiesCenterEnvContext.generated.h"

class AUnit;

class AUnit;

/**
 * Provides to the environment query the center of all the visible enemy units
 */
UCLASS()
class MYPROJECT_API UUp_VisibleEnemyCenterEnvContext : public UEnvQueryContext
{
   GENERATED_BODY()

<<<<<<< HEAD:Source/MyProject/Public/AIStuff/EQGen/FindVisibleEnemiesCenterContext.h
 public:
=======
public:
>>>>>>> componentrefactor:Source/MyProject/Public/AIStuff/EQContexts/Up_VisibleEnemiesCenterEnvContext.h
   void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

 private:
   const TSet<AUnit*>& GetVisibleEnemies(const FEnvQueryInstance& queryInstance) const;
};
