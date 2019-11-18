// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "ChannelingState.h"
#include "Unit.h"
#include "AIStuff/AIControllers/UnitController.h"
#include "MySpell.h"

IncantationState::IncantationState()
{
}

IncantationState::~IncantationState()
{
}

void IncantationState::Enter(AUnit& unit)
{
   //todo: Add channel UI bar to the screen
   unit.PlayAnimMontage(unit.castAnimation, 1/unit.unitSpellData.channelTime);
}

void IncantationState::Exit(AUnit& unit)
{
   // If we leave casting state before the spell can finish being casted, we've been interrupted so tell the AI system that
   if (unit.unitSpellData.currentChannelTime < unit.unitSpellData.channelTime) {
      FAIMessage msg(AUnit::AIMessage_SpellInterrupt, &unit);
      FAIMessage::Send(unit.GetUnitController(), msg);
   }
   unit.unitSpellData.currentChannelTime = 0;
}

void IncantationState::Update(AUnit& unit, float deltaSeconds)
{
   if (unit.unitSpellData.currentChannelTime < unit.unitSpellData.channelTime)
      unit.unitSpellData.currentChannelTime += deltaSeconds;
   else
      unit.CastSpell(unit.GetCurrentSpell());
}
