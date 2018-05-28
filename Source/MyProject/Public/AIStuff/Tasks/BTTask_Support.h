// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SpellSystem/MySpell.h"
#include "BTTask_Support.generated.h"

/**
 * Node instances are shared for all behavior trees
 * see https://answers.unrealengine.com/questions/173494/bttask-nodememory.html for more details
 * Task for supporting (unit is on passive AI mode)
 * 
 * The act of supporting involves a few things:
 * 1. Moving to a defensive position
 * 2. Choosing a defensive spell not on cooldown
 * 3. Choose the best target to cast it on (person or area)
 * 4. Cast spell
 */

class AUserInput;
class AUnitController;
class AAlly;
class AUnit;
class UMySpell;

struct FBTSupportTaskMemory
{
	UMySpell*						supportSpell = nullptr;
	int								supportSpellIndex = -1; //use index because we need it to properly set CD
	AUnit*							unitRef = nullptr;
	AUnitController*				unitControllerRef = nullptr;
};

UCLASS()
class MYPROJECT_API UBTTask_Support : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_Support();

	EBTNodeResult::Type				ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
	void							TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	uint16							GetInstanceMemorySize() const override;
};	
