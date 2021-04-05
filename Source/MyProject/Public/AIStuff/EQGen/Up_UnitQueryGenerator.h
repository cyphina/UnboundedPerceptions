#pragma once

#include "EnvironmentQuery/EnvQueryGenerator.h"
#include "Up_UnitQueryGenerator.generated.h"

class AUnit;

/**
 * Generator that returns every single unit in the level.
 */
UCLASS()
class MYPROJECT_API UUp_UnitQueryGenerator : public UEnvQueryGenerator
{
   GENERATED_BODY()

   UUp_UnitQueryGenerator();

   /** Generates the items */
   virtual void GenerateItems(FEnvQueryInstance& queryInstance) const override;
};
