// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "IdleState.h"

IdleState::IdleState()
{
}

IdleState::~IdleState()
{
}

void IdleState::Enter(AUnit& unit)
{
   // GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Entering Idle"));
}

void IdleState::Exit(AUnit& unit)
{
   // GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Exiting Idle"));
}

void IdleState::Update(AUnit& unit, float deltaSeconds)
{
}
