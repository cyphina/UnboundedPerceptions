#pragma once

#include "Engine.h"

// Collision channels include both trace and object channels.
// Trace channels involve shooting our ray and seeing what kind of objects it can penetrate.
// Object channel tracing involves emulating moving some object of a certain channel and seeing what it can overlap.
#define GROUND_TRACE_ONLY_CHANNEL ECollisionChannel::ECC_GameTraceChannel1 // Different from static mesh, is actually the trace channel
#define ENEMY_OBJECT_CHANNEL ECollisionChannel::ECC_GameTraceChannel2
#define INTERACTABLE_CHANNEL ECollisionChannel::ECC_GameTraceChannel3
#define NPC_CHANNEL ECollisionChannel::ECC_GameTraceChannel4
#define TRIGGER_CHANNEL ECollisionChannel::ECC_GameTraceChannel5
#define VISION_BLOCKER_CHANNEL ECollisionChannel::ECC_GameTraceChannel6
#define SELECTABLE_BY_CLICK_CHANNEL ECollisionChannel::ECC_GameTraceChannel7
#define ENEMY_TRACE_CHANNEL ECollisionChannel::ECC_GameTraceChannel8
#define ALLY_OBJECT_CHANNEL ECollisionChannel::ECC_GameTraceChannel9
#define ALLY_TRACE_CHANNEL ECollisionChannel::ECC_GameTraceChannel10
#define FADE_CHANNEL ECollisionChannel::ECC_GameTraceChannel12
#define UNIT_VISION_CHANNEL ECollisionChannel::ECC_GameTraceChannel13 // UNIT VISION TRACE blocked by scenery

// Object query lists object channels and are used when we want to query for objects of a certain type
#define WORLD_STATIC_OBJECT_QUERY EObjectTypeQuery::ObjectTypeQuery1
#define ENEMY_OBJECT_QUERY EObjectTypeQuery::ObjectTypeQuery7
#define INTERACTABLE_OBJECT_QUERY EObjectTypeQuery::ObjectTypeQuery8
#define NPC_OBJECT_QUERY EObjectTypeQuery::ObjectTypeQuery9
#define TRIGGER_OBJECT_QUERY EObjectTypeQuery::ObjectTypeQuery10
#define VISION_BLOCKER_OBJECT_QUERY EObjectTypeQuery::ObjectTypeQuery11
#define FRIENDLY_OBJECT_QUERY EObjectTypeQuery::ObjectTypeQuery12
