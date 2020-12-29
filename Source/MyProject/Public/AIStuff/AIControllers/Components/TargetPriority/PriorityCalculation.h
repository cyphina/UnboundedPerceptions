// Created 8/13/20 11:47 AM

#pragma once
#include "GameplayAbilityTargetTypes.h"
#include "GameplayTagContainer.h"
#include "PriorityCalculation.generated.h"

class UEnvQueryGenerator;
class UEnvQueryTest;
class UEnvQueryItemType;
struct FEnvQueryResult;
struct FSpellTargetCriteria;

UINTERFACE(MinimalAPI)
class UPriorityCalculation : public UInterface
{
   GENERATED_UINTERFACE_BODY()
};

class IPriorityCalculation
{
   GENERATED_IINTERFACE_BODY()

 public:

  UFUNCTION()
  virtual TArray<UEnvQueryTest*> GetQueryTestsFromDescriptors(const FGameplayTagContainer& spellDescriptiveTags, const FSpellTargetCriteria& targetCriteria) = 0;

  UFUNCTION()
  virtual UEnvQueryGenerator* GetGeneratorFromManualTag(FGameplayTag manualTag) = 0;

  UFUNCTION()
  virtual FGameplayAbilityTargetDataHandle GetBestTargetFromDistribution(TSharedPtr<FEnvQueryResult> queryResult) = 0;

};

