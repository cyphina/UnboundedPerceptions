// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Stop.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API UBTTask_Stop : public UBTTaskNode
{
   GENERATED_BODY()

   UBTTask_Stop();
   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
};
