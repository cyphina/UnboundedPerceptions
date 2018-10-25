// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "IUnitState.h"

/**
 * When a hero is currently interacting with an object
 */

class ABaseHero;

class MYPROJECT_API InteractState : public IUnitState
{
   ABaseHero* heroRef;

 public:
   InteractState(ABaseHero* newHeroRef);
   ~InteractState();

   virtual void       Enter(AUnit& unit) override;
   virtual void       Exit(AUnit& unit) override;
   virtual void       Update(AUnit& unit, float deltaSeconds) override;
   virtual EUnitState GetName() const override { return EUnitState::STATE_INTERACTING; }
};
