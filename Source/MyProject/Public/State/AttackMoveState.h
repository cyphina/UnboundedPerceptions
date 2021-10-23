#pragma once
#include "IUnitState.h"

/**
 * Represents state when unit moves to a location and attacks anything in the way
 * Initially if the player selects a target location to move to, the unit will focus on attacking anything closest to that
 * point. If there is no unit close to that point, the unit will just attack whatever it sees as it moves into position.
 */
class MYPROJECT_API AttackMoveState : public IUnitState
{
public:
   AttackMoveState();
   void Enter(AUnit& unit) override;
   void Exit(AUnit& unit) override;
   void Update(AUnit& unit, float deltaSeconds) override;

   EUnitState GetName() const override { return EUnitState::STATE_ATTACK_MOVE; }

   ~AttackMoveState();

private:
   FVector attackMoveLocation;
   
};
