#include "PriorityComponent.h"

#include "GameplayTagContainer.h"

#include "UnitQueryGenerator.h"
#include "EnvQueryTest_LowHPTarget.h"
#include "PriorityCalculation.h"
#include "TargetComponent.h"
#include "Unit.h"

#include "UnitController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BTTaskNode.h"

#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryOption.h"

void UUpPriorityComponent::FindBestTargetForSpell(TSubclassOf<UMySpell> spell)
{
   FEnvQueryRequest queryRequest;
   queryRequest.SetFloatParam("SimpleGrid.GridSize", 700);
   queryRequest.Execute(EEnvQueryRunMode::RandomBest25Pct, this, &UUpPriorityComponent::OnTargetFound);
}

void UUpPriorityComponent::BeginPlay()
{
   unitControllerRef = Cast<AUnitController>(GetOwner());
}

void UUpPriorityComponent::FindBestAOELocation(bool isSupport)
{
   if(isSupport)
      queryRequest = FEnvQueryRequest(findBestAOESupportLocation, ownerRef);
   else
      queryRequest = FEnvQueryRequest(findBestAOEAssaultLocation, ownerRef);

   // queryRequest.SetFloatParam("CircleRadius", ownerRef->abilities[spellIndex].GetDefaultObject()->GetAOE(ownerRef->GetAbilitySystemComponent()));
}

void UUpPriorityComponent::OnTargetFound(TSharedPtr<FEnvQueryResult> result)
{
   if(result->IsSuccsessful()) {
      UTargetComponent* targetComp = unitControllerRef->GetUnitOwner()->FindComponentByClass<UTargetComponent>();
      targetComp->SetSpellTarget(priorityCalculation->GetBestTargetFromDistribution(result));

   } else {
      StopBehaviorTreeTargetTask();
   }
}

UBehaviorTreeComponent* UUpPriorityComponent::GetBehaviorTreeComp() const
{
   return unitControllerRef->behaviorTreeComp;
}

void UUpPriorityComponent::StopBehaviorTreeTargetTask() const
{
   Cast<UBTTaskNode>(GetBehaviorTreeComp()->GetActiveNode())->FinishLatentTask(*GetBehaviorTreeComp(), EBTNodeResult::Failed);
}

void UUpPriorityComponent::GetCurrentlySelectedSpell() const
{
}

void UUpPriorityComponent::CastCurrentlySelectedSpell() const
{
   unitControllerRef->FindComponentByClass<USpellCastComponent>()->BeginCastSpell(unitControllerRef->GetUnitOwner()->abilities[spellToCastIndex]);
}
