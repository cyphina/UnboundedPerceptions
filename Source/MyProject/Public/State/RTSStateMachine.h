#pragma once
<<<<<<< HEAD
// Leave these headers here so that classes that include StateMachine can see the relationships as well
=======

#include "AttackMoveState.h"
>>>>>>> componentrefactor
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
<<<<<<< HEAD
 public:
   explicit RTSStateMachine(AUnit* unitOwner);
   virtual ~RTSStateMachine();
=======
public:
   explicit RTSStateMachine(AUnit* unitOwner);
   virtual  ~RTSStateMachine();
>>>>>>> componentrefactor

   EUnitState GetCurrentState() const { return currentState->GetName(); }

   virtual void ChangeState(EUnitState newState);
<<<<<<< HEAD
   IUnitState*  GetStateObject() const { return currentState; }
   const ChannelingState&  GetChannelState() const { return Channeling; }
   void         Update(float deltaSeconds);
=======

   IUnitState* GetStateObject() const { return currentState; }

   const ChannelingState& GetChannelState() const { return Channeling; }

   void Update(float deltaSeconds);
>>>>>>> componentrefactor

   // As long as our states don't actually hold any state, we can keep em static
   static IdleState        Idle;
   static MovingState      Moving;
   AttackState             Attacking;
<<<<<<< HEAD
=======
   AttackMoveState         AttackMove;
>>>>>>> componentrefactor
   CastingState            Casting;
   static IncantationState Incanting;
   ChannelingState         Channeling;
   static ChasingState     Chasing;

<<<<<<< HEAD
 protected:
   AUnit*      unitOwner = nullptr;
   IUnitState* currentState;

 private:
=======
protected:
   AUnit*      unitOwner = nullptr;
   IUnitState* currentState;

private:
>>>>>>> componentrefactor
   virtual IUnitState* getStateFromEnum(EUnitState enumVal);
};
