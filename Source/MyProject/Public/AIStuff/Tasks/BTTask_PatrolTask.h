// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_PatrolTask.generated.h"

class AUnitController;
class UBlackboardComponent;
class UPatrolComponent;

struct FBTPatrolTaskMemory {
   AAIController*  AICon      = nullptr;
   UPatrolComponent* patrolComp = nullptr;
};

/**
 * Task which makes a unit or NPC patrol a fixed set of points defined in a UPatrolComponent.
 * Has no logic to start attacking if an enemy is found - that would have to be added as a decorator or parallel task.
 */
UCLASS()
class MYPROJECT_API UBTTask_Patrol : public UBTTaskNode
{
   GENERATED_BODY()

   FName locKeyName    = "moveToLocation";
   FName targetKeyName = "target";

 public:
   UBTTask_Patrol(const FObjectInitializer& ObjectInitializer);

   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override final;

   EBTNodeResult::Type PatrolToNextLocation(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory);
   uint16              GetInstanceMemorySize() const override final;
   virtual void        OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess) override final;
};
