// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "AllyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "WorldObjects/Ally.h"
#include "WorldObjects/Unit.h"

AAllyAIController::AAllyAIController()
{
   SetActorTickInterval(0.2f);
   behaviorTrees.SetNum(NUM_BEHAVIORAL_MODES);
}

void AAllyAIController::Tick(float deltaTime)
{
   Super::Tick(deltaTime);
}

void AAllyAIController::Possess(APawn* InPawn)
{
   Super::Possess(InPawn);
   allyRef             = Cast<AAlly>(GetPawn());
   currentAllyBehavior = AllyBehavioralMode::ABM_Neutral;
}

void AAllyAIController::OnPerceptionUpdated(TArray<AActor*> updatedActors)
{
}

void AAllyAIController::SwitchAIModes(AllyBehavioralMode newMode)
{
   if (currentAllyBehavior != AllyBehavioralMode::ABM_Neutral) behaviorTreeComp->StopTree();

   // if we choose neutral there's no behavior tree since it's element in the tree array is empty
   if (behaviorTrees[static_cast<uint8>(newMode)]) {
      UseBlackboard(behaviorTrees[static_cast<uint8>(newMode)]->BlackboardAsset, blackboardComp);
      behaviorTreeComp->RestartTree(); // restart tree or we could be stuck on tasks that can't complete if they are stopped in progress
      behaviorTreeComp->StartTree(*behaviorTrees[static_cast<uint8>(newMode)]);
   }

   currentAllyBehavior = newMode;
   aggressionLevel     = static_cast<uint8>(newMode);
}
