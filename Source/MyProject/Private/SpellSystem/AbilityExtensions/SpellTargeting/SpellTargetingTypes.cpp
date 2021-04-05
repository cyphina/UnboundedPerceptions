#include "SpellTargetingTypes.h"
#include "Unit.h"

<<<<<<< HEAD
AActor* FUpSpellTargeting::GetHitActor(const FHitResult& hitResult)
=======
AActor* UUpSpellTargeting::GetHitActor(const FHitResult& hitResult)
>>>>>>> componentrefactor
{
   return hitResult.Actor.Get();
}

<<<<<<< HEAD
AUnit* FUpSpellTargeting::GetHitUnit(const FHitResult& hitResult)
=======
AUnit* UUpSpellTargeting::GetHitUnit(const FHitResult& hitResult)
>>>>>>> componentrefactor
{
   return Cast<AUnit>(GetHitActor(hitResult));
}

<<<<<<< HEAD
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
=======
bool UUpSpellTargeting::CheckValidHitActor(const FHitResult& hitResult)
{
   return IsValid(GetHitActor(hitResult));
}
>>>>>>> componentrefactor
