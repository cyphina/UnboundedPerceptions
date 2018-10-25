// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "GameplayTagContainer.h"
#include "EnvQueryTest_NumBuffs.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API UEnvQueryTest_NumBuffs : public UEnvQueryTest
{
   GENERATED_BODY()

   UEnvQueryTest_NumBuffs();

   virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

   virtual FText GetDescriptionTitle() const override;

   virtual FText GetDescriptionDetails() const override;

   static const FGameplayTag debuffTags;
};
