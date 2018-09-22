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
	unit.unitSpellData.channelTime = unit.GetCurrentSpell().GetDefaultObject()->GetCastTime(unit.GetAbilitySystemComponent());
}

void ChannelingState::Exit(AUnit& unit)
{
	unit.unitSpellData.currentChannelTime = 0;
}

void ChannelingState::Update(AUnit& unit, float deltaSeconds)
{
	if(unit.unitSpellData.currentChannelTime < unit.unitSpellData.channelTime)
		unit.unitSpellData.currentChannelTime += deltaSeconds;
	else
		unit.CastSpell(unit.GetCurrentSpell());
}
