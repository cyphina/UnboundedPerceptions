#include "SpellTargetingTypes.h"
#include "Unit.h"

AActor* FUpSpellTargeting::GetHitActor(const FHitResult& hitResult)
{
   return hitResult.Actor.Get();
}

AUnit* FUpSpellTargeting::GetHitUnit(const FHitResult& hitResult)
{
   return Cast<AUnit>(GetHitActor(hitResult));
}

bool FUpSpellTargeting::CheckValidHitActor(const FHitResult& hitResult)
{
   return IsValid(GetHitActor(hitResult));
}

bool FUpSpellTargeting::IsFriendTargetTag() const
{
   return GetTargetTag().GetTagName() == "Skill.Targeting.Single.Friendly";
}

bool FUpSpellTargeting::IsEnemyTargetTag() const
{
   return GetTargetTag().GetTagName() == "Skill.Targeting.Single.Enemy";
}

bool FUpSpellTargeting::IsInteractableTargetTag() const
{
   return GetTargetTag().GetTagName() == "Skill.Targeting.Single.Interactable";
}
