// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BTTask_AttackWhileMove.h"
#include "WorldObjects/Unit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIStuff/AIControllers/UnitController.h"

UBTTask_AttackWhileMove::UBTTask_AttackWhileMove()
{
   NodeName = "Attack While Moving";
   bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_AttackWhileMove::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   FBTAttackMoveMemory* myMemory         = (FBTAttackMoveMemory*)nodeMemory;
   myMemory->AICon                       = Cast<AUnitController>(ownerComp.GetAIOwner());

   EPathFollowingRequestResult::Type res = myMemory->AICon->Move(ownerComp.GetBlackboardComponent()->GetValueAsVector("moveToLocation"));
   if (res == EPathFollowingRequestResult::RequestSuccessful) { //if we sent a successful move request
      //
      //Listen for these kinds of messages to figure out if we're finished with this task
      WaitForMessage(ownerComp, UBrainComponent::AIMessage_MoveFinished); 
      WaitForMessage(ownerComp, UBrainComponent::AIMessage_RepathFailed);
      WaitForMessage(ownerComp, AUnit::AIMessage_AttackReady);

      // We set a function to be called periodically 
      TFunction<void(void)> attackFunc = [myMemory]()
      {
         myMemory->AICon->BeginAttack(Cast<AUnit>(myMemory->AICon->blackboardComp->GetValueAsObject("target")));
         myMemory->AICon->PauseMove(myMemory->AICon->GetCurrentMoveRequestID());
      };
      myMemory->AICon->GetWorld()->GetTimerManager().SetTimer(myMemory->attackTime, MoveTemp(attackFunc), attackDelay, false, 0.f);

      return EBTNodeResult::InProgress;

      // If we requested to move somewhere but we are already at there
   } else if (res == EPathFollowingRequestResult::AlreadyAtGoal) { 
      myMemory->AICon->BeginAttack(Cast<AUnit>(ownerComp.GetBlackboardComponent()->GetValueAsObject("target")));
      return EBTNodeResult::Succeeded;
   }
   // If something else happens, that's not what we intend so just fail the ai task
   return EBTNodeResult::Failed;
}

uint16 UBTTask_AttackWhileMove::GetInstanceMemorySize() const
{
   return sizeof(FBTAttackMoveMemory);
}

void UBTTask_AttackWhileMove::OnMessage(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, FName message, int32 requestID, bool bSuccess)
{
   //This attack message means that we should move again because we just initiated an attack
   if (message == AUnit::AIMessage_AttackReady) {
      FBTAttackMoveMemory* myMemory = (FBTAttackMoveMemory*)nodeMemory;
      myMemory->AICon->ResumeMove(myMemory->AICon->GetCurrentMoveRequestID());
   } 
   else { //finishes task when we get a message that our move finished
      bSuccess &= (message != UBrainComponent::AIMessage_RepathFailed);
      FBTAttackMoveMemory* myMemory         = (FBTAttackMoveMemory*)nodeMemory;
      myMemory->attackTime.Invalidate();
      Super::OnMessage(ownerComp, nodeMemory, message, requestID, bSuccess);
   }
}

