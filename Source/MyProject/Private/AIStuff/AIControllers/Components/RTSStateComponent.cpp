#include "RTSStateComponent.h"
#include "RTSStateMachine.h"
#include "Unit.h"
#include "StateMachineFactory.h"
#include "UnitController.h"

EUnitState URTSStateComponent::GetState() const
{
   return stateMachine->GetCurrentState();
}

IUnitState* URTSStateComponent::GetStateObject() const
{
   return stateMachine->GetStateObject();
}

const ChannelingState& URTSStateComponent::GetChannelingState() const
{
   return stateMachine->GetChannelState();
}

void URTSStateComponent::ChangeState(EUnitState newState) const
{
   stateMachine->ChangeState(newState);
}

void URTSStateComponent::BeginPlay()
{
   Super::BeginPlay();
   if(AUnitController* ownerController = Cast<AUnitController>(GetOwner())) { stateMachine = StateMachineFactory::BuildStateMachine(ownerController->GetUnitOwner()); }
   Cast<AUnitController>(GetOwner())->OnUnitStopped().AddUObject(this, &URTSStateComponent::OnUnitStopped);
}

void URTSStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
   Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
   stateMachine->Update(DeltaTime);
}

void URTSStateComponent::OnUnitStopped()
{
   ChangeState(EUnitState::STATE_IDLE);
}
