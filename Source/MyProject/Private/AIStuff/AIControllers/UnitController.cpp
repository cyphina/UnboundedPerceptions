// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "WorldObjects/Unit.h"
#include "UnitController.h"
#include "UserInput.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "BehaviorTree/BTTaskNode.h"

#include "AbilitySystemBlueprintLibrary.h"

#include "SpellSystem/MySpell.h"
#include "EnvironmentQuery/Generators/EnvQueryGenerator_OnCircle.h"
#include "BehaviorTree/Tasks/BTTask_RunEQSQuery.h"

AUnitController::AUnitController()
{
   // Component Init
   behaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
   blackboardComp   = CreateDefaultSubobject<UBlackboardComponent>(FName("BlackboardComp"));
}

void AUnitController::Possess(APawn* InPawn)
{
   Super::Possess(InPawn);
   ownerRef = Cast<AUnit>(GetPawn());
}

void AUnitController::BeginPlay()
{
   Super::BeginPlay();
   CPCRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
}

void AUnitController::FindBestAOELocation(int spellIndex, bool isSupport)
{
   FEnvQueryRequest queryRequest;
   if (isSupport)
      queryRequest = FEnvQueryRequest(findBestAOESupportLocation, GetUnitOwner());
   else
      queryRequest = FEnvQueryRequest(findBestAOEAssaultLocation, GetUnitOwner());

   // queryRequest.SetFloatParam("CircleRadius", GetUnitOwner()->abilities[spellIndex].GetDefaultObject()->GetAOE(GetUnitOwner()->GetAbilitySystemComponent()));
   spellToCastIndex = spellIndex;
   queryRequest.SetFloatParam("SimpleGrid.GridSize", 700);
   queryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &AUnitController::OnAOELocationFound);
}

void AUnitController::FindWeakestTarget(int spellIndex, bool isSupport)
{
   FEnvQueryRequest queryRequest;
   if (isSupport)
      queryRequest = FEnvQueryRequest(findWeakestAllyTarget, GetUnitOwner());
   else
      queryRequest = FEnvQueryRequest(findWeakestEnemyTarget, GetUnitOwner());

   spellToCastIndex = spellIndex;
   queryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &AUnitController::OnWeakestTargetFound);
}

void AUnitController::OnAOELocationFound(TSharedPtr<FEnvQueryResult> result)
{
   if (result->IsSuccsessful()) {
      FGameplayAbilityTargetingLocationInfo tInfo;
      tInfo.LocationType     = EGameplayAbilityTargetingLocationType::LiteralTransform;
      tInfo.LiteralTransform = FTransform(result->GetItemAsLocation(0));
      FGameplayAbilityTargetDataHandle targetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromLocations(tInfo, tInfo); // start point isn't really used so we can put this twice
      GetUnitOwner()->BeginCastSpell(spellToCastIndex, targetData);
   } else
      // Aborting means it needs some cleanup while fail just ends the node
      Cast<UBTTaskNode>(behaviorTreeComp->GetActiveNode())->FinishLatentTask(*behaviorTreeComp, EBTNodeResult::Failed);
}

void AUnitController::OnWeakestTargetFound(TSharedPtr<FEnvQueryResult> result)
{
   GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("FOUND WEAKEST TARGET!"));
   if (result.IsValid()) // need to actually check here since our filter could end up with us having no heroes to target like if all heroes are full
   {
      FGameplayAbilityTargetDataHandle targetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(result->GetItemAsActor(0));
      GetUnitOwner()->BeginCastSpell(spellToCastIndex, targetData);
   } else
      Cast<UBTTaskNode>(behaviorTreeComp->GetActiveNode())->FinishLatentTask(*behaviorTreeComp, EBTNodeResult::Failed);
}
