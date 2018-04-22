// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "InteractState.h"
#include "WorldObjects/BaseHero.h"

InteractState::InteractState()
{

}

InteractState::~InteractState()
{

}

void InteractState::Enter(AUnit& unit)
{
	heroRef = Cast<ABaseHero>(&unit);
}

void InteractState::Exit(AUnit& unit)
{
}

void InteractState::Update(AUnit& unit, float deltaSeconds)
{
	heroRef->PrepareInteract();
}
