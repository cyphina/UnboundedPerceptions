#include "MyProject.h"
#include "CombatParameters.h"

float UPAICombatParameters::GetDPS(float timespan, float currentWorldTime)
{
   unsigned int      i                   = 0;
   int               totalDamage         = 0;
   TPair<int, float> currentDamageAmount = damageDealt.GetPrevItem(i);

   while (currentDamageAmount.Value + timespan > currentWorldTime && i < damageDealt.Size()) {
      totalDamage += currentDamageAmount.Key;
      currentDamageAmount = damageDealt.GetPrevItem(++i);
   }
   return totalDamage / timespan;
}

float UPAICombatParameters::GetDamageRecievedPerSecond(float timespan, float currentWorldTime)
{
   unsigned int      i                   = 0;
   int               totalDamageRecieved = 0;
   TPair<int, float> currentDamageAmount = damageRecieved.GetPrevItem(i);

   while (currentDamageAmount.Value + timespan > currentWorldTime && i < damageRecieved.Size()) {
      totalDamageRecieved += currentDamageAmount.Key;
      currentDamageAmount = damageDealt.GetPrevItem(++i);
   }
   return totalDamageRecieved / timespan;
}

float UPAICombatParameters::GetHealingPerSecond(float timespan, float currentWorldTime)
{
   unsigned int      i                   = 0;
   int               totalHealingDealt   = 0;
   TPair<int, float> currentDamageAmount = healingDealt.GetPrevItem(i);

   while (currentDamageAmount.Value + timespan > currentWorldTime && i < healingDealt.Size()) {
      totalHealingDealt += currentDamageAmount.Key;
      currentDamageAmount = healingDealt.GetPrevItem(++i);
   }
   return totalHealingDealt / timespan;
}

float UPAICombatParameters::GetHealingRecievedPerSecond(float timespan, float currentWorldTime)
{
   unsigned int      i                    = 0;
   int               totalHealingRecieved = 0;
   TPair<int, float> currentDamageAmount  = healingRecieved.GetPrevItem(i);

   while (currentDamageAmount.Value + timespan > currentWorldTime && i < healingRecieved.Size()) {
      totalHealingRecieved += currentDamageAmount.Key;
      currentDamageAmount = healingRecieved.GetPrevItem(++i);
   }
   return totalHealingRecieved / timespan;
}
