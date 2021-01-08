#include "ActorPriorityCalculation.h"

TArray<UEnvQueryTest*> UActorPriorityCalculation::GetQueryTestsFromDescriptors
(const FGameplayTagContainer& spellDescriptiveTags,
 const FSpellTargetCriteria&  targetCriteria)
{
   return TArray<UEnvQueryTest*>();
}

UEnvQueryGenerator* UActorPriorityCalculation::GetGeneratorFromManualTag(FGameplayTag manualTag)
{
   return nullptr;
}

FGameplayAbilityTargetDataHandle UActorPriorityCalculation::GetBestTargetFromDistribution(TSharedPtr<FEnvQueryResult> queryResult)
{
   return nullptr;
}
