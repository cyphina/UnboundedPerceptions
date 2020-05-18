// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BTTask_PatrolTask.h"
#include "../AIControllers/UnitController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "WorldObjects/Components/PatrolComponent.h"
#include "WorldObjects/Unit.h"

UBTTask_Patrol::UBTTask_Patrol(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
   NodeName    = "Patrol";
   bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   FBTPatrolTaskMemory* myMemory = (FBTPatrolTaskMemory*)nodeMemory;
   myMemory->AICon               = Cast<AAIController>(ownerComp.GetAIOwner());
   myMemory->patrolComp          = ownerComp.GetAIOwner()->GetPawn()->FindComponentByClass<UPatrolComponent>();

   if(myMemory->AICon && myMemory->patrolComp->patrolPoints.Num() > 0) {
      UBlackboardComponent* blackboardComp = ownerComp.GetBlackboardComponent();
      if(blackboardComp) {
         if(myMemory->patrolComp->patrolPoints.Num() > 0) {
            return PatrolToNextLocation(ownerComp, nodeMemory);
         }
      }
   }
   return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_Patrol::PatrolToNextLocation(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   FBTPatrolTaskMemory* myMemory = (FBTPatrolTaskMemory*)nodeMemory;

   EPathFollowingRequestResult::Type result = myMemory->patrolComp->MoveToNextPatrolPoint();
   myMemory->AICon->GetBlackboardComponent()->SetValueAsVector(locKeyName, myMemory->patrolComp->GetCurrentPatrolPoint());

   if(result == EPathFollowingRequestResult::RequestSuccessful) {
      // Without a request ID, the new move request creation will somehow trigger the message instantly, infinitely looping OnMessage and PatrolToNextLocation
      auto requestId = myMemory->AICon->GetCurrentMoveRequestID();
      WaitForMessage(ownerComp, UBrainComponent::AIMessage_MoveFinished, requestId.GetID());
      WaitForMessage(ownerComp, UBrainComponent::AIMessage_RepathFailed, requestId.GetID());
      WaitForMessage(ownerComp, AUnit::AIMessage_FoundTarget);
      return EBTNodeResult::InProgress;
   } else if(result == EPathFollowingRequestResult::AlreadyAtGoal) {
      return EBTNodeResult::Succeeded;
   }
   return EBTNodeResult::Failed;
}

uint16 UBTTask_Patrol::GetInstanceMemorySize() const
{
   return sizeof(FBTPatrolTaskMemory);
}

void UBTTask_Patrol::OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess)
{
   bSuccess = Message != UBrainComponent::AIMessage_RepathFailed;

   if(Message != UBrainComponent::AIMessage_MoveFinished) {
      // If we do not move to the next point as intended
      if(!bSuccess)
         UE_LOG(LogBehaviorTree, Warning, TEXT("Problem patrolling to location"))
      else {
         // Found a target so move on from this task
         Super::OnMessage(OwnerComp, NodeMemory, Message, RequestID, bSuccess);
         return;
      }
   }

   // Didn't find a target so keep patrolling
   Super::OnMessage(OwnerComp, NodeMemory, Message, RequestID, bSuccess);
   PatrolToNextLocation(OwnerComp, NodeMemory);
}
