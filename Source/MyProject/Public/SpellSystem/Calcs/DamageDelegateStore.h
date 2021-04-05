#pragma once
#include "RTSDamageCalculation.h"

struct FUpDamage;
class UUpDamageComponent;

<<<<<<< HEAD
DECLARE_EVENT_OneParam(URTSDamageCalculation, FOnDamageTaken, const FUpDamage&);
DECLARE_EVENT_OneParam(URTSDamageCalculation, FOnDamageDealt, const FUpDamage&);
=======
DECLARE_EVENT_OneParam(URTSDamageCalculation, FOnDamageDealt, const FUpDamage&);
DECLARE_EVENT_OneParam(URTSHealthReplenishCalculation, FOnHealingDealt, const FUpDamage&);
>>>>>>> componentrefactor

namespace DamageEvents
{
   /** Callback when a damage event occurs (somebody either took or dodged an attacK and somebody else dealt the attack)
    * For unit specific damage callbacks, look in Unit.h
    */
<<<<<<< HEAD
   inline FOnDamageTaken OnDamageEvent;
=======
   inline FOnDamageDealt OnDamageEvent;

   inline FOnHealingDealt OnHealingEvent;
>>>>>>> componentrefactor
}
