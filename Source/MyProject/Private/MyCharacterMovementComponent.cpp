// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "MyCharacterMovementComponent.h"

UMyCharacterMovementComponent::UMyCharacterMovementComponent(const FObjectInitializer& oI) : Super(oI)
{
}

void UMyCharacterMovementComponent::InitializeComponent()
{
   Super::InitializeComponent();
}

void UMyCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
   Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
