#pragma once
//Leave these headers here so that classes that include StateMachine can see the relationships as well
#include "IUnitState.h"
#include "AttackState.h"
#include "IdleState.h"
#include "MovingState.h"
#include "InteractState.h"
#include "CastingState.h"
#include "ItemState.h"

class IdleState;
class MovingState;
class AttackState;
class InteractState;
class CastingState;
class ItemState;

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	IUnitState* GetCurrentState() const { return currentState; }
	void ChangeState(AUnit& unit, IUnitState* newState);

	static IdleState Idle;
	static MovingState Moving;
	static AttackState Attacking;
	static InteractState Interacting;
	static CastingState Casting;
	static ItemState UsingItem;

private:
	IUnitState* currentState = &Idle;
};

