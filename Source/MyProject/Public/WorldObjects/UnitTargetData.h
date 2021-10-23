#pragma once
#include "GameplayAbilityTargetTypes.h"
#include "Misc/TVariant.h"

class AUnit;

struct UnitTargetData {
   /** Discriminated union of all the types used for targeting:
    * AActor* - Used for interactable related targeting and manual interactable spell targeting
    * AUnit* - Used for regular auto attack based targeting and manual spell targeting
    * FVector - Used for movement and manual spell targeting
    * FGameplayAbilityTargetDataHandle* - Used for spell targeting and AI spell targeting
    */
   TVariant<FEmptyVariantState, AActor*, AUnit*, FVector, FGameplayAbilityTargetDataHandle> target;

   /** Resets all this unit's targetting information */
   void ResetTarget();
};

inline void UnitTargetData::ResetTarget()
{
   target.Set<FEmptyVariantState>(FEmptyVariantState());
}
