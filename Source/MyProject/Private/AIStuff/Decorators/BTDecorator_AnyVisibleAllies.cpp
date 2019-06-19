// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BTDecorator_AnyVisibleAllies.h"
#include "RTSGameState.h"

bool UBTDecorator_AnyVisibleAllies::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
   return (bool)Cast<ARTSGameState>(GetWorld()->GetGameState())->visibleAllies.Num();
}
