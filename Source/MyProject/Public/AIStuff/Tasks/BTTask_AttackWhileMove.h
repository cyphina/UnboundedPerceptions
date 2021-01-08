// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackWhileMove.generated.h"

/**
 * Task that moves a target but continuously attacks the enemy if the target is in range while moving
 */

class AUnitController;

struct FBTAttackMoveMemory {
   AUnitController* AICon = nullptr;
   FTimerHandle attackTime;
};

UCLASS()
class MYPROJECT_API UBTTask_AttackWhileMove : public UBTTaskNode
{
   GENERATED_BODY()

   UPROPERTY(EditAnywhere, Meta=(ClampMin ="0", ClampMax="120"))
   float attackDelay = 1.f;

   UPROPERTY(EditAnywhere, Meta=(ClampMin ="0", ClampMax="20"))
   float delayVariance = 0.5f;
   
   UBTTask_AttackWhileMove();

   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
   uint16              GetInstanceMemorySize() const override;
   virtual void        OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess) override;
};
