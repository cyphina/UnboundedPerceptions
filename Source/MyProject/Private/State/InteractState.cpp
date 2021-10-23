// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "InteractState.h"
#include "WorldObjects/BaseHero.h"

DECLARE_CYCLE_STAT(TEXT("Hero Interaction"), STAT_HeroInteraction, STATGROUP_UnitStateMachine)

InteractState::InteractState(ABaseHero* newHeroRef)
{
   heroRef = newHeroRef;
}

InteractState::~InteractState()
{
}

void InteractState::Enter(AUnit& unit)
{
}

void InteractState::Exit(AUnit& unit)
{
}

void InteractState::Update(AUnit& unit, float deltaSeconds)
{
   // Only times what is in scope.  Remove after fixing hotspots, since stat tracking is expensive
   SCOPE_CYCLE_COUNTER(STAT_HeroInteraction);
   {
      // heroRef->PrepareInteract();
   }
}
