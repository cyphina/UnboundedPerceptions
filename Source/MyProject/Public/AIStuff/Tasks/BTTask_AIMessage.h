// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AIMessage.generated.h"

/**
 * 
 */

UCLASS()
class MYPROJECT_API UBTTask_AIMessage : public UBTTaskNode
{
   GENERATED_BODY()

   UPROPERTY(EditAnywhere)
   FGameplayTag messageNametag;

   const FString tagPrefix = "Combat.AI.Message.";
   UBTTask_AIMessage();

   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
	
};
