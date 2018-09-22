// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Aggresion.generated.h"

/**
 * Act of aggression involves:
 * 1. Wait till an enemy appears on screen to act
 * 2. Choose an offensive spell not on CD
 * 3. Choose best target (lowest HP focus)
 * 4. Cast spells and attack 
 */

class AUserInput;
class AUnitController;
class AAlly;
class AUnit;
class UMySpell;

struct FBTAggresssionTaskMemory
{
	UMySpell*						attackSpell = nullptr;
	int								attackSpellIndex = -1;
	AUnit*							unitRef = nullptr;
	AUnitController*				unitControllerRef = nullptr;
};

/**Uses a single offensive spell (chosen by set priority) with targetting chosen by an aggressive ruleset*/
UCLASS()
class MYPROJECT_API UBTTask_Aggresion : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_Aggresion();

	EBTNodeResult::Type				ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
	void							TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	uint16							GetInstanceMemorySize() const override;
	
	
};
