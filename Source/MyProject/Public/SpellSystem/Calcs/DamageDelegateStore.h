#pragma once
#include "RTSDamageCalculation.h"

struct FUpDamage;
class UUpDamageComponent;

DECLARE_EVENT_OneParam(URTSDamageCalculation, FOnDamageDealt, const FUpDamage&);
DECLARE_EVENT_OneParam(URTSHealthReplenishCalculation, FOnHealingDealt, const FUpDamage&);

namespace DamageEvents
{
   /** Callback when a damage event occurs (somebody either took or dodged an attacK and somebody else dealt the attack)
    * For unit specific damage callbacks, look in Unit.h
    */
   inline FOnDamageDealt OnDamageEvent;

   inline FOnHealingDealt OnHealingEvent;
}
