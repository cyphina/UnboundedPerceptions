#include "PriorityComponent.h"

#include "ActorPriorityCalculation.h"
#include "GameplayTagContainer.h"

#include "UnitQueryGenerator.h"
#include "EnvQueryTest_LowHPTarget.h"
#include "MySpell.h"
#include "PointPriorityCalculation.h"
#include "PriorityCalculation.h"
#include "PriorityStructs.h"
#include "TargetComponent.h"
#include "Unit.h"

#include "UnitController.h"
#include "VectorPriorityCalculation.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BTTaskNode.h"

#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryOption.h"

void UUpPriorityComponent::FindBestTargetForSpell(TSubclassOf<UMySpell> spell)
{
   FEnvQueryRequest queryRequest;
   priorityCalculation = MakePriorityCalculation(GetManualTag(spell));
   queryRequest.SetFloatParam("SimpleGrid.GridSize", 700);
   queryRequest.Execute(EEnvQueryRunMode::RandomBest25Pct, this, &UUpPriorityComponent::OnTargetFound);
}

void UUpPriorityComponent::BeginPlay()
{
   unitControllerRef = Cast<AUnitController>(GetOwner());
}

UPriorityCalculation* UUpPriorityComponent::MakePriorityCalculation(FGameplayTag targetingTag) const
{
   if(targetingTag.MatchesTag(FGameplayTag::RequestGameplayTag("Skill.Targetting.Single"))) {
      return NewObject<UPriorityCalculation>();
   } else if(targetingTag.MatchesTag(FGameplayTag::RequestGameplayTag("Skill.Targetting.Area"))) {
      return NewObject<UPriorityCalculation>();

   } else if(targetingTag.MatchesTag(FGameplayTag::RequestGameplayTag("Skill.Targetting.Vector"))) {
      return NewObject<UPriorityCalculation>();
   } else {
      checkf(false, TEXT("Incorrect targetting tag passed to priority calculation factory"));
   }
   return nullptr;
}

void UUpPriorityComponent::OnTargetFound(TSharedPtr<FEnvQueryResult> envQuery)
{
   if(envQuery->IsSuccsessful()) {
      GetTargetComp()->SetTarget(priorityCalculation->GetBestTargetFromDistribution(envQuery));
   } else {
      StopBehaviorTreeTargetTask();
   }
}

UTargetComponent* UUpPriorityComponent::GetTargetComp() const
{
   return unitControllerRef->GetUnitOwner()->FindComponentByClass<UTargetComponent>();
}

FGameplayTag UUpPriorityComponent::GetManualTag(TSubclassOf<UMySpell> spell) const
{
   return spell.GetDefaultObject()->GetTargeting()->GetTargetTag();
}

FGameplayTagContainer UUpPriorityComponent::GetDescriptorTags(TSubclassOf<UMySpell> spell) const
{
   return spell.GetDefaultObject()->AbilityTags.Filter(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Skill.Category")));
}

UBehaviorTreeComponent* UUpPriorityComponent::GetBehaviorTreeComp() const
{
   return unitControllerRef->FindComponentByClass<UBehaviorTreeComponent>();
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
   /*unitControllerRef->FindComponentByClass<USpellCastComponent>()->BeginCastSpell(
       unitControllerRef->GetUnitOwner()->GetAbilitySystemComponent()->GetAbilities()[spellToCastIndex]);*/
}
