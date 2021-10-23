#pragma once
#include "IUnitState.h"

/**
 * State achieved by moving a unit for no other purpose than repositioning 
 */
class MYPROJECT_API MovingState : public IUnitState {
public:
   MovingState() {}

   virtual void Enter(AUnit& unit) override;
   virtual void Exit(AUnit& unit) override;
   virtual void Update(AUnit& unit, float deltaSeconds) override;

   virtual EUnitState GetName() const override { return EUnitState::STATE_MOVING; }

   ~MovingState() {}
};
