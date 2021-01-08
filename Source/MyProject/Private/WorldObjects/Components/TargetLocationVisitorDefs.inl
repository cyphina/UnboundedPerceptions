#pragma once
#include "TargetComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

FGameplayAbilityTargetDataHandle UTargetComponent::TargetDataVisitor::operator()(FEmptyVariantState) const
{
   // If we call BeginCastSpell for a spell that requires no targetting
   return FGameplayAbilityTargetDataHandle();
}

FGameplayAbilityTargetDataHandle UTargetComponent::TargetDataVisitor::operator()(FVector v) const
{
   FGameplayAbilityTargetingLocationInfo tInfo;
   tInfo.LocationType     = EGameplayAbilityTargetingLocationType::LiteralTransform;
   tInfo.LiteralTransform = FTransform(v);
   return UAbilitySystemBlueprintLibrary::AbilityTargetDataFromLocations(tInfo, tInfo);
}

FGameplayAbilityTargetDataHandle UTargetComponent::TargetDataVisitor::operator()(FGameplayAbilityTargetDataHandle h) const
{
   return h;
}

FGameplayAbilityTargetDataHandle UTargetComponent::TargetDataVisitor::operator()(AActor* a) const
{
   return UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(a);
}

FGameplayAbilityTargetDataHandle UTargetComponent::TargetDataVisitor::operator()(AUnit* u) const
{
   return UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(u);
}
