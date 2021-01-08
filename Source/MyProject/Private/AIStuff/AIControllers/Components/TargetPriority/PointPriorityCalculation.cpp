#include "PointPriorityCalculation.h"

TArray<UEnvQueryTest*> UPointPriorityCalculation::GetQueryTestsFromDescriptors
(const FGameplayTagContainer& spellDescriptiveTags, const FSpellTargetCriteria& targetCriteria)
{
   return TArray<UEnvQueryTest*>();
}

UEnvQueryGenerator* UPointPriorityCalculation::GetGeneratorFromManualTag(FGameplayTag manualTag)
{
   return nullptr;
}

FGameplayAbilityTargetDataHandle UPointPriorityCalculation::GetBestTargetFromDistribution(TSharedPtr<FEnvQueryResult> queryResult)
{
   return nullptr;
}
