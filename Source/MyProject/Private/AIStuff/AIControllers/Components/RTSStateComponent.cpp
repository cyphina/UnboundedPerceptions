#include "RTSStateComponent.h"
#include "RTSStateMachine.h"
<<<<<<< HEAD
=======
#include "Unit.h"
>>>>>>> componentrefactor
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
<<<<<<< HEAD
   stateMachine = StateMachineFactory::BuildStateMachine(Cast<AUnit>(GetOwner()));
   Cast<AUnitController>(GetOwner())->OnUnitStopped().AddUObject(this, &URTSStateComponent::OnUnitStopped);
=======
   Super::BeginPlay();
   GetWorld()->GetTimerManager().SetTimerForNextTick([this]() {
      if(AUnitController* ownerController = Cast<AUnitController>(GetOwner()))
      {
         stateMachine = StateMachineFactory::BuildStateMachine(Cast<AUnit>(ownerController->GetPawn()));
         if(stateMachine)
         {
            ownerController->OnUnitStopped().AddUObject(this, &URTSStateComponent::OnUnitStopped);
         }
      }
   });
>>>>>>> componentrefactor
}

void URTSStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
<<<<<<< HEAD
=======
   Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
>>>>>>> componentrefactor
   stateMachine->Update(DeltaTime);
}

void URTSStateComponent::OnUnitStopped()
{
   ChangeState(EUnitState::STATE_IDLE);
}
