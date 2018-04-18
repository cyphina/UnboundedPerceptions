// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "UnitController.generated.h"

class AUnit;
class AUserInput;
class UEnvQuery;
class UMySpell;

/**
 * Base controller for all unit classes
 */

UCLASS()
class MYPROJECT_API AUnitController : public AAIController
{
	GENERATED_BODY()

	AUnit*								ownerRef = nullptr;
	AUserInput*							CPCRef = nullptr;

protected:

	/**blackboard key value name*/
	const FName							blackboardEnemyKey = FName("Target");

public:

	AUnitController();

	/** Location which our controller will patrol */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISettings")
	TArray<FVector>						patrolLocations;

	/**Behaviortreecomponent used to start a behavior tree*/
	UBehaviorTreeComponent*				behaviorTreeComp;

	/**blackboardcomponent used to initialize blackboard values and set/get values from a blackboard asset*/
	UBlackboardComponent*				blackboardComp;

	/**Make sure to call UseBlackboard/InitializeBlackboard in the children classes depending on how they use their blackboards as well as starting the tree*/
	void								Possess(APawn* InPawn) override;

	inline AUnit*						GetUnitOwner() const { return ownerRef; } 
	inline AUserInput*					GetCPCRef() const { return CPCRef; }

	void								FindBestAOELocation(int spellIndex);
	void								FindWeakestTarget(int spellIndex);

private:

	/**Spell to cast after target is found*/
	int									spellToCastIndex;

	/**Environment query to find the best place to cast an AOE spell for maximum target hits*/
	UPROPERTY(EditDefaultsOnly)
	UEnvQuery*							findBestAOELocation;

	/**Environment query to find the weakest allied unit.  Allies and enemies have should have different EQueries set*/
	UPROPERTY(EditDefaultsOnly)
	UEnvQuery*							findWeakestTarget;

	void								OnAOELocationFound(TSharedPtr<FEnvQueryResult> result);
	void								OnWeakestTargetFound(TSharedPtr<FEnvQueryResult> result);
};	
