// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "PatrolComponent.h"
#include "AIController.h"

UPatrolComponent::UPatrolComponent()
{
   PrimaryComponentTick.bCanEverTick = false;
}

void UPatrolComponent::BeginPlay()
{
   Super::BeginPlay();
   APawn* pawnOwner = Cast<APawn>(GetOwner());
   check(pawnOwner);

   ownerControllerRef = Cast<AAIController>(pawnOwner->GetController());
   ownerControllerRef->ReceiveMoveCompleted.AddDynamic(this, &UPatrolComponent::OnMoveCompleted);

   if (patrolPoints.Num() > 0) ownerControllerRef->MoveToLocation(patrolPoints[currentPatrolIndex]);
}

void UPatrolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
   Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
   // ...
}

void UPatrolComponent::DeletePatrolPoint(int patrolIndex)
{
   if (patrolIndex >= 0 && patrolIndex <= patrolPoints.Num()) { patrolPoints.RemoveAt(patrolIndex, 1); }
}

void UPatrolComponent::MoveToNextPatrolPoint()
{
   currentPatrolIndex = (currentPatrolIndex + 1) % patrolPoints.Num();
   ownerControllerRef->MoveToLocation(patrolPoints[currentPatrolIndex]);
}

void UPatrolComponent::OnMoveCompleted(FAIRequestID requestID, EPathFollowingResult::Type result)
{
   switch (result) {
      case EPathFollowingResult::Success: MoveToNextPatrolPoint(); break;
      case EPathFollowingResult::Aborted: break;
      case EPathFollowingResult::Blocked: ownerControllerRef->MoveToLocation(patrolPoints[currentPatrolIndex]); break;
   }
}
