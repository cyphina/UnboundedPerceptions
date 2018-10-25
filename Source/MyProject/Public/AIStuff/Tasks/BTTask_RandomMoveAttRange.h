// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RandomMoveAttRange.generated.h"

/**
 * Task that sets moveLocation to some random location within attack range
 */

UCLASS()
class MYPROJECT_API UBTTask_RandomMoveAttRange : public UBTTaskNode
{
   GENERATED_BODY()

   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
};
