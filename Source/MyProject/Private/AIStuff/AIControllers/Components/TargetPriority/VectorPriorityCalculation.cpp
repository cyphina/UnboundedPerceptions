#include "VectorPriorityCalculation.h"

TArray<UEnvQueryTest*> UVectorPriorityCalculation::GetQueryTestsFromDescriptors
(const FGameplayTagContainer& spellDescriptiveTags, const FSpellTargetCriteria& targetCriteria)
{
   return TArray<UEnvQueryTest*>();
}

UEnvQueryGenerator* UVectorPriorityCalculation::GetGeneratorFromManualTag(FGameplayTag manualTag)
{
   return nullptr;
}

FGameplayAbilityTargetDataHandle UVectorPriorityCalculation::GetBestTargetFromDistribution(TSharedPtr<FEnvQueryResult> queryResult)
{
   return nullptr;
}
