// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

/**
 * Interface for UnitStates
 */

UENUM(BlueprintType)
enum class EUnitState : uint8
{
	STATE_IDLE,
	STATE_MOVING,
	STATE_ATTACKING,
	STATE_INTERACTING,
	STATE_CASTING,
	STATE_ITEM,
	STATE_CHANNELING
};

class AUnit;

class MYPROJECT_API IUnitState
{
public:
	virtual ~IUnitState() {}
	virtual void Update(AUnit& unit, float deltaSeconds) = 0;
	virtual void Enter(AUnit& unit) = 0;
	virtual void Exit(AUnit& unit) = 0;
	virtual EUnitState GetName() const = 0;
};


