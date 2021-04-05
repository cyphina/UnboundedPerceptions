#pragma once

#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_DamageScore.generated.h"

class AUnit;

/**
 * Used to score how dangerous the enemies around ups are (based on stats, damage dealt recently, etc).
 */
UCLASS()
class MYPROJECT_API UEnvQueryTest_DamageScore : public UEnvQueryTest
{
   GENERATED_BODY()

   static const int STAT_AFF_START_INDEX = 6;  // index of affinities in our stats
   static const int STAT_AFF_END_INDEX   = 18; // last index of the last element's affinity

   UEnvQueryTest_DamageScore();

   virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

   virtual FText GetDescriptionTitle() const override;

   virtual FText GetDescriptionDetails() const override;

   int FindNumOffensiveAbilities(AUnit* unit) const;

   int GetHighestOffensiveAttribute(AUnit* unit) const;
};
