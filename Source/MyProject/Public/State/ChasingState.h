#pragma once
#include "IUnitState.h"

/**
 * Represents a unit that has lost vision over its target and is trying to find it again
 * Used for AI since player commands stop if a unit gets out of vision or perhaps the unit will walk to where it last saw the target before it goes out of vision
 */
class MYPROJECT_API ChasingState : public IUnitState
{
 public:
   ChasingState() = default;
   void Enter(AUnit& unit) override;
   void Exit(AUnit& unit) override;
   void Update(AUnit& unit, float deltaSeconds) override;

   EUnitState GetName() const override { return EUnitState::STATE_CHASING; }
   ~ChasingState() = default;
};
