// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UnitMoveComponent.h"
#include "MyAttributeSet.h"
#include "Unit.h"
#include "UpStatComponent.h"

void UUnitMoveComponent::BeginPlay()
{
   Cast<AUnit>(GetOwner())->statComponent->GetAttSet()->statUpdatedEvent.AddUObject(this, &UUnitMoveComponent::OnMoveSpeedChanged);
}

void UUnitMoveComponent::OnMoveSpeedChanged(const FGameplayAttribute& att, float& newVal, AUnit* unitRef)
{
   if(att == UMyAttributeSet::MovementSpeedAttribute()) { // Update movement speed on character component when move speed attribute changes
      MaxWalkSpeed = newVal;
   }
}

