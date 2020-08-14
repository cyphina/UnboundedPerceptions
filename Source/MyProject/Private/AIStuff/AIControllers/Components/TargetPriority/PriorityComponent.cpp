#include "PriorityComponent.h"

#include "GameplayTagContainer.h"

#include "UnitQueryGenerator.h"
#include "EnvQueryTest_LowHPTarget.h"

#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryOption.h"

void UUpPriorityComponent::FindBestAOELocation(bool isSupport)
{
   FEnvQueryRequest queryRequest;

   if(isSupport)
      queryRequest = FEnvQueryRequest(findBestAOESupportLocation, ownerRef);
   else
      queryRequest = FEnvQueryRequest(findBestAOEAssaultLocation, ownerRef);

   // queryRequest.SetFloatParam("CircleRadius", ownerRef->abilities[spellIndex].GetDefaultObject()->GetAOE(ownerRef->GetAbilitySystemComponent()));
   queryRequest.SetFloatParam("SimpleGrid.GridSize", 700);
   queryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &AUnitController::OnAOELocationFound);
}

void UUpPriorityComponent::FindWeakestTarget(bool isSupport)
{
   FEnvQueryRequest queryRequest;
   if(isSupport)
      queryRequest = FEnvQueryRequest(findWeakestAllyTarget, ownerRef);
   else
      queryRequest = FEnvQueryRequest(findWeakestEnemyTarget, ownerRef);

   queryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &AUnitController::OnWeakestTargetFound);
}

void UUpPriorityComponent::CreateTargetDistribution()
{
   UEnvQuery::
}

void UUpPriorityComponent::FindBestSpellTarget(FGameplayTag targettingTag, bool isSupport)
{
   if(targettingTag == FGameplayTag::RequestGameplayTag("Skill.Targetting.Area")) {
      FindBestAOELocation(isSupport);
   } else if(targettingTag == FGameplayTag::RequestGameplayTag("Skill.Targetting.None")) {
      BeginCastSpell(ownerRef->abilities[spellToCastIndex]);
   } else if(targettingTag.MatchesTag(FGameplayTag::RequestGameplayTag("Skill.Targetting.Single"))) {
      FindWeakestTarget(isSupport);
   }
}

void UUpPriorityComponent::OnAOELocationFound(TSharedPtr<FEnvQueryResult> result)
{
   if(result->IsSuccsessful()) {
      ownerRef->SetTargetLocation(result->GetItemAsLocation(0));
      BeginCastSpell(ownerRef->abilities[spellToCastIndex]);
   } else
      // Aborting means it needs some cleanup while fail just ends the node
      Cast<UBTTaskNode>(behaviorTreeComp->GetActiveNode())->FinishLatentTask(*behaviorTreeComp, EBTNodeResult::Failed);
}

void UUpPriorityComponent::OnWeakestTargetFound(TSharedPtr<FEnvQueryResult> result)
{
   GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("FOUND WEAKEST TARGET!"));
   if(result.IsValid())
   // Need to actually check here since our filter could end up with us having no heroes to target like if all heroes are full hp
   {
      ownerRef->SetTargetUnit(Cast<AUnit>(result->GetItemAsActor(0)));
      BeginCastSpell(ownerRef->abilities[spellToCastIndex]);
   } else
      Cast<UBTTaskNode>(behaviorTreeComp->GetActiveNode())->FinishLatentTask(*behaviorTreeComp, EBTNodeResult::Failed);
}
