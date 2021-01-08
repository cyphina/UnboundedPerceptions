// Created 8/12/20 5:38 AM

#pragma once
#include "PriorityCalculation.h"
#include "ActorPriorityCalculation.generated.h"

UCLASS()
class MYPROJECT_API UActorPriorityCalculation : public UObject, public IPriorityCalculation
{
   GENERATED_BODY()
   
 public:
   TArray<UEnvQueryTest*>           GetQueryTestsFromDescriptors(const FGameplayTagContainer& spellDescriptiveTags, const FSpellTargetCriteria& targetCriteria) override;
   UEnvQueryGenerator*              GetGeneratorFromManualTag(FGameplayTag manualTag) override;
   FGameplayAbilityTargetDataHandle GetBestTargetFromDistribution(TSharedPtr<FEnvQueryResult> queryResult) override;
};
