// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BTTask_Aggresion.h"

#include "WorldObjects/Unit.h"
#include "UnitController.h"

#include "UpResourceManager.h"
#include "BehaviorTree/BTFunctionLibrary.h"

UBTTask_Aggresion::UBTTask_Aggresion()
{
   NodeName = "Aggression";
   bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_Aggresion::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   AUnitController* unitControllerRef = Cast<AUnitController>(ownerComp.GetAIOwner());
   if (unitControllerRef->SearchAndCastSpell(UpResourceManager::offensiveTags))
   {   
      WaitForMessage(ownerComp, AUnit::AIMessage_SpellCasted);
      WaitForMessage(ownerComp, AUnit::AIMessage_SpellInterrupt);
      WaitForMessage(ownerComp, AUnit::AIMessage_TargetLoss);
      return EBTNodeResult::InProgress;
   }
   // if there's no spell to be casted
   return EBTNodeResult::Failed;
}

void UBTTask_Aggresion::OnMessage(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, FName message, int32 requestID, bool bSuccess)
{
   //Finishes task if we get a message representing a successful action or if we get stunned or something
   bSuccess = message == AUnit::AIMessage_SpellCasted & message != AUnit::AIMessage_SpellInterrupt & message != AUnit::AIMessage_TargetLoss;
   Super::OnMessage(ownerComp, nodeMemory, message, requestID, bSuccess);
}


