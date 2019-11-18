// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "IUnitState.h"

/**
 * Represents when a unit is saying the mystical words to cast a spell.
 */

class MYPROJECT_API ChannelingState : public IUnitState
{
 public:
   ChannelingState();
   virtual void       Enter(AUnit& unit) override;
   virtual void       Exit(AUnit& unit) override;
   virtual void       Update(AUnit& unit, float deltaSeconds) override;
   virtual EUnitState GetName() const override { return EUnitState::STATE_CHANNELING; }
   ~ChannelingState();
};
