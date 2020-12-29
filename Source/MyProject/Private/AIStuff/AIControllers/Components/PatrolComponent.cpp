#include "MyProject.h"
#include "PatrolComponent.h"
#include "AIController.h"
#include "UnitController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "UpResourceManager.h"

UPatrolComponent::UPatrolComponent()
{
   PrimaryComponentTick.bCanEverTick = false;
   const ConstructorHelpers::FObjectFinder<UBehaviorTree> patrolTreeRef(TEXT("/Game/RTS_Tutorial/Blueprints/AI/Patrol/PatrolBT"));
   if(patrolTreeRef.Succeeded()) patrolTree = patrolTreeRef.Object;
}

void UPatrolComponent::BeginPlay()
{
   Super::BeginPlay();
   if(AAIController* ownerControllerRef = Cast<AAIController>(GetOwner())) {
      ownerBTComp = ownerControllerRef->FindComponentByClass<UBehaviorTreeComponent>();
      ownerControllerRef->ReceiveMoveCompleted.AddDynamic(this, &UPatrolComponent::MoveToNextPatrolPoint);
      ensure(ownerBTComp);
      if(AUnitController* unitControllerRef = Cast<AUnitController>(ownerControllerRef)) {
         unitControllerRef->OnUnitStopped().AddUObject(this, &UPatrolComponent::StopPatrolling);
      }
      if(patrolOnStart) { Patrol(); }
   }
}

void UPatrolComponent::Patrol()
{
   if(AAIController* ownerControllerRef = Cast<AAIController>(GetOwner())) {
      if(patrolPoints.Num() > 0) {
         ownerControllerRef->ReceiveMoveCompleted.AddDynamic(this, &UPatrolComponent::MoveToNextPatrolPoint);
         currentPatrolIndex = 0;
         const auto moveRes = ownerControllerRef->MoveToLocation(patrolPoints[currentPatrolIndex], 10, false);
         if(moveRes != EPathFollowingResult::Type::Success) {
            UE_LOG(LogTemp, Error, TEXT("Failed to patrol to point with reason: %s"), UEnum::GetValueAsString(moveRes));
         }
      }
   }
}

void UPatrolComponent::StopPatrolling()
{
   if(AAIController* ownerControllerRef = Cast<AAIController>(GetOwner())) {
      ownerControllerRef->ReceiveMoveCompleted.RemoveAll(this);
      ownerControllerRef->StopMovement();
   }
}

void UPatrolComponent::DeletePatrolPoint(int patrolIndex)
{
   if(patrolIndex >= 0 && patrolIndex <= patrolPoints.Num()) { patrolPoints.RemoveAt(patrolIndex, 1); }
}

void UPatrolComponent::MoveToNextPatrolPoint(FAIRequestID requestId, EPathFollowingResult::Type moveRes)
{
   if(moveRes != EPathFollowingResult::Type::Success) { UE_LOG(LogTemp, Error, TEXT("Failed to patrol to point with reason: %s"), UEnum::GetValueAsString(moveRes)); }

   if(AAIController* ownerControllerRef = Cast<AAIController>(GetOwner())) {
      currentPatrolIndex = (currentPatrolIndex + 1) % patrolPoints.Num();
      ownerControllerRef->MoveToLocation(patrolPoints[currentPatrolIndex], 10, false);
   }
}
