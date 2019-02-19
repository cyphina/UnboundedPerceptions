// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttTarget.generated.h"

/**
 *Attacks a target and on sucessful attack completes the task.  To peform N attacks, use a loop decorator
 */
UCLASS()
class MYPROJECT_API UBTTask_AttTarget : public UBTTaskNode
{
   GENERATED_BODY()

 public:

   UBTTask_AttTarget();
   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
   virtual void        OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess) override;
};
