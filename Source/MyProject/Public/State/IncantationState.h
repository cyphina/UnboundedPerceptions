// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "IUnitState.h"

/**
 * Represents when a unit is channeling an ability (saying magic words to cast a spell and maybe do some Naruto hand seals).
 * This pretty much happens for every spell - albeit late game character's will have massive cast speed buffs
 */

class MYPROJECT_API IncantationState : public IUnitState
{
 public:
   IncantationState();
   virtual void Enter(AUnit& unit) override;
   virtual void Exit(AUnit& unit) override;
   virtual void Update(AUnit& unit, float deltaSeconds) override;

   virtual EUnitState GetName() const override { return EUnitState::STATE_INCANTATION; }

   ~IncantationState();

 private:
   float currentChannelTime = 0; // Time spent channeling by unit
   float channelTime        = 0; // How long unit has to channel
};
