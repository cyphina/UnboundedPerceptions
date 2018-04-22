#pragma once
//Leave these headers here so that classes that include StateMachine can see the relationships as well
#include "IUnitState.h"
#include "AttackState.h"
#include "IdleState.h"
#include "MovingState.h"
#include "InteractState.h"
#include "CastingState.h"
#include "ItemState.h"
#include "ChannelingState.h"

class StateMachine
{
public:
	StateMachine(AUnit* unitOwner);
	~StateMachine();

	EUnitState GetCurrentState() const { return currentState->GetName(); }
	void ChangeState(EUnitState newState);
	void Update(float deltaSeconds);

	//As long as our states don't actually hold any state, we can keep em static 
	static IdleState Idle;
	static MovingState Moving;
	static AttackState Attacking;
	static InteractState Interacting;
	static CastingState Casting;
	static ItemState UsingItem;
	static ChannelingState Channeling; 

private:

	AUnit* unitOwner = nullptr;
	IUnitState* currentState;
	IUnitState* getStateFromEnum(EUnitState enumVal);
};

