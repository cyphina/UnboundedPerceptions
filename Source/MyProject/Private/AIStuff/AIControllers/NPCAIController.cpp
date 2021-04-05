// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "NPCAIController.h"

#include "PatrolComponent.h"
#include "AIModule/Classes/BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "WorldObjects/BaseHero.h"

const FName ANPCAIController::targetKeyName = "target";

ANPCAIController::ANPCAIController()
{
}

void ANPCAIController::OnPossess(APawn* inPawn)
{
   Super::OnPossess(inPawn);
}

void ANPCAIController::BeginPlay()
{
   Super::BeginPlay();
}

<<<<<<< HEAD
void ANPCAIController::Follow(const ABaseHero* heroToFollow)
{
   blackboardComp->SetValueAsObject(targetKeyName, const_cast<ABaseHero*>(heroToFollow));
   RunBehaviorTree(followTree);
=======
void ANPCAIController::Follow(const AActor* actorToFollow)
{
   followTarget = actorToFollow;
   GetWorld()->GetTimerManager().SetTimer(
       followLoopTimer,
       [this]() {
          const float pawnRadius = GetPawn()->GetSimpleCollisionRadius();
          MoveToLocation(followTarget->GetActorLocation() - (pawnRadius * followTarget->GetActorForwardVector()));
       },
       0.5f, true, 0.f);
>>>>>>> componentrefactor
}

void ANPCAIController::Patrol()
{
   if(UPatrolComponent* patrolComp = FindComponentByClass<UPatrolComponent>())
   {
      patrolComp->Patrol();
   }
}

void ANPCAIController::Stop()
{
   if(UPatrolComponent* patrolComp = FindComponentByClass<UPatrolComponent>())
   {
      patrolComp->StopPatrolling();
   }
   GetWorld()->GetTimerManager().ClearTimer(followLoopTimer);
}
