#pragma once
// Leave these headers here so that classes that include StateMachine can see the relationships as well
#include "IUnitState.h"
#include "AttackState.h"
#include "IdleState.h"
#include "MovingState.h"
#include "CastingState.h"
#include "ChannelingState.h"
#include "InteractState.h"
#include "ItemState.h"
#include "ChasingState.h"

DECLARE_STATS_GROUP(TEXT("UnitStateMachine"), STATGROUP_UnitStateMachine, STATCAT_Advanced);

class StateMachine
{
public:
   StateMachine(AUnit* unitOwner);
   virtual ~StateMachine();

   EUnitState GetCurrentState() const { return currentState->GetName(); }

   virtual void ChangeState(EUnitState newState);
   void         Update(float deltaSeconds);

   // As long as our states don't actually hold any state, we can keep em static
   static IdleState       Idle;
   static MovingState     Moving;
   static AttackState     Attacking;
   static CastingState    Casting;
   static ChannelingState Channeling;
   static ChasingState    Chasing;

protected:
   AUnit*      unitOwner = nullptr;
   IUnitState* currentState;

private:
   virtual IUnitState* getStateFromEnum(EUnitState enumVal);
};
