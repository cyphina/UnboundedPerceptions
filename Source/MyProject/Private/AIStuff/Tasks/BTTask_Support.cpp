// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Tasks/BTTask_Support.h"

#include "WorldObjects/Unit.h"
#include "UnitController.h"

#include "ResourceManager.h"

#include "BehaviorTree/BTFunctionLibrary.h"

UBTTask_Support::UBTTask_Support()
{
   NodeName = "Support";
   bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_Support::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   AUnitController* unitControllerRef = Cast<AUnitController>(ownerComp.GetAIOwner());
   if (unitControllerRef->SearchAndCastSpell(ResourceManager::supportTags))
      return EBTNodeResult::InProgress;
   // if there's no spell to be casted
   return EBTNodeResult::Failed;
}

void UBTTask_Support::OnMessage(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, FName message, int32 requestID, bool bSuccess)
{
   //Finishes task if we get a message representing a successful action or if we get stunned or something
   if (message == AUnit::AIMessage_SpellCasted) {
      Super::OnMessage(ownerComp, nodeMemory, message, requestID, bSuccess);
   }
   else { //finishes task if our spell casting gets interrupted
      bSuccess &= (message != AUnit::AIMessage_SpellInterrupt);
      Super::OnMessage(ownerComp, nodeMemory, message, requestID, bSuccess);
   }
}



