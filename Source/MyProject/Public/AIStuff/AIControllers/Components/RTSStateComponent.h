// Created 7/24/20 4:27 AM

#pragma once
#include "RTSStateMachine.h"
#include "RTSStateComponent.generated.h"

class RTSStateMachine;
class IUnitState;
class ChannelingState;

enum class EUnitState : uint8;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API URTSStateComponent : public UActorComponent
{
   GENERATED_BODY()

public:
   EUnitState             GetState() const;
   IUnitState*            GetStateObject() const;
   const ChannelingState& GetChannelingState() const;
   void                   ChangeState(EUnitState newState) const;

protected:
   void BeginPlay() override;
   void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
   void OnUnitStopped();

   TUniquePtr<RTSStateMachine> stateMachine;
};
