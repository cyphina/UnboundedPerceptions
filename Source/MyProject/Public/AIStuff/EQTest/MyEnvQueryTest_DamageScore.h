// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "MyEnvQueryTest_DamageScore.generated.h"

/**
 * I use overlap because it
 */

class AUnit;

UCLASS()
class MYPROJECT_API UMyEnvQueryTest_DamageScore : public UEnvQueryTest
{
   GENERATED_BODY()

   static const int STAT_AFF_START_INDEX = 6;  // index of affinities in our stats
   static const int STAT_AFF_END_INDEX   = 18; // last index of the last element's affinity

   UMyEnvQueryTest_DamageScore();

   virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

   virtual FText GetDescriptionTitle() const override;

   virtual FText GetDescriptionDetails() const override;

   int FindNumOffensiveAbilities(AUnit* unit) const;

   int GetHighestOffensiveAttribute(AUnit* unit) const;
};
