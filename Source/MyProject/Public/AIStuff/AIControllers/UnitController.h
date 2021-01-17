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
struct FUpDamage;

using FAIMessageObserverHandle = TSharedPtr<struct FAIMessageObserver, ESPMode::Fast>;

DECLARE_EVENT(AUnitController, FOnUnitStopped);

/**
 * Base controller for all unit classes.  Controller manages a behavior flow, and allows different units
 * to swap out how they behave by storing various trees.  Holds all of the task functions.
 */
UCLASS()
class MYPROJECT_API AUnitController : public AAIController
{
   GENERATED_BODY()

 public:
   AUnitController();

   UFUNCTION(BlueprintCallable, BlueprintPure)
   FORCEINLINE AUnit* GetUnitOwner() const { return ownerRef; }

   /**
    * @brief Used when the player moves units to a certain location (only on right click move)
    * @param newLocation Location we want to move the selected units to.
    * @return Returns a enum that denotes if the move was successful or why it failed.
    */
   UFUNCTION(BlueprintCallable, Category = "Action")
   virtual EPathFollowingRequestResult::Type Move(FVector newLocation);

   /** Similar to Move function but moves towards a target  actor */
   UFUNCTION(BlueprintCallable, Category = "Action")
   virtual EPathFollowingRequestResult::Type MoveActor(AActor* targetActor);

   /** Function to turn self towards a direction*/
   UFUNCTION(BlueprintCallable, Category = "Movement")
   void TurnTowardsPoint(FVector targetLocation);

   /** Function to turn self towards an actor*/
   UFUNCTION(BlueprintCallable, Category = "Movement")
   void TurnTowardsActor(AActor* targetActor);

   /**
    * Called whenever we initiate a new action to cancel whatever was currently happening.
    * Stop should be overrode based on the subclass because stopping some classes has to cancel more things
    */
   UFUNCTION(BlueprintCallable, Category = "Action")
   virtual void Stop();

   /** Stop behavior tree from running. Used in the case we want to stop tasks that are performed via the behavior tree (like attack move) and override them with our predefined actions
    * like when we issue a command via a click
    */
   UFUNCTION(BlueprintCallable, Category = "Action")
   void StopAutomation() const;

   UFUNCTION(BlueprintCallable, Category = "Action")
   void Die();

   /** Calls execute within the functor object and handles some logic common to all attacking methods */
   UFUNCTION(BlueprintCallable, Category = "Action")
   void Attack();

   /**
    * Function to move to appropriate distance from target and face direction*
    * This variant is used when initiating some kind of BeginAction() operation we have to move to a closer location before we can start our action
    * @param range - Distance away from target we can be to stop moving closer
    * @param targetLocation - Actual location of target point we're attempting to move closer to
    * @param finishedTurnAction - What should we tell this unit to do after we finished moving and turn towards our target?
    */
   bool AdjustPosition(float range, FVector targetLocation, TFunction<void()> finishedTurnAction);

   bool AdjustPosition(float range, AActor* targetActor, TFunction<void()> finishedTurnAction);

   FOnUnitStopped& OnUnitStopped() { return OnUnitStoppedEvent; }

   static const int CHASE_RANGE = 100;

 protected:
   void BeginPlay() override;
   void Tick(float deltaSeconds) override final;

   /**Make sure to call UseBlackboard/InitializeBlackboard in the children classes depending
    * on how they use their blackboards as well as starting the tree*/
   void OnPossess(APawn* InPawn) override;

   /** Either Idle, Follow, Patrol, Search, or Roam*/
   UPROPERTY(EditDefaultsOnly)
   UBehaviorTree* idleMoveLogic;

 private:
   UFUNCTION()
   void OnActorTurnFinished();

   UFUNCTION()
   void OnPointTurnFinished();

   UFUNCTION()
   void Turn(float turnValue);

   UFUNCTION()
   void TurnActor(float turnValue);

   void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

   void OnDamageReceived(const FUpDamage& d);

   /** This timeline is for when we have to turn towards a point (FVector)*/
   void SetupTurnPointTimeline();

   /** This timeline is for when we have to turn towards an actor (AActor*). It may take several attempts to reach the actor's location
    * because the actor may be moving around.
    */
   void SetupTurnActorTimeline();

   /**
    * Function to move to appropriate distance from target and face direction
    * It does two checks. First it checks to see if we're out of range and then moves the unit to the position.
    * Else if the unit is not facing the target it rotates the unit towards the target.
    * If the unit needs both adjusted, then the unit will turn towards the target after the move finishes as setup by a callback that triggers after a move.
    */
   bool AdjustPosition(const float range, FVector targetLocation);

   /**
    * Function to move to appropriate distance from target and face the target
    */
   bool AdjustPosition(const float range, AActor* targetActor);

   /**
    * @brief Used in adjust position to setup turning after we finished moving, else we trigger the finish move action.
    * @param targetLoc Location to turn to.
    */
   void QueueTurnAfterMovement(FVector targetLoc);

   /**
    * @brief Used in adjust position to setup turning after we finished moving, else we trigger the finish move action.
    * @param targetActor Actor to turn to.
    */
   void QueueTurnAfterMovement(AActor* targetActor);

   UPROPERTY(EditAnywhere, Category = "Movement", Meta = (AllowPrivateAccess = true))
   float rotationRate = 0.03f;

   UPROPERTY()
   AUnit* ownerRef = nullptr;

   UPROPERTY()
   TSet<AUnit*> groupRef; // Denotes is unit is part of a group (group AI)

   /** Turn action that is set after movement because we either turn towards a point or an actor (which is set by the corresponding AdjustPosition functino).
    *  If the unit is already already facing its target, executes onPosAdjDoneAct
    *  We use TFunctions over corresponding delegates because this functionality is only supposed
    *  to be set through very specific interfaces.
    */
   TFunction<void()> queuedTurnAction;

   /** Action to be done after positional adjustment is finished. This means we've moved into position and turned towards the target
    *  We use TFunctions over corresponding delegates because this functionality is only supposed
    *  to be set through very specific interfaces.
    */
   TFunction<void()> onPosAdjDoneAct;

   UCurveFloat* turnCurve;
   FQuat        turnRotator;
   FQuat        startRotation;

   FTimeline turnTimeline;
   FTimeline turnActorTimeline;

   FOnUnitStopped OnUnitStoppedEvent;

   /**
    * @brief Listens for a specific AI message to manipulate AI
    */
   FAIMessageObserverHandle protectListener;

   class MoveCompletedVisitor
   {
    public:
      MoveCompletedVisitor(AUnitController* controllerRef) : finishedMovingUnitController(controllerRef) {}
      void operator()(FEmptyVariantState);
      void operator()(FVector targetVector);
      void operator()(AActor* targetActor);
      void operator()(AUnit* targetUnit);

    private:
      inline AUnit&    FinishedMovingUnit() const;
      AUnitController* finishedMovingUnitController;
   };
};
