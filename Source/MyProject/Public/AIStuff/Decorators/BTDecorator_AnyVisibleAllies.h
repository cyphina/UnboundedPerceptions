// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_AnyVisibleAllies.generated.h"

/**
 * Figures out of any enemy units are visible (from enemy's perspective)
 */
UCLASS()
class MYPROJECT_API UBTDecorator_AnyVisibleAllies : public UBTDecorator
{

   GENERATED_BODY()

   virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
