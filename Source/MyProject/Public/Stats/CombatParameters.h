#pragma once
#include "RTSCircularBuffer.h"
#include "Runtime/Core/Public/Containers/Map.h"

UENUM(BlueprintType)
enum class ECombatType : uint8 { Melee, Ranged, Magic };

struct UPAICombatParameters {
   float            currentAttTime   = 0.f;   // keeps track of how long we wait between autoattacks
   float            currentChaseTime = 0.f;   // time the enemy has been attempting to chase us
   bool             readyToAttack    = false; // True if our target is in range and we're in attack state
   bool             isDead           = false;
   bool             isEnemy          = false;
   ECombatType      combatStyle;               // type of attack we autoattack with
   static const int attackRangeCancel = 350.f; // distance that an attack in progress will cancel since it is out of range

   RTSCircularBuffer<TPair<int, float>> damageDealt     = RTSCircularBuffer<TPair<int, float>>(30);
   RTSCircularBuffer<TPair<int, float>> healingDealt    = RTSCircularBuffer<TPair<int, float>>(30);
   RTSCircularBuffer<TPair<int, float>> stunDealt       = RTSCircularBuffer<TPair<int, float>>(30);
   RTSCircularBuffer<TPair<int, float>> healingRecieved = RTSCircularBuffer<TPair<int, float>>(30);
   RTSCircularBuffer<TPair<int, float>> damageRecieved  = RTSCircularBuffer<TPair<int, float>>(30);

   float GetDPS(float timespan, float currentWorldTime);
   float GetDamageRecievedPerSecond(float timespan, float currentWorldTime);
   float GetHealingPerSecond(float timespan, float currentWorldTime);
   float GetHealingRecievedPerSecond(float timespan, float currentWorldTime);

   // float								GetStunPerSecond(float timespan);
};