// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Action.generated.h"

/**
 * Used to choose between a set of actions.
 * Rolls a dice (0-100). Sets the value of the selected key to be an action index based on the roll
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
