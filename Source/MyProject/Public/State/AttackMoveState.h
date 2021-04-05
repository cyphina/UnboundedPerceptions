<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.
=======
>>>>>>> componentrefactor
#pragma once
#include "IUnitState.h"

/**
 * Represents state when unit moves to a location and attacks anything in the way
 * Initially if the player selects a target location to move to, the unit will focus on attacking anything closest to that
 * point. If there is no unit close to that point, the unit will just attack whatever it sees as it moves into position.
 */
class MYPROJECT_API AttackMoveState : public IUnitState
{
<<<<<<< HEAD
 public:
   AttackMoveState();
   virtual void Enter(AUnit& unit) override;
   virtual void Exit(AUnit& unit) override;
   virtual void Update(AUnit& unit, float deltaSeconds) override;

   virtual EUnitState GetName() const override { return EUnitState::STATE_ATTACKING; }

   ~AttackMoveState();

 private:
   static const int attackRangeCancel = 350.f; // distance that an attack in progress will cancel since it is out of range
   float            currentAttTime    = 0.f;   // keeps track of how long we wait between autoattacks
   bool             readyToAttack     = false; // True if our target is in range and we're in attack state
=======
public:
   AttackMoveState();
   void Enter(AUnit& unit) override;
   void Exit(AUnit& unit) override;
   void Update(AUnit& unit, float deltaSeconds) override;

   EUnitState GetName() const override { return EUnitState::STATE_ATTACK_MOVE; }

   ~AttackMoveState();

private:
   FVector attackMoveLocation;
   
>>>>>>> componentrefactor
};
