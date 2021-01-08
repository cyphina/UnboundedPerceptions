#pragma once

class RTSStateMachine;
class AUnit;

struct StateMachineFactory
{
   static TUniquePtr<RTSStateMachine> BuildStateMachine(AUnit* unitToBuildMachineFor);
};
