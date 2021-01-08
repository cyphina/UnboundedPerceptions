// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

/**
 * Interface for UnitStates, which are classes of states inside an FSM.
 * The state machines along with the behavior trees define a unit's behavior
 * The state machine provides looping functionality, while the trees describe the flow and set transitions
 * between the states.
 * For player controlled units, the player's actions will manually change states.
 */

UENUM(BlueprintType)
enum class EUnitState : uint8 {
   STATE_IDLE,
   STATE_MOVING,
   STATE_ATTACKING,   // Attacking an enemy
   STATE_ATTACK_MOVE, // Attacking while moving
   STATE_CHASING, // Chase a unit targetted when it goes out of vision
   STATE_INTERACTING, // Moving to interacting position
   STATE_CASTING,     // Moving to spell position
   STATE_ITEM,        // Moving to item position
   STATE_INCANTATION, // Startup time for spell/item
   STATE_CHANNELING // Channeling time for spells  
};

class AUnit;

class MYPROJECT_API IUnitState
{
 public:
   virtual ~IUnitState() {}
   virtual void       Update(AUnit& unit, float deltaSeconds) = 0;
   virtual void       Enter(AUnit& unit)                      = 0;
   virtual void       Exit(AUnit& unit)                       = 0;
   virtual EUnitState GetName() const                         = 0;
};
