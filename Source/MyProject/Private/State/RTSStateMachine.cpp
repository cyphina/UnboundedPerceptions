#include "MyProject.h"
#include "RTSStateMachine.h"

MovingState       RTSStateMachine::Moving = MovingState();
IdleState         RTSStateMachine::Idle = IdleState();
CastingState      RTSStateMachine::Casting = CastingState();
ChannelingState   RTSStateMachine::Channeling = ChannelingState();
IncantationState   RTSStateMachine::Incanting = IncantationState();
ChasingState      RTSStateMachine::Chasing = ChasingState();

RTSStateMachine::RTSStateMachine(AUnit* unitOwner)
{
   currentState    = &Idle;
   this->unitOwner = unitOwner;
}

void RTSStateMachine::ChangeState(EUnitState newState)
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

void RTSStateMachine::Update(float deltaSeconds)
{
   currentState->Update(*unitOwner, deltaSeconds);
}

IUnitState* RTSStateMachine::getStateFromEnum(EUnitState enumVal)
{
   switch (enumVal) {
      case EUnitState::STATE_IDLE: return &Idle;
      case EUnitState::STATE_ATTACKING: return &Attacking;
      case EUnitState::STATE_CASTING: return &Casting;
      case EUnitState::STATE_INCANTATION: return &Incanting;
      case EUnitState::STATE_CHANNELING: return &Channeling;
      case EUnitState::STATE_MOVING: return &Moving;
      case EUnitState::STATE_CHASING: return &Chasing;
      default: return nullptr;
   }
}

RTSStateMachine::~RTSStateMachine()
{
}
