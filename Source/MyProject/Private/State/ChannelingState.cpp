// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "ChannelingState.h"
#include "Unit.h"
#include "AIStuff/AIControllers/UnitController.h"
#include "MySpell.h"

ChannelingState::ChannelingState()
{
}

ChannelingState::~ChannelingState()
{
}

void ChannelingState::Enter(AUnit& unit)
{
   //todo: Add channel UI bar to the screen
}

void ChannelingState::Exit(AUnit& unit)
{
   // If we leave casting state before the spell can finish being casted, we've been interrupted so tell the AI system that
   if (unit.unitSpellData.currentChannelTime < unit.unitSpellData.channelTime) {
      FAIMessage msg(AUnit::AIMessage_SpellInterrupt, &unit);
      FAIMessage::Send(unit.GetUnitController(), msg);
   }
   unit.unitSpellData.currentChannelTime = 0;
}

void ChannelingState::Update(AUnit& unit, float deltaSeconds)
{
   if (unit.unitSpellData.currentChannelTime < unit.unitSpellData.channelTime)
      unit.unitSpellData.currentChannelTime += deltaSeconds;
   else
      unit.CastSpell(unit.GetCurrentSpell());
}
