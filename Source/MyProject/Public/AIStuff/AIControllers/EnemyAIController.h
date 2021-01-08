// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnitController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "EnemyAIController.generated.h"

class AUnit;

/**
 * Base controller for enemies.  Enemies have sight which they use to react to things, but once they see an enemy (which is a hero to them),
 * they can react as if they know where the whole team is (we don't have to keep track of visible allies hence).
 */
UCLASS()
class MYPROJECT_API AEnemyAIController : public AUnitController
{
   GENERATED_BODY()

 public:
 protected:
   AEnemyAIController();
   void BeginPlay() override;
   void OnPossess(APawn* InPawn) override;

 private:
};
