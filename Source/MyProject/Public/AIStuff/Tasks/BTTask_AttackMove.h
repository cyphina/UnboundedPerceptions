// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackMove.generated.h"

/**
 * Task that moves a target but continuously attacks the enemy if the target is in range while moving
 */

class AUnitController;

struct FBTAttackMoveMemory {
   AUnitController* AICon = nullptr;
};

UCLASS()
class MYPROJECT_API UBTTask_AttackMove : public UBTTaskNode
{
   GENERATED_BODY()

   UBTTask_AttackMove();

   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
   uint16              GetInstanceMemorySize() const override;
   virtual void        OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess) override;
};
