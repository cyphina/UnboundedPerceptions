#pragma once
#include "RTSCircularBuffer.h"
#include "Runtime/Core/Public/Containers/Map.h"

UENUM(BlueprintType)
enum class ECombatType : uint8
{
	Melee,
	Ranged,
	Magic
};

struct UPAICombatParameters
{
	float								currentAttTime = 0; //keeps track of how long we wait between autoattacks
	bool								readyToAttack = false; //bool to check if we can autoattack again
	bool								isDead;
	ECombatType							combatStyle; //type of attack we autoattack with

	RTSCircularBuffer<TPair<int, float>>			damageDealt = RTSCircularBuffer<TPair<int,float>>(30);
	RTSCircularBuffer<TPair<int, float>>			healingDealt = RTSCircularBuffer<TPair<int,float>>(30);
	RTSCircularBuffer<TPair<int, float>>			stunDealt = RTSCircularBuffer<TPair<int,float>>(30);
	RTSCircularBuffer<TPair<int, float>>			healingRecieved = RTSCircularBuffer<TPair<int,float>>(30);
	RTSCircularBuffer<TPair<int, float>>			damageRecieved = RTSCircularBuffer<TPair<int,float>>(30);

	float								GetDPS(float timespan, float currentWorldTime);
	float								GetDamageRecievedPerSecond(float timespan, float currentWorldTime);
	float								GetHealingPerSecond(float timespan, float currentWorldTime);
	float								GetHealingRecievedPerSecond(float timespan, float currentWorldTime);

	//float								GetStunPerSecond(float timespan);
};