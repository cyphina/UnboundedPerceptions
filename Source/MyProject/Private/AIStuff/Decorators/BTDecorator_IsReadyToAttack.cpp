// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BTDecorator_IsReadyToAttack.h"
#include "UnitController.h"
#include "WorldObjects/Unit.h"

UBTDecorator_IsReadyToAttack::UBTDecorator_IsReadyToAttack()
{
   NodeName = "Is Ready to Attack";
   // For now, don't allow users to select any "Abort Observers", because it's currently not supported.
   bAllowAbortNone       = false;
   bAllowAbortLowerPri   = false;
   bAllowAbortChildNodes = false;
}

bool UBTDecorator_IsReadyToAttack::CalculateRawConditionValue(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) const
{
   return Cast<AUnitController>(ownerComp.GetAIOwner())->GetUnitOwner()->GetIsReadyToAttack();
}
