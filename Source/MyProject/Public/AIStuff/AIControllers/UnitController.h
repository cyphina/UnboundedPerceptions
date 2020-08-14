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
class URTSAttackExecution;
class URTSDeathExecution;
class URTSMoveExecution;

/**
 * Base controller for all unit classes.  Controller manages a behavior flow, and allows different units
 * to swap out how they behave by storing various trees.  Holds all of the task functions.
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

   static const FName AIMessage_Help; // sent when a unit needs some help defensively
   void               CastTurnAction();

   AUserInput* GetCPCRef() const { return CPCRef; }

 public:
   static const int CHASE_RANGE = 100;

   AUnitController();

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

   /** This timeline is for when we have to turn towards a point (FVector)*/
   void SetupTurnPointTimeline();
   /** This timeline is for when we have to turn towards an actor (AActor*). It may take several attempts to reach the actor's location
    * because the actor may be moving around.
    */
   void SetupTurnActorTimeline();

   void BeginPlay() override;
   void Tick(float deltaSeconds) override final;

   UFUNCTION(BlueprintCallable, BlueprintPure)
   FORCEINLINE AUnit* GetUnitOwner() const { return ownerRef; }

 private:
   typedef TSharedPtr<struct FAIMessageObserver, ESPMode::Fast> FAIMessageObserverHandle;
   FAIMessageObserverHandle                                     protectListener;


 private:
   /** Holds logic for basic auto attack */
   UPROPERTY(EditDefaultsOnly)
   TSubclassOf<URTSAttackExecution> customAttackLogic;

   /** Holds logic for death */
   UPROPERTY(EditDefaultsOnly)
   TSubclassOf<URTSDeathExecution> customDeathLogic;

   /** Holds custom move logic so that we add some logic for the AI to move in certain ways instead of just walking everywhere.
    * For instance, if we want our character to teleport towards its target we can cast the spell if its on CD and if the target is far or something. 
    */
   UPROPERTY(EditDefaultsOnly)
   TSubclassOf<URTSMoveExecution> customMoveLogic;

   void FindBestSpellTarget(FGameplayTag targettingTag, bool isSupport);

   UFUNCTION()
   void OnDamageReceived(const FUpDamage& d);

/** Functions used for commands and AI tasks on the tree */
#pragma region actions

 public:
   /**Function called when unit dies :(*/
   void Die();

   /** Calls execute within the functor object and handles some logic common to all attacking methods
    * Some common functionality implemented for attack is:
    *
    */
   void Attack();

   bool SearchAndCastSpell(const FGameplayTagContainer& spellRequirementTags);

   /** Function for moving units around, based upon the ACharacter move.  Changes our state, and some different characters may move differently*/
   UFUNCTION(BlueprintCallable, Category = "Action")
   virtual EPathFollowingRequestResult::Type Move(FVector newLocation);

   /** Similar to Move function but moves to an actor */
   UFUNCTION(BlueprintCallable, Category = "Action")
   virtual EPathFollowingRequestResult::Type MoveActor(AActor* targetActor);

   /** Either Idle, Follow, Patrol, Search, or Roam*/
   UPROPERTY(EditAnywhere)
   UBehaviorTree* idleMoveLogic;

   /** Stop should be overidden based on the subclass because stopping some classes has to cancel more things*/
   UFUNCTION(BlueprintCallable, Category = "Action")
   virtual void Stop();

   /** Stop behavior tree from running. Used in the case we want to stop tasks that are performed via the behavior tree (like attack move) and override them with our predefined actions
    * like when we issue a command via a click
    */
   void StopAutomation() const;

   /** Triggers setup required to enter attack loop logic, and them sets up callbacks to enter the loop once the setup is complete */
   UFUNCTION(BlueprintCallable, Category = "Action")
   virtual void BeginAttack(AUnit* target);

#pragma endregion

 protected:
   /** Initiates attack animation after moving into position */
   UFUNCTION()
   void PrepareAttack();

#pragma endregion

/** Not actions, but functions used to get the character in the right position */
#pragma region MoveAdjustments

 private:
   friend void USpellCastComponent::AdjustCastingPosition(TSubclassOf<UMySpell> spellClass, AActor* spellTargetActor);
   friend void USpellCastComponent::AdjustCastingPosition(TSubclassOf<UMySpell> spellClass, FVector spellTargetLocation);

   /**Function to turn self towards a direction*/
   UFUNCTION(BlueprintCallable, Category = "Movement")
   void TurnTowardsPoint(FVector targetLocation);

   /**Function to turn self towards an actor*/
   UFUNCTION(BlueprintCallable, Category = "Movement")
   void TurnTowardsActor(AActor* targetActor);

   /**Function to move to appropriate distance from target and face direction
    * It does two checks. First it checks to see if we're out of range and then moves the unit to the position.
    * Else if the unit is not facing the target it rotates the unit towards the target.
    * If the unit needs both adjusted, then the unit will turn towards the target after the move finishes as setup by a callback that triggers after a move.
    */
   bool AdjustPosition(float range, FVector targetLocation);

   inline static FVector GetLocation(AActor* targetActor);

   /**Function to move to appropriate distance from target and face the target*/
   bool AdjustPosition(float range, AActor* targetActor);

   /**Function to move to appropriate distance from target and face direction*
   * This variant is used when initiating some kind of BeginAction() operation we have to move to a closer location before we can start our action
   * @param range - Distance away from target we can be to stop moving closer
   * @param targetLocation - Actual location of target point we're attempting to move closer to
   * @param finishedTurnAction - What should we tell this unit to do after we finished moving and turn towards our target?
   */
   bool AdjustPosition(float range, FVector targetLocation, TFunction<void()>&& finishedTurnAction);

   bool AdjustPosition(float range, AActor* targetActor, TFunction<void()>&& finishedTurnAction);

   UFUNCTION()
   void OnActorTurnFinished();

   UFUNCTION()
   void OnPointTurnFinished();

   UPROPERTY(EditAnywhere, Category = "Movement")
   float rotationRate = 0.03f;

   /** Turn action that is set after movement because we either turn towards a point or an actor (which is set by the corresponding AdjustPosition functino).
    *  If the unit is already already facing its target, executes onPosAdjDoneAct
    *  We use TFunctions over corresponding delegates because this functionality is only supposed
    *  to be set through very specific interfaces.
    */
   TFunction<void()> QueuedTurnAction;

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

   UFUNCTION()
   void Turn(float turnValue);

   UFUNCTION()
   void TurnActor(float turnValue);

   void QueueTurnAfterMovement(FVector targetLoc);
   void QueueTurnAfterMovement(AActor* targetActor);

   /** Callback function for move completed. Delegates can work with virtual functions. */
   UFUNCTION()
   void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

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
#pragma endregion
};
