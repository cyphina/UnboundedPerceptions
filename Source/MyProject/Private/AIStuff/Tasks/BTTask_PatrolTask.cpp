// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BTTask_PatrolTask.h"
#include "../AIControllers/UnitController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "WorldObjects/Unit.h"

UBTTask_Patrol::UBTTask_Patrol(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
   NodeName    = "Patrol";
   bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   FBTPatrolTaskMemory* myMemory = (FBTPatrolTaskMemory*)nodeMemory;
   myMemory->patrolIndex         = -1;
   myMemory->AICon               = Cast<AUnitController>(ownerComp.GetAIOwner());

   if (myMemory->AICon && myMemory->AICon->patrolLocations.Num() > 0) {
      UBlackboardComponent* blackboardComp = ownerComp.GetBlackboardComponent();
      if (blackboardComp) {
         if (myMemory->AICon->patrolLocations.Num() > 0) { return PatrolToNextLocation(ownerComp, nodeMemory); }
      }
   }
   return EBTNodeResult::Failed;
}

void UBTTask_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* nodeMemory, float DeltaSeconds)
{
   FBTPatrolTaskMemory* myMemory = (FBTPatrolTaskMemory*)nodeMemory;
   // if we stop patrol to the next location
   // in a simple parallel, if we detect any units from our eqs query, then stop patrolling and attack
   if (myMemory->AICon->GetUnitOwner() && myMemory->AICon->GetBlackboardComponent()->GetValueAsObject(targetKeyName)) {
      myMemory->AICon->StopMovement();
      FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
   }
}

EBTNodeResult::Type UBTTask_Patrol::PatrolToNextLocation(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   FBTPatrolTaskMemory* myMemory = (FBTPatrolTaskMemory*)nodeMemory;

   ++myMemory->patrolIndex;

   if (myMemory->patrolIndex > myMemory->AICon->patrolLocations.Num() - 1) myMemory->patrolIndex = 0;

   myMemory->AICon->GetBlackboardComponent()->SetValueAsVector(locKeyName, myMemory->AICon->patrolLocations[myMemory->patrolIndex]);
   EPathFollowingRequestResult::Type result = myMemory->AICon->GetUnitOwner()->Move(myMemory->AICon->patrolLocations[myMemory->patrolIndex]);

   if (result == EPathFollowingRequestResult::RequestSuccessful) {
      WaitForMessage(ownerComp, UBrainComponent::AIMessage_MoveFinished);
      WaitForMessage(ownerComp, UBrainComponent::AIMessage_RepathFailed);
      return EBTNodeResult::InProgress;
   } else if (result == EPathFollowingRequestResult::AlreadyAtGoal) {
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
   PatrolToNextLocation(OwnerComp, NodeMemory);
}
