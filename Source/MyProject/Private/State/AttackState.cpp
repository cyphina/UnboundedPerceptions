// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "AttackState.h"
#include "RTSGameState.h"
#include "Unit.h"

AttackState::AttackState()
{
}

AttackState::~AttackState()
{
}

void AttackState::Enter(AUnit& unit)
{
}

void AttackState::Exit(AUnit& unit)
{
   // GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("WEE"));
}

void AttackState::Update(AUnit& unit, float deltaSeconds)
{
   if (IsValid(unit.targetData.targetUnit)) unit.PrepareAttack();
}
