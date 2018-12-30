// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Action.generated.h"

/**
 * Randomly pick a number and set the blackboard key to that value.  Numbers picked based on the numberCap, which is the number the dice (0-100) has to roll lower than to 
 * pick that number 
 */

UCLASS()
class MYPROJECT_API UBTTask_Action : public UBTTask_BlackboardBase
{
   GENERATED_BODY()

   UPROPERTY(EditAnywhere)
   TArray<int> numberCap;

   UBTTask_Action();
   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
};
