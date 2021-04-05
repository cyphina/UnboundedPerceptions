// Created 7/24/20 4:27 AM

#pragma once
<<<<<<< HEAD
=======
#include "RTSStateMachine.h"
>>>>>>> componentrefactor
#include "RTSStateComponent.generated.h"

class RTSStateMachine;
class IUnitState;
class ChannelingState;
<<<<<<< HEAD
=======

>>>>>>> componentrefactor
enum class EUnitState : uint8;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API URTSStateComponent : public UActorComponent
{
   GENERATED_BODY()

<<<<<<< HEAD
 public:
=======
public:
>>>>>>> componentrefactor
   EUnitState             GetState() const;
   IUnitState*            GetStateObject() const;
   const ChannelingState& GetChannelingState() const;
   void                   ChangeState(EUnitState newState) const;

<<<<<<< HEAD
 protected:
   void BeginPlay() override;
   void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

 private:
=======
protected:
   void BeginPlay() override;
   void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
>>>>>>> componentrefactor
   void OnUnitStopped();

   TUniquePtr<RTSStateMachine> stateMachine;
};
