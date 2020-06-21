// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "UnitTargetData.h"
#include "UnitController.generated.h"

struct FAIMessage;
enum class EUnitState : uint8;
class AUnit;
class AUserInput;
class UEnvQuery;
class UMySpell;

/**
 * Base controller for all unit classes.  Controller manages a behavior, and allows different units
 * to swap out how they behave.  Holds all of the task functions.
 */

UCLASS()
class MYPROJECT_API AUnitController : public AAIController
{
   GENERATED_BODY()

   UPROPERTY()
   AUnit* ownerRef = nullptr;
   UPROPERTY()
   AUserInput* CPCRef = nullptr;
   UPROPERTY()
   TSet<AUnit*> groupRef;         // Denotes is unit is part of a group (group AI)
   int          spellToCastIndex; // Spell to cast after target is found using EQS

 protected:
   /**blackboard key value name*/
   const FName blackboardEnemyKey   = FName("target");
   const FName blackboardRiskKey    = FName("risk");
   const FName blackboardProtectKey = FName("protect");
   const FName blackboardThreatKey  = FName("threat");

   static const FName             AIMessage_Help; // sent when a unit needs some help defensively
   void CastTurnAction();

   AUserInput* GetCPCRef() const { return CPCRef; }

 public:
   static const int CHASE_RANGE = 100;

   AUnitController();

   /** Location which our controller will patrol */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISettings")
   TArray<FVector> patrolLocations;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISettings")
   bool canProtect = false;

   /** Behaviortreecomponent used to start/stop behavior trees */
   class UBehaviorTreeComponent* behaviorTreeComp;

   /** Blackboardcomponent used to initialize blackboard values and
    * set/get values from a blackboard asset */
   UBlackboardComponent* blackboardComp;

   /**Make sure to call UseBlackboard/InitializeBlackboard in the children classes depending
    * on how they use their blackboards as well as starting the tree*/
   void OnPossess(APawn* InPawn) override;
   void BeginPlay() override;
   void Tick(float deltaSeconds) override final;

   UFUNCTION(BlueprintCallable, BlueprintPure)
   FORCEINLINE AUnit* GetUnitOwner() const { return ownerRef; }

 private:
   typedef TSharedPtr<struct FAIMessageObserver, ESPMode::Fast> FAIMessageObserverHandle;
   FAIMessageObserverHandle                                     protectListener;

/** Used to search for an actor/vector that matches certain criteria */
#pragma region EnvironmentQuery

 public:
   /**Find the best spot for targetting an AOE spell
    *@param isSupport - Find best AOE location to hit the most enemies (false) or friends (true)? */
   virtual void FindBestAOELocation(bool isSupport);

   /**Find the best unit to target a single spell
    *@param isSupport - Find best AOE location to hit the most enemies (false) or friends (true)? */
   virtual void FindWeakestTarget(bool isSupport);

 private:
   /**Environment query to find the best place to cast an AOE spell for maximum target hits when healing*/
   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* findBestAOESupportLocation;

   /** Environment query to find the best place to cast an AOE spell for maximum target hits when casting an AOE spell*/
   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* findBestAOEAssaultLocation;

   /** Environment query to find the weakest allied unit.  Allies and enemies have should have different EQueries set*/
   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* findWeakestAllyTarget;

   /** Environment query to find the weakest enemy unit.  Allies and enemies have should have different EQueries set*/
   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* findWeakestEnemyTarget;

   /** Environment query to find the best place to escape to.  Allies and enemies have should have different EQueries set*/
   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* findBestDefensivePosition;

   /** Environment query to find the best place to deal damage.  Allies and enemies have should have different EQueries set*/
   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* findBestOffensivePosition;

   /** Chances of the unit casting a spell when at state 0 - Passive, 1 - Defensive, 2 - Offensive, 3 - Aggressive*/
   UPROPERTY(EditDefaultsOnly)
   TArray<int> spellCastingChances;

   void FindBestSpellTarget(FGameplayTag targettingTag, bool isSupport);
   void OnAOELocationFound(TSharedPtr<FEnvQueryResult> result);
   void OnWeakestTargetFound(TSharedPtr<FEnvQueryResult> result);

#pragma endregion;

/** Functions used for commands and AI tasks on the tree */
#pragma region actions

 public:
   bool SearchAndCastSpell(const FGameplayTagContainer& spellRequirementTags);

   virtual void Protect(UBrainComponent* BrainComp, const FAIMessage& Message);

   /** Function for moving units around, based upon the ACharacter move.  Changes our state, and some different characters may move differently*/
   UFUNCTION(BlueprintCallable, Category = "Action")
   virtual EPathFollowingRequestResult::Type Move(FVector newLocation);

   /** Similar to Move function but moves to an actor */
   UFUNCTION(BlueprintCallable, Category = "Action")
   virtual EPathFollowingRequestResult::Type MoveActor(AActor* targetActor);

   /** Follows a target denoted by the followTarget field in the Unit class*/
   virtual void Follow();

   /** Patrol around a set of points*/
   virtual void Patrol(TArray<FVector> newPatrolPoints);

   /** Move after a target even when it is out of vision by guessing where it could be*/
   virtual void Search();

   /** Randomly move about the map*/
   virtual void Roam();

   /** Get away from enemies*/
   virtual void Run();

   /** Used to chase after enemies that leave the vision radius*/
   virtual void Chase();

   /** Condition used to indicate chasing failure.  Chasing sucess is finding the enemy again*/
   virtual bool ChasingQuit();

   /** Stop should be overidden based on the subclass because stopping some classes has to cancel more things*/
   UFUNCTION(BlueprintCallable, Category = "Action")
   virtual void Stop();

   /** Stop behavior tree from running. Used in the case we want to stop tasks that are performed via the behavior tree (like attack move) and override them with our predefined actions
    * like when we issue a command via a click
    */
   void StopAutomation() const;

   /**Used to initiate an attack on a target*/
   UFUNCTION(BlueprintCallable, Category = "Action")
   virtual void BeginAttack(AUnit* target);

   /**Setup a move towards a position so we're in range to cast our spell (Requires currentSpell be set)*/
   void AdjustCastingPosition(TSubclassOf<UMySpell> spellClass, FVector spellTargetLocation);

   /**Setup a move towards an actor so we're in range to cast our spell (Requires currentSpell be set)*/
   void AdjustCastingPosition(TSubclassOf<UMySpell> spellClass, AActor* spellTargetActor);

   /**Initiate spell casting procedure without any input triggers.  Used for AI spellcasting.  Returns if we successfully transitioned to CASTING state*/
   UFUNCTION(BlueprintCallable, Category = "Action")
   virtual bool BeginCastSpell(TSubclassOf<UMySpell> spellToCastIndex);

   /** Move to a location searching for a target to attack or attack the unit closest to the location*/
   UFUNCTION(BlueprintCallable, Category = "Action")
   virtual void AttackMove(FVector moveLocation);

#pragma endregion

/** Functions used to perform checks within the tasks */
#pragma region ActionHelpers

   /**Function for checking if a unit is in range of some action*/
   UFUNCTION(BlueprintCallable, Category = "Positioning")
   bool IsTargetInRange(float range, FVector targetLocation);

   /**Function to see if unit is facing the direction*/
   UFUNCTION(BlueprintCallable, Category = "Positioning")
   bool IsFacingTarget(FVector targetLocation, float errorAngleCutoff = .02f);

 protected:
   /** Check to see if we're targetting ourself with a spell*/
   bool IsTargettingSelf(AUnit* unitRef) const { return unitRef == GetUnitOwner(); }

   /**Checks to see if spell has a cast time, and if so, it will start channeling (incantation) process.  Else it will just cast the spell*/
   virtual void IncantationCheck(TSubclassOf<UMySpell> spellToCast) const;

   /**If the spell has the Skill.Channeling tag then it requires us to channel after the incantation aka unit has to focus energy into the spell*/
   virtual void SpellChanneling(TSubclassOf<UMySpell> spellToCast);

   /** Initiates attack animation after moving into position */
   UFUNCTION()
   void PrepareAttack();

#pragma endregion

/** Not actions, but functions used to get the character in the right position */
#pragma region MoveAdjustments

 public:
   FQuat FindLookRotation(FVector targetPoint);

   /**Function to turn self towards a direction*/
   UFUNCTION(BlueprintCallable, Category = "Movement")
   void TurnTowardsTarget(FVector targetLocation);

   /**Function to turn self towards an actor*/
   UFUNCTION(BlueprintCallable, Category = "Movement")
   void TurnTowardsActor(AActor* targetActor);

   /**Function to move to appropriate distance from target and face direction*/
   bool AdjustPosition(float range, FVector targetLocation);

   /**Function to move to appropriate distance from target and face direction*/
   bool AdjustPosition(float range, AActor* targetActor);

   /**Function to move to appropriate distance from target and face direction*
   * This variant is used when intiating some kind of BeginAction() operation we have to move to a closer location before we can start our action
   * @param range - Distance away from target we can be to stop moving closer
   * @param targetLocation - Actual location of target point we're attempting to move closer to
   * @param newState - What state should we change this unit to be in when attempting to move
   * @param turnAction - What should we tell this unit to do after we finished moving and turn towards our target?
   */
   bool AdjustPosition(float range, FVector targetLocation, EUnitState newState, TFunction<void()> turnAction);

   bool AdjustPosition(float range, AActor* targetActor, EUnitState newState, TFunction<void()> turnAction);

   UFUNCTION()
   void OnActorTurnFinished();
    
   UFUNCTION()
   void ActionAfterTurning();

   UPROPERTY(EditAnywhere, Category = "Movement")
   float rotationRate = 0.03f;

 private:
   UCurveFloat* turnCurve;
   FQuat        turnRotator;
   FQuat        startRotation;

   FTimeline turnTimeline;
   FTimeline turnActorTimeline;

   UFUNCTION()
   void Turn(float turnValue);

   UFUNCTION()
   void TurnActor(float turnValue);

#pragma endregion
};
