// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "PassiveModeService.generated.h"

/**
 * Service for passive 
 */

class AUserInput;

UCLASS()
class MYPROJECT_API UPassiveModeService : public UBTService
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, Meta=(AllowPrivateAccess = "true", ExposeOnSpawn="true"))
	AUserInput* controllerRef;	

	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
