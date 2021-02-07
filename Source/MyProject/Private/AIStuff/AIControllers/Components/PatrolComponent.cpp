#include "MyProject.h"
#include "PatrolComponent.h"
#include "AIController.h"
#include "UnitController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "UpResourceManager.h"

FLinearColor UPatrolComponent::EDITOR_UNSELECTED_PATROL_COLOR = FLinearColor::Black;
FLinearColor UPatrolComponent::EDITOR_SELECTED_PATROL_COLOR   = FLinearColor::Blue;

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
      if(AUnitController* unitControllerRef = Cast<AUnitController>(ownerControllerRef)) {
         unitControllerRef->OnUnitStopped().AddUObject(this, &UPatrolComponent::StopPatrolling);
      }
      if(patrolOnStart) { Patrol(); }
   }
}

bool UPatrolComponent::Patrol()
{
   if(AAIController* ownerControllerRef = Cast<AAIController>(GetOwner())) {
      if(patrolPoints.Num() > 0) {
         ownerControllerRef->ReceiveMoveCompleted.AddDynamic(this, &UPatrolComponent::MoveToNextPatrolPoint);
         currentPatrolIndex = 0;
         const auto moveRes = ownerControllerRef->MoveToLocation(patrolPoints[currentPatrolIndex], 10, false);
         return MoveToNextPointAfterMoveRequestFail(moveRes);
      }
   }
   return false;
}

void UPatrolComponent::StopPatrolling()
{
   if(AAIController* ownerControllerRef = Cast<AAIController>(GetOwner())) {
      ownerControllerRef->ReceiveMoveCompleted.RemoveAll(this);
   }
}

void UPatrolComponent::DeletePatrolPoint(int patrolIndex)
{
   if(patrolIndex >= 0 && patrolIndex <= patrolPoints.Num()) { patrolPoints.RemoveAt(patrolIndex, 1); }
}

void UPatrolComponent::MoveToNextPatrolPoint(FAIRequestID requestId, EPathFollowingResult::Type moveRes)
{
   // Check results of the movement (or move request that instantly failed) we just finished
   if(moveRes != EPathFollowingResult::Type::Success) { UE_LOG(LogTemp, Error, TEXT("Failed to patrol to point %s with reason: %s"), *patrolPoints[currentPatrolIndex].ToString(), *UEnum::GetValueAsString(moveRes)); }

   if(AAIController* ownerControllerRef = Cast<AAIController>(GetOwner())) {
      currentPatrolIndex = (currentPatrolIndex + 1) % patrolPoints.Num();
      const auto newMoveRes = ownerControllerRef->MoveToLocation(patrolPoints[currentPatrolIndex], 10, false);
      MoveToNextPointAfterMoveRequestFail(newMoveRes);
   }
}

bool UPatrolComponent::MoveToNextPointAfterMoveRequestFail(EPathFollowingRequestResult::Type moveRes)
{
   if(moveRes == EPathFollowingRequestResult::Type::AlreadyAtGoal) {
      MoveToNextPatrolPoint(FAIRequestID(-1), EPathFollowingResult::Success);
      return true;
   }
         
   if(moveRes != EPathFollowingRequestResult::Type::RequestSuccessful) {
      MoveToNextPatrolPoint(FAIRequestID(-1), EPathFollowingResult::Invalid);
      return false;
   }

   return true;
}
