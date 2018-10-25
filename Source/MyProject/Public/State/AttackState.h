// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "IUnitState.h"
/**
 * Represents state when unit is attacking (with auto attacks)
 */
class MYPROJECT_API AttackState : public IUnitState
{
 public:
   AttackState();
   virtual void       Enter(AUnit& unit) override;
   virtual void       Exit(AUnit& unit) override;
   virtual void       Update(AUnit& unit, float deltaSeconds) override;
   virtual EUnitState GetName() const override { return EUnitState::STATE_ATTACKING; }
   ~AttackState();
};
