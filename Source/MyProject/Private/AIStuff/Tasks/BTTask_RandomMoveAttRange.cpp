// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BTTask_RandomMoveAttRange.h"
#include "Unit.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem/Public/NavigationSystem.h"

EBTNodeResult::Type UBTTask_RandomMoveAttRange::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   AUnit* target = Cast<AUnit>(ownerComp.GetBlackboardComponent()->GetValueAsObject("target"));
   AUnit* owner  = Cast<AUnit>(ownerComp.GetAIOwner()->GetPawn());

   if (target && owner) {
      FVector targetLocation = target->GetActorLocation();

      float   attackRange  = owner->GetMechanicAdjValue(EMechanics::AttackRange);
      FVector pointInRange = FVector(FMath::RandPointInCircle(attackRange), 0);

      UNavigationSystemV1* navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
      FPathFindingQuery    q(ownerComp.GetOwner(), *navSys->GetDefaultNavDataInstance(), ownerComp.GetOwner()->GetActorLocation(), targetLocation + pointInRange);

      // ensure this path is actually closeby since there could be obstacles making the path long even if the point is close
      if (navSys->FindPathSync(q).Path->GetLength() < attackRange * 2) {
         ownerComp.GetBlackboardComponent()->SetValueAsVector("targetLocation", targetLocation + pointInRange);
         return EBTNodeResult::Succeeded;
      }
   }
   return EBTNodeResult::Failed;
}
