// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BTTask_AttTarget.h"
#include "SpellDataLibrary.h"
#include "TargetedAttackComponent.h"
#include "WorldObjects/Unit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIControllers/UnitController.h"
#include "UnitMessages.h"

UBTTask_AttTarget::UBTTask_AttTarget()
{
   NodeName    = "Attack Target";
   bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_AttTarget::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   AUnitController* AICon = Cast<AUnitController>(ownerComp.GetAIOwner());
   if(AICon) {
      AUnit* target = Cast<AUnit>(ownerComp.GetBlackboardComponent()->GetValueAsObject("target"));
      if(target && USpellDataLibrary::IsAttackable(target->GetAbilitySystemComponent())) {
         AICon->FindComponentByClass<UTargetedAttackComponent>()->BeginAttack(target);
         WaitForMessage(ownerComp, UnitMessages::AIMessage_TargetLoss);
         WaitForMessage(ownerComp, UnitMessages::AIMessage_Stunned);
         return EBTNodeResult::InProgress;
      }
   } else {
      ownerComp.GetBlackboardComponent()->SetValueAsObject("target", nullptr);
   }

   return EBTNodeResult::Failed;
}

void UBTTask_AttTarget::OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess)
{
   bSuccess = Message != UnitMessages::AIMessage_Stunned & Message != UnitMessages::AIMessage_TargetLoss;
   Super::OnMessage(OwnerComp, NodeMemory, Message, RequestID, bSuccess);
}
