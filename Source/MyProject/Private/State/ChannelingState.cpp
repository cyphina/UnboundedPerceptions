// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "ChannelingState.h"
#include "Unit.h"
#include "MySpell.h"

ChannelingState::ChannelingState()
{
}

ChannelingState::~ChannelingState()
{
}

void ChannelingState::Enter(AUnit& unit)
{
	//Add channel UI bar to the screen
	unit.channelTime = unit.GetCurrentSpell().GetDefaultObject()->GetCastTime(unit.GetAbilitySystemComponent());
}

void ChannelingState::Exit(AUnit& unit)
{
	unit.currentChannelTime = 0;
}

void ChannelingState::Update(AUnit& unit, float deltaSeconds)
{
	if(unit.currentChannelTime < unit.channelTime)
		unit.currentChannelTime += deltaSeconds;
	else
		unit.CastSpell(unit.GetCurrentSpell());
}
