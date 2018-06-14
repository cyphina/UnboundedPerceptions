// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BTTask_AttTarget.h"
#include "WorldObjects/Unit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIControllers/UnitController.h"
#include "State/AttackState.h"

EBTNodeResult::Type UBTTask_AttTarget::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	AUnitController* AICon = Cast<AUnitController>(ownerComp.GetAIOwner());
	if (AICon)
	{
		AUnit* target = Cast<AUnit>(ownerComp.GetBlackboardComponent()->GetValueAsObject("target"));
		if (target && target->GetCanTarget())
		{
			//AICon->GetUnitOwner()->SetTarget(target);
			//if we aren't already attacking this target
			if(AICon->GetUnitOwner()->GetState() == EUnitState::STATE_ATTACKING && AICon->GetUnitOwner()->GetTargetUnit() != target)
			{
				AICon->GetUnitOwner()->BeginAttack(target);
				return EBTNodeResult::Succeeded;
			}
		}
		else
		{
			ownerComp.GetBlackboardComponent()->SetValueAsObject("target", nullptr);
		}
	}
	return EBTNodeResult::Failed;
}
