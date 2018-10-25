// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttTarget.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API UBTTask_AttTarget : public UBTTaskNode
{
   GENERATED_BODY()

 public:
   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
};
