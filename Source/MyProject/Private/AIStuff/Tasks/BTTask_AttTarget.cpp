// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BTTask_AttTarget.h"
#include "WorldObjects/Unit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIControllers/UnitController.h"
#include "State/AttackState.h"

UBTTask_AttTarget::UBTTask_AttTarget()
{
   NodeName = "Attack Target";
   bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_AttTarget::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   AUnitController* AICon = Cast<AUnitController>(ownerComp.GetAIOwner());
   if (AICon) {
      AUnit* target = Cast<AUnit>(ownerComp.GetBlackboardComponent()->GetValueAsObject("target"));
      if (target && target->GetCanTarget()) {
         // AICon->GetUnitOwner()->SetTarget(target);
         // if we aren't already attacking this target
         AICon->BeginAttack(target);
         WaitForMessage(ownerComp, AUnit::AIMessage_AttackReady);
         WaitForMessage(ownerComp, AUnit::AIMessage_Stunned);
         return EBTNodeResult::InProgress;
      }
   }
   else {
      ownerComp.GetBlackboardComponent()->SetValueAsObject("target", nullptr);
   }

   return EBTNodeResult::Failed;
}

void UBTTask_AttTarget::OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess)
{
   bSuccess = Message != AUnit::AIMessage_Stunned;
   Super::OnMessage(OwnerComp, NodeMemory, Message, RequestID, bSuccess);
}
