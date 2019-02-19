// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CastSpellByTags.generated.h"

/**
 *Casts one of the spells in the unit's equipped abilities with a certain gameplay tag.  AI handles what to target
 */
UCLASS()
class MYPROJECT_API UBTTask_CastSpellByTags : public UBTTaskNode
{
   GENERATED_BODY()

   UPROPERTY(EditAnywhere)
   FGameplayTagContainer filterTags;

   UBTTask_CastSpellByTags();

   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
   void                OnMessage(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, FName message, int32 requestID, bool bSuccess) override;
};
