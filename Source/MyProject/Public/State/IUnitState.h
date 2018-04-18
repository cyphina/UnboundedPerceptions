// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
class AUnit;
/**
 * Interface *
 */
class MYPROJECT_API IUnitState
{
public:
	virtual ~IUnitState() {}
	virtual void Update(AUnit& unit) = 0;
	virtual void Enter(AUnit& unit) = 0;
	virtual void Exit(AUnit& unit) = 0;
	virtual FName GetName() const = 0;
};


