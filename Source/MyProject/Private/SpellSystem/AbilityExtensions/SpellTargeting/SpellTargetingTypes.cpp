#include "SpellTargetingTypes.h"
#include "Unit.h"

AActor* UUpSpellTargeting::GetHitActor(const FHitResult& hitResult)
{
   return hitResult.Actor.Get();
}

AUnit* UUpSpellTargeting::GetHitUnit(const FHitResult& hitResult)
{
   return Cast<AUnit>(GetHitActor(hitResult));
}

bool UUpSpellTargeting::CheckValidHitActor(const FHitResult& hitResult)
{
   return IsValid(GetHitActor(hitResult));
}