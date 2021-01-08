// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "EnemyAIController.h"

#include "WorldObjects/Unit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AEnemyAIController::AEnemyAIController()
{
}

void AEnemyAIController::BeginPlay()
{
   Super::BeginPlay();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
   Super::OnPossess(InPawn);
}

