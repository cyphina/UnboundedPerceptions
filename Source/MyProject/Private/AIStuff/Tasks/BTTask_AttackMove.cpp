// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BTTask_AttackMove.h"
#include "WorldObjects/Unit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIStuff/AIControllers/UnitController.h"

UBTTask_AttackMove::UBTTask_AttackMove()
{
   NodeName = "Attack While Moving";
   bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_AttackMove::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   FBTAttackMoveMemory* myMemory         = (FBTAttackMoveMemory*)nodeMemory;
   myMemory->AICon                       = Cast<AUnitController>(ownerComp.GetAIOwner());

   EPathFollowingRequestResult::Type res = myMemory->AICon->Move(ownerComp.GetBlackboardComponent()->GetValueAsVector("moveToLocation"));
   if (res == EPathFollowingRequestResult::RequestSuccessful) { //if we sent a successful move request
      //listen for these kinds of messages
      WaitForMessage(ownerComp, UBrainComponent::AIMessage_MoveFinished); 
      WaitForMessage(ownerComp, UBrainComponent::AIMessage_RepathFailed);
      WaitForMessage(ownerComp, AUnit::AIMessage_AttackReady);

      // if ready to attack already, then just attack
      TFunction<void(void)> attackFunc = [myMemory]()
      {
         myMemory->AICon->BeginAttack(Cast<AUnit>(myMemory->AICon->blackboardComp->GetValueAsObject("target")));
         myMemory->AICon->PauseMove(myMemory->AICon->GetCurrentMoveRequestID());
      };
      myMemory->AICon->GetWorld()->GetTimerManager().SetTimer(myMemory->attackTime, MoveTemp(attackFunc), attackDelay, false, 0.f);

      return EBTNodeResult::InProgress;

   } else if (res == EPathFollowingRequestResult::AlreadyAtGoal) { //if we requested to move somewhere but we are already at there
      myMemory->AICon->BeginAttack(Cast<AUnit>(ownerComp.GetBlackboardComponent()->GetValueAsObject("target")));
      return EBTNodeResult::Succeeded;
   }
   //if something else happens, that's not what we intend so just fail the ai task
   return EBTNodeResult::Failed;
}

uint16 UBTTask_AttackMove::GetInstanceMemorySize() const
{
   return sizeof(FBTAttackMoveMemory);
}

void UBTTask_AttackMove::OnMessage(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, FName message, int32 requestID, bool bSuccess)
{
   //This attack message means that we should move again because we just initiated an attack
   if (message == AUnit::AIMessage_AttackReady) {
      FBTAttackMoveMemory* myMemory = (FBTAttackMoveMemory*)nodeMemory;
      myMemory->AICon->ResumeMove(myMemory->AICon->GetCurrentMoveRequestID());
   } 
   else { //finishes task when we get a message that our move finished
      bSuccess &= (message != UBrainComponent::AIMessage_RepathFailed);
      Super::OnMessage(ownerComp, nodeMemory, message, requestID, bSuccess);
   }
}

