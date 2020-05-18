// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_CastSpell.generated.h"

class UMySpell;

/**
 * Spell casting task where the spell and target is chosen by the designer.  The spell is a property where the target is the blackboard key
 */
UCLASS()
class MYPROJECT_API UBTTask_CastSpell : public UBTTask_BlackboardBase
{
   GENERATED_UCLASS_BODY()

   UPROPERTY(EditAnywhere)
   TSubclassOf<UMySpell> spellToCast;

   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
   virtual void        OnMessage(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, FName message, int32 requestID, bool bSuccess) override;
};
