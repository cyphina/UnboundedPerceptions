#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_FastOverlapEnemy.generated.h"

/**
 * Unlike overlap, this just checks to see how many enemies are in radial distance and it only checks on enemies that are visible.
 */
UCLASS()
class MYPROJECT_API UEnvQueryTest_FastOverlapEnemy : public UEnvQueryTest
{
   GENERATED_BODY()

 public:
   UEnvQueryTest_FastOverlapEnemy();

 protected:
   virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

   /**
    * @brief If there are no query params for the overlap radius, this one is used.
    */
   UPROPERTY(EditAnywhere, Category = "QueryParams")
   float defaultOverlapRadius;

   FText GetDescriptionDetails() const override;
};
