#include "MyProject.h"
#include "StateMachine.h"

MovingState       StateMachine::Moving = MovingState();
AttackState       StateMachine::Attacking = AttackState();
IdleState         StateMachine::Idle = IdleState();
CastingState      StateMachine::Casting = CastingState();
ChannelingState   StateMachine::Channeling = ChannelingState();
ChasingState      StateMachine::Chasing = ChasingState();

StateMachine::StateMachine(AUnit* unitOwner)
{
   currentState    = &Idle;
   this->unitOwner = unitOwner;
}

void StateMachine::ChangeState(EUnitState newState)
{
   // if we currently have a state in our state machine
   if (!currentState) { GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Error, currentState is nullptr")); }

   if (newState != currentState->GetName()) // make sure we're not just switching to the same state >_>
   {
      currentState->Exit(*unitOwner);
      currentState = getStateFromEnum(newState);
      currentState->Enter(*unitOwner);
   }
}

void StateMachine::Update(float deltaSeconds)
{
   currentState->Update(*unitOwner, deltaSeconds);
}

IUnitState* StateMachine::getStateFromEnum(EUnitState enumVal)
{
   switch (enumVal) {
      case EUnitState::STATE_IDLE: return &Idle;
      case EUnitState::STATE_ATTACKING: return &Attacking;
      case EUnitState::STATE_CASTING: return &Casting;
      case EUnitState::STATE_CHANNELING: return &Channeling;
      case EUnitState::STATE_MOVING: return &Moving;
      case EUnitState::STATE_CHASING: return &Chasing;
      default: return nullptr;
   }
}

StateMachine::~StateMachine()
{
}
