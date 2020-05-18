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
 * Task which directs unit to move to next patrol location.  Relies on the unit having a patrol component else it fails.  
 * Patrol component ended up being the best place to store the control points since the points are really a decorative member.
 * Does not initiate any attacks on its own
 */
UCLASS()
class MYPROJECT_API UBTTask_Patrol : public UBTTaskNode
{
   GENERATED_BODY()

   FName locKeyName    = "moveToLocation";
   FName targetKeyName = "target";

 public:
   UBTTask_Patrol(const FObjectInitializer& ObjectInitializer);

   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

   EBTNodeResult::Type PatrolToNextLocation(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory);
   uint16              GetInstanceMemorySize() const override;
   virtual void        OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess) override;
};
