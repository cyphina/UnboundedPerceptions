// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "EnemyAIController.generated.h"

/**
 * Base controller for enemies.  Enemies have sight which they use to react to things, but once they see an enemy (which is a hero to them),
 * they can react as if they know where the whole team is (we don't have to keep track of visible allies hence).
 */

class AUnit;

UCLASS()
class MYPROJECT_API AEnemyAIController : public AUnitController
{
	GENERATED_BODY()
	
private:

	/**function that fires when the perception of our ai gets updated*/
	UFUNCTION()
	void								OnPerceptionUpdated(const TArray<AActor*>& updatedActors);

	/**A sight sense config for our AI*/
	UAISenseConfig_Sight*				sight;

	/**Currently percieved actors*/
	FActorPerceptionBlueprintInfo		info;

	/**current closest distance to some actor in range*/
	int									currentClosest = TNumericLimits<int32>::Max(); 

	/**Sight perception information*/
	const FActorPerceptionInfo*			sightPerceptionInfo;

	TArray<AActor*>						percievedActors;
	/**Stored closest target to us during perception changes*/
	AUnit*								percievedTargetUnit;

protected:

	/**Perception Component of our AI*/
	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent*				AIPerceptionComponent;

public: 
	/**Behavior tree contains logic of our AI*/
	UPROPERTY(EditAnywhere)
	UBehaviorTree*						behaviorTree;

	AEnemyAIController();
	void								Possess(APawn* InPawn) override;

	/** Returns the seeing pawn.  Returns null if our AI has no unitTarget. */
	AActor* GetSeeingPawn();
};
