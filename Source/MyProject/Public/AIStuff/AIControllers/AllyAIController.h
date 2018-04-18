// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AllyAIController.generated.h"

/**
 * AI Controller of Ally.
 * Doesn't need perception because enemies viewed are shared
 */

UENUM(BlueprintType)
enum class AllyBehavioralMode : uint8
{
	ABM_Passive, //Heal and buff allies but run away of enemies are nearby
	ABM_Defensive, //Avoid enemies, focus on healing and attack if the enemy isn't attacking back
	ABM_Neutral, //Player controlled behavior
	ABM_Offensive, //Attack closest enemy 
 	ABM_Aggressive //Attack lowest hp enemy
};

UENUM(BlueprintType)
enum class AllyGroupTacticsMode : uint8
{
	AM_Split, //Split up units
	AM_Surround, //Surround a target
	AM_TacGroup, //Stay in formation
	AM_Lure, //Lure a unit
	AM_Escape, //Escape safely 
	AM_Run, //Escape hastily
	AM_Protect, //Protect a target
	AM_Tank, //Tanking procedures
	AM_Explore, //Explore the surrounding
	AM_None //No tactics enabled
};

class UBehaviorTreeComponent;
class AUserInput;
class AAlly;

UCLASS()
class MYPROJECT_API AAllyAIController : public AUnitController
{
	GENERATED_BODY()

	/**Behavior tree contains logic of our AI.  Will swap depending on tactic and behavioral mode*/
	UPROPERTY(EditAnywhere)
	TArray<UBehaviorTree*>			behaviorTrees;

	/**blackboard key value name*/
	const FName						blackboardEnemyKey = FName("Target");

	/*Current Behavior mode*/
	UPROPERTY(BlueprintReadOnly, Category = "AI Mode", meta = (AllowPrivateAccess="true"))
	AllyBehavioralMode				currentAllyBehavior;

	/*If this unit was part of a group tactic assignment, then set its tactic mode here*/
	AllyGroupTacticsMode			tacticsBehavior;
	AUserInput*						controllerRef;
	AAlly*							allyRef;

	UFUNCTION()
	void							OnPerceptionUpdated(TArray<AActor*> UpdatedActors);

public:

	const int						NUM_BEHAVIORAL_MODES = 5;

	AAllyAIController();

	virtual void					Tick(float deltaTime) override; 
	virtual void					Possess(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable, Category = "AI Mode")
	void							SwitchAIModes(AllyBehavioralMode newMode);

};
