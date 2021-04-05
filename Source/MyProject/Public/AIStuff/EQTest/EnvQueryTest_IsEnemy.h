#pragma once

#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_IsEnemy.generated.h"

/**
 * Used to test if the things inside our query generator are enemy units or not. We already have the ability to create generates that filter for enemies, but this may be a good test for certain spells
 * that can target allies and enemies.
 */
UCLASS()
class MYPROJECT_API UEnvQueryTest_IsEnemy : public UEnvQueryTest
{
   GENERATED_BODY()

   UEnvQueryTest_IsEnemy();

   virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

   virtual FText GetDescriptionTitle() const override;

   virtual FText GetDescriptionDetails() const override;
};
