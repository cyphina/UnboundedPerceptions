// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "IUnitState.h"

/**
 * Represents when a unit is channeling an ability
 */

class MYPROJECT_API IncantationState : public IUnitState
{
 public:
   IncantationState();
   virtual void       Enter(AUnit& unit) override;
   virtual void       Exit(AUnit& unit) override;
   virtual void       Update(AUnit& unit, float deltaSeconds) override;
   virtual EUnitState GetName() const override { return EUnitState::STATE_INCANTATION; }
   ~IncantationState();
};
