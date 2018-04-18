#include "MyProject.h"
#include "StateMachine.h"

MovingState StateMachine::Moving = MovingState();
AttackState StateMachine::Attacking = AttackState();
IdleState StateMachine::Idle = IdleState();
InteractState StateMachine::Interacting = InteractState();
CastingState StateMachine::Casting = CastingState();
ItemState StateMachine::UsingItem = ItemState();

void StateMachine::ChangeState(AUnit & unit, IUnitState * newState)
{
	//if we currently have a state in our state machine
	if (!currentState)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Error, currentState is nullptr"));
	}

	if(newState != currentState) //make sure we're not just switching to the same state >_>
	{ 
		currentState->Exit(unit);
		if (newState)
		{
			currentState = newState;
			currentState->Enter(unit);
		}
	}

}

StateMachine::StateMachine()
{
	currentState = &StateMachine::Idle;
}


StateMachine::~StateMachine()
{
}
