// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BTTask_AttackWhileMove.h"
#include "WorldObjects/Unit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UnitMessages.h"
#include "AIStuff/AIControllers/UnitController.h"

UBTTask_AttackWhileMove::UBTTask_AttackWhileMove()
{
   NodeName    = "Attack While Moving";
   bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_AttackWhileMove::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   FBTAttackMoveMemory* myMemory = (FBTAttackMoveMemory*)nodeMemory;
   myMemory->AICon               = Cast<AUnitController>(ownerComp.GetAIOwner());

   EPathFollowingRequestResult::Type res = myMemory->AICon->Move(ownerComp.GetBlackboardComponent()->GetValueAsVector("moveToLocation"));
   if(res == EPathFollowingRequestResult::RequestSuccessful) { //if we sent a successful move request

      WaitForMessage(ownerComp, UBrainComponent::AIMessage_MoveFinished);
      WaitForMessage(ownerComp, UBrainComponent::AIMessage_RepathFailed);
      WaitForMessage(ownerComp, UnitMessages::AIMessage_Hit);

      // We set a function to be called periodically
      TFunction<void(void)> attackFunc = [myMemory]() {
         myMemory->AICon->Attack();
         myMemory->AICon->PauseMove(myMemory->AICon->GetCurrentMoveRequestID());
      };

      FMath::FRandRange(0, delayVariance);
      myMemory->AICon->GetWorld()->GetTimerManager().SetTimer(myMemory->attackTime, MoveTemp(attackFunc), attackDelay + delayVariance, false, 0.f);

      return EBTNodeResult::InProgress;

      // If we requested to move somewhere but we are already at there
   } else if(res == EPathFollowingRequestResult::AlreadyAtGoal) {
      myMemory->AICon->Attack();
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
   // This attack message means that we should move again because we just initiated an attack
   if(message == UnitMessages::AIMessage_Hit) {
      FBTAttackMoveMemory* myMemory = (FBTAttackMoveMemory*)nodeMemory;
      myMemory->AICon->ResumeMove(myMemory->AICon->GetCurrentMoveRequestID());
   }
   //Finish the task when we get a message that our move finished
   else { 
      bSuccess &= (message != UBrainComponent::AIMessage_RepathFailed);
      FBTAttackMoveMemory* myMemory = (FBTAttackMoveMemory*)nodeMemory;
      myMemory->attackTime.Invalidate();
      Super::OnMessage(ownerComp, nodeMemory, message, requestID, bSuccess);
   }
}
