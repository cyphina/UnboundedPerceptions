// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "IUnitState.h"

/**
 * Occurs when the unit is moving in place to cast a spell
 */
class MYPROJECT_API CastingState : public IUnitState {
public:
   CastingState();
   ~CastingState();
   virtual void Enter(AUnit& unit) override;
   virtual void Exit(AUnit& unit) override;
   virtual void Update(AUnit& unit, float deltaSeconds) override;

   virtual EUnitState GetName() const override { return EUnitState::STATE_CASTING; }
};
