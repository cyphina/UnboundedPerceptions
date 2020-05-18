// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "IUnitState.h"

class ABaseHero;

/**
 * State when using an item: Similar to channeling state?
 */
class MYPROJECT_API ItemState : public IUnitState {

   ABaseHero* heroRef;

public:
   ItemState(ABaseHero* hero);
   ~ItemState();

   virtual void Enter(AUnit& unit) override;
   virtual void Exit(AUnit& unit) override;
   virtual void Update(AUnit& unit, float deltaSeconds) override;

   virtual EUnitState GetName() const override { return EUnitState::STATE_ITEM; }
};
