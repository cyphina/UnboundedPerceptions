// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "TargetComponent.h"
#include "TargetLocationVisitor.inl"

UTargetComponent::UTargetComponent()
{
   PrimaryComponentTick.bCanEverTick = false;
}

AActor* UTargetComponent::GetTargetActorOrUnit() const
{
   if(targetData->target.IsType<UTargetComponent*>())
      return targetData->target.Get<AUnit*>();
   else if(targetData->target.IsType<AActor*>())
      return targetData->target.Get<AActor*>();
   else
      return nullptr;
}

bool UTargetComponent::IsTargettingSelf() const
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

