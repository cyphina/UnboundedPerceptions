#pragma once
#include "GameplayAbilityTargetTypes.h"
#include "Misc/TVariant.h"

class AUnit;

struct UnitTargetData {
   TVariant<FEmptyVariantState, AActor*, AUnit*, FVector> target; // Used for any targetting required when a user issues some command

   AUnit* followTarget; // A target that we can follow.  We can still target something else to attack it while following this unit perhaps?

   void ResetTarget(); // Resets all this unit's targetting information
};

inline void UnitTargetData::ResetTarget()
{
   target.Set<FEmptyVariantState>(FEmptyVariantState());
   followTarget = nullptr;
}
