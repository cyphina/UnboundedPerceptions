#pragma once

#include "AttackMoveState.h"
#include "IUnitState.h"
#include "AttackState.h"
#include "IdleState.h"
#include "MovingState.h"
#include "CastingState.h"
#include "IncantationState.h"
#include "ChannelingState.h"
#include "InteractState.h"
#include "ItemState.h"
#include "ChasingState.h"

DECLARE_STATS_GROUP(TEXT("UnitStateMachine"), STATGROUP_UnitStateMachine, STATCAT_Advanced);

class RTSStateMachine
{
public:
   explicit RTSStateMachine(AUnit* unitOwner);
   virtual  ~RTSStateMachine();

   EUnitState GetCurrentState() const { return currentState->GetName(); }

   virtual void ChangeState(EUnitState newState);

   IUnitState* GetStateObject() const { return currentState; }

   const ChannelingState& GetChannelState() const { return Channeling; }

   void Update(float deltaSeconds);

   // As long as our states don't actually hold any state, we can keep em static
   static IdleState        Idle;
   static MovingState      Moving;
   AttackState             Attacking;
   AttackMoveState         AttackMove;
   CastingState            Casting;
   static IncantationState Incanting;
   ChannelingState         Channeling;
   static ChasingState     Chasing;

protected:
   AUnit*      unitOwner = nullptr;
   IUnitState* currentState;

private:
   virtual IUnitState* getStateFromEnum(EUnitState enumVal);
};
