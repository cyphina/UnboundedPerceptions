// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Unit.h"

#include "TargetComponent.h"
#include "TargetLocationVisitorDefs.inl"

UTargetComponent::UTargetComponent()
{
   PrimaryComponentTick.bCanEverTick = false;
}

AActor* UTargetComponent::GetTargetActorOrUnit() const
{
   if(targetData.target.IsType<AUnit*>())
      return targetData.target.Get<AUnit*>();
   else if(targetData.target.IsType<AActor*>())
      return targetData.target.Get<AActor*>();
   else
      return nullptr;
}

bool UTargetComponent::IsTargetingSelf() const
{
   return GetTargetUnit() == GetOwner();
}

FVector UTargetComponent::TargetLocationVisitor::operator()(FEmptyVariantState) const
{
   checkf(false, TEXT("Should never call this!"));
   return FVector::ZeroVector;
}

FVector UTargetComponent::TargetLocationVisitor::operator()(FVector v) const
{
   return v;
}

FVector UTargetComponent::TargetLocationVisitor::operator()(AActor* a) const
{
   return a->GetActorLocation();
}

FVector UTargetComponent::TargetLocationVisitor::operator()(AUnit* u) const
{
   return u->GetActorLocation();
}

FVector UTargetComponent::TargetLocationVisitor::operator()(FGameplayAbilityTargetDataHandle h) const
{
   return UAbilitySystemBlueprintLibrary::GetTargetDataEndPoint(h, 0);
}

void UTargetComponent::SetTargetVisitor::operator()(FEmptyVariantState) const
{
   checkf(false, TEXT("Should never call this!"));
}

void UTargetComponent::SetTargetVisitor::operator()(FVector v) const
{
   targetCompRef->SetTargetLocation(v);
}

void UTargetComponent::SetTargetVisitor::operator()(AActor* a) const
{
   targetCompRef->SetTargetActor(a);
}

void UTargetComponent::SetTargetVisitor::operator()(AUnit* u) const
{
   targetCompRef->SetTargetUnit(u);
}

void UTargetComponent::SetTargetVisitor::operator()(FGameplayAbilityTargetDataHandle h) const
{
   targetCompRef->SetSpellTarget(h);
}