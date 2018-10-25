// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BTTask_AttackMove.h"
#include "WorldObjects/Unit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIStuff/AIControllers/UnitController.h"

UBTTask_AttackMove::UBTTask_AttackMove()
{
   NodeName = "Attack While Moving";
}

EBTNodeResult::Type UBTTask_AttackMove::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   FBTAttackMoveMemory* myMemory         = (FBTAttackMoveMemory*)nodeMemory;
   myMemory->AICon                       = Cast<AUnitController>(ownerComp.GetAIOwner());
   EPathFollowingRequestResult::Type res = myMemory->AICon->GetUnitOwner()->Move(ownerComp.GetBlackboardComponent()->GetValueAsVector("moveToLocation"));
   if (res == EPathFollowingRequestResult::RequestSuccessful) {
      WaitForMessage(ownerComp, UBrainComponent::AIMessage_MoveFinished);
      WaitForMessage(ownerComp, UBrainComponent::AIMessage_RepathFailed);
      WaitForMessage(ownerComp, AUnit::AIMessage_AttackReady);
      // if ready to attack then just attack
      myMemory->AICon->GetUnitOwner()->SetTarget(Cast<AUnit>(ownerComp.GetBlackboardComponent()->GetValueAsObject("target")));
      myMemory->AICon->GetUnitOwner()->Attack();
      return EBTNodeResult::InProgress;
   } else if (res == EPathFollowingRequestResult::AlreadyAtGoal) {
      return EBTNodeResult::Succeeded;
      myMemory->AICon->GetUnitOwner()->BeginAttack(Cast<AUnit>(ownerComp.GetBlackboardComponent()->GetValueAsObject("target")));
   }

   return EBTNodeResult::Failed;
}

void UBTTask_AttackMove::OnMessage(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, FName message, int32 requestID, bool bSuccess)
{
   if (message == AUnit::AIMessage_AttackReady) {
      FBTAttackMoveMemory* myMemory = (FBTAttackMoveMemory*)nodeMemory;
      myMemory->AICon->GetUnitOwner()->Attack();
   } else {
      bSuccess &= (message != UBrainComponent::AIMessage_RepathFailed);
      Super::OnMessage(ownerComp, nodeMemory, message, requestID, bSuccess);
   }
}

uint16 UBTTask_AttackMove::GetInstanceMemorySize() const
{
   return sizeof(FBTAttackMoveMemory);
}