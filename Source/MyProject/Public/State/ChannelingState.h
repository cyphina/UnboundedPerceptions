// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "IUnitState.h"

/**
 * Some spells can be channeled after it has been initially cast (like Snowstorm which continually creates snow in an area after the incantation until
 * the player cancels the command or the channeling duration is over).  Uses similar variables to incantation state but serves a completely different purpose.  
 */

class MYPROJECT_API ChannelingState : public IUnitState
{
 public:
   ChannelingState();
   virtual void       Enter(AUnit& unit) override;
   virtual void       Exit(AUnit& unit) override;
   virtual void       Update(AUnit& unit, float deltaSeconds) override;
   virtual EUnitState GetName() const override { return EUnitState::STATE_CHANNELING; }
   virtual ~ChannelingState();
};
