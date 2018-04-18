// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TargetCheckBTS_Service.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UTargetCheckBTS_Service : public UBTService
{
	GENERATED_BODY()

	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
