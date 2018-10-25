// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_PatrolTask.generated.h"

/**
 * Task which directs unit to move to next patrol location
 */

class AUnitController;
class UBlackboardComponent;

struct FBTPatrolTaskMemory {
   int              patrolIndex;
   AUnitController* AICon = nullptr;
};

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

 protected:
   void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
