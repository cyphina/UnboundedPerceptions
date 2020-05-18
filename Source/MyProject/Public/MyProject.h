// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"

#define GROUND_TRACE_ONLY_CHANNEL ECollisionChannel::ECC_GameTraceChannel1
#define ENEMY_CHANNEL ECollisionChannel::ECC_GameTraceChannel2
#define INTERACTABLE_CHANNEL ECollisionChannel::ECC_GameTraceChannel3
#define NPC_CHANNEL ECollisionChannel::ECC_GameTraceChannel4
#define TRIGGER_CHANNEL ECollisionChannel::ECC_GameTraceChannel5
#define VISION_BLOCKER_CHANNEL ECollisionChannel::ECC_GameTraceChannel6
#define SELECTABLE_BY_CLICK_CHANNEL ECollisionChannel::ECC_GameTraceChannel7
#define FRIENDLY_CHANNEL ECollisionChannel::ECC_GameTraceChannel9
#define FADE_CHANNEL ECollisionChannel::ECC_GameTraceChannel12
#define UNIT_VISION_CHANNEL ECollisionChannel::ECC_GameTraceChannel13

#define WORLD_STATIC_OBJECT_QUERY EObjectTypeQuery::ObjectTypeQuery1
#define ENEMY_OBJECT_QUERY EObjectTypeQuery::ObjectTypeQuery7
#define INTERACTABLE_OBJECT_QUERY EObjectTypeQuery::ObjectTypeQuery8
#define NPC_OBJECT_QUERY EObjectTypeQuery::ObjectTypeQuery9
#define TRIGGER_OBJECT_QUERY EObjectTypeQuery::ObjectTypeQuery10
#define VISION_BLOCKER_OBJECT_QUERY EObjectTypeQuery::ObjectTypeQuery11
#define FRIENDLY_OBJECT_QUERY EObjectTypeQuery::ObjectTypeQuery12