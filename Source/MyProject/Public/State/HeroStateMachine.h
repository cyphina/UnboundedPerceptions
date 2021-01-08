// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RTSStateMachine.h"

/**
 * State machine specifically for heroes
 */

class ABaseHero;

class MYPROJECT_API HeroStateMachine : public RTSStateMachine {
public:
   explicit HeroStateMachine(ABaseHero* hero);
   virtual ~HeroStateMachine();

   InteractState Interacting;
   ItemState     UsingItem;

   virtual void ChangeState(EUnitState newState) override;

private:
   virtual IUnitState* getStateFromEnum(EUnitState enumVal) override;

};