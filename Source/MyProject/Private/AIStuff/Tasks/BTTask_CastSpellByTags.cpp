// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BTTask_CastSpellByTags.h"

#include "WorldObjects/Unit.h"
#include "UnitController.h"

#include "UpResourceManager.h"
#include "BehaviorTree/BTFunctionLibrary.h"


UBTTask_CastSpellByTags::UBTTask_CastSpellByTags()
{
   NodeName = "CastSpellByTags";
   bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_CastSpellByTags::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   AUnitController* unitControllerRef = Cast<AUnitController>(ownerComp.GetAIOwner());
   if (unitControllerRef->SearchAndCastSpell(filterTags))
   {
      WaitForMessage(ownerComp, AUnit::AIMessage_SpellCasted);
      WaitForMessage(ownerComp, AUnit::AIMessage_SpellInterrupt);
      WaitForMessage(ownerComp, AUnit::AIMessage_TargetLoss);
      return EBTNodeResult::InProgress;
   }
   // if there's no spell to be casted
   return EBTNodeResult::Failed;
}

void UBTTask_CastSpellByTags::OnMessage(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, FName message, int32 requestID, bool bSuccess)
{
   bSuccess &= message == AUnit::AIMessage_SpellCasted & message != AUnit::AIMessage_SpellInterrupt & message != AUnit::AIMessage_TargetLoss;
   Super::OnMessage(ownerComp, nodeMemory, message, requestID, bSuccess);
}
