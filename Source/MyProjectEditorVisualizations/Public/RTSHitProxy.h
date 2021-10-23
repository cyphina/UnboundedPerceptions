#pragma once
#include "ComponentVisualizer.h"

struct HRTSVisProxy : public HComponentVisProxy
{
	DECLARE_HIT_PROXY();

	HRTSVisProxy(const UActorComponent* inComponent) : HComponentVisProxy(inComponent, HPP_Wireframe) {}
};

struct HPatrolProxy : public HRTSVisProxy
{
	DECLARE_HIT_PROXY();

	HPatrolProxy(const UActorComponent* inComponent, int32 patrolIndex) : HRTSVisProxy(inComponent), patrolIndex(patrolIndex) {}

	int32 patrolIndex;
};