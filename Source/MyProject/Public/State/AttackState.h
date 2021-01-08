#pragma once
#include "IUnitState.h"

/**
 * Represents state when unit is attacking (with auto attacks)
 * Some notable details about this state:

 */
class MYPROJECT_API AttackState : public IUnitState
{
 public:
   virtual void Enter(AUnit& unit) override;
   virtual void Exit(AUnit& unit) override;
   virtual void Update(AUnit& unit, float deltaSeconds) override;

   virtual EUnitState GetName() const override { return EUnitState::STATE_ATTACKING; }

   friend class AttackMoveState; // Friendship with attack move state since it also uses similar attacking logic
};
