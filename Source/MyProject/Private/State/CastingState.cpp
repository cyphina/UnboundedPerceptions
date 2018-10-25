// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "CastingState.h"
#include "WorldObjects/Unit.h"

CastingState::CastingState()
{
}

CastingState::~CastingState()
{
}

void CastingState::Enter(AUnit& unit)
{
}

void CastingState::Exit(AUnit& unit)
{
}

void CastingState::Update(AUnit& unit, float deltaSeconds)
{
   if (unit.targetData.spellTargetData.Num() > 0) // due to polymorphic nature of this data structure, its num will be greater than 0 when it has relevant information
   {
      unit.PrepareCastSpell();
   }
}
