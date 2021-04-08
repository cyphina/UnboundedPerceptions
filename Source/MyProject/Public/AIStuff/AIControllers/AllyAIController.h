// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnitController.h"
#include "ManualTargetingControl.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "UnitTargetData.h"

#include "AllyAIController.generated.h"

class UManualSpellComponent;
class USpellCastComponent;
class UTargetComponent;
struct FGameplayAbilityTargetDataHandle;

UENUM(BlueprintType)
enum class EAllyBehavioralMode : uint8
{
   ABM_Passive,   // Heal and buff allies but run away of enemies are nearby
   ABM_Defensive, // Avoid enemies, focus on healing and attack if the enemy isn't attacking back
   ABM_Neutral,   // Player controlled behavior (may change this to doing both controlled attack and defensive options)
   ABM_Offensive, // Attack closest enemy and try to use spells as efficiently as possible
   ABM_Aggressive // Attack lowest hp enemy and drop all spells on weak enemy
};

UENUM(BlueprintType)
enum class AllyGroupTacticsMode : uint8
{
   AM_Split,    // Split up units
   AM_Surround, // Surround a target
   AM_TacGroup, // Stay in formation
   AM_Lure,     // Lure a unit
   AM_Escape,   // Escape safely
   AM_Run,      // Escape hastily
   AM_Protect,  // Protect a target
   AM_Tank,     // Tanking procedures
   AM_Explore,  // Explore the surrounding
   AM_None      // No tactics enabled
};

class UPatrolComponent;
class UBehaviorTreeComponent;
class UTargetedAttackComponent;
class AUserInput;
class AAlly;

/**
 * @brief See AUnitController comments for more details about AIControllers. This class is an AIController with functionality specific to all allied units
 * TODO: Componentize the ally behaviors and group behaviors
 */
UCLASS()
class MYPROJECT_API AAllyAIController : public AUnitController
{
   GENERATED_BODY()

 public:
   AAllyAIController(const FObjectInitializer& ObjectInitializer);

   /** Players who are tired of microing can switch to one of the various AI modes which vary in their offensive and defensive play. */
   UFUNCTION(BlueprintCallable, Category = "AI Mode")
   void SwitchAIModes(EAllyBehavioralMode newMode);

   UFUNCTION(BlueprintCallable)
   UPatrolComponent* GetPatrolComponent() const { return patrolComp; }

   UFUNCTION(BlueprintCallable)
   class URTSStateComponent* GetStateComponent() const { return stateComp; }

   UTargetedAttackComponent* GetTargetedAttackComponent() const { return targetedAttackComponent; }

   USpellCastComponent* GetSpellCastComponent() const { return spellCastComponent; }

   UManualSpellComponent* GetManualSpellComponent() const { return manualSpellCastComponent; }

   EAllyBehavioralMode GetAllyBehaviorMode() const { return currentAllyBehavior; }

 protected:
   void OnPossess(APawn* InPawn) override;
   void OnUnPossess() override;

   UPROPERTY(VisibleAnywhere)
   UPatrolComponent* patrolComp;

   UPROPERTY(VisibleAnywhere)
   class URTSStateComponent* stateComp;

   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   UTargetedAttackComponent* targetedAttackComponent;

   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   USpellCastComponent* spellCastComponent;

   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   UManualSpellComponent* manualSpellCastComponent;

   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   UBlackboardComponent* blackboardComp;

   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
   UBehaviorTreeComponent* behaviorTreeComp;

 private:
   /** Behavior tree contains logic of our AI.  Will swap depending on tactic and behavioral mode */
   UPROPERTY(EditAnywhere)
   TArray<UBehaviorTree*> behaviorTrees;

   /** Used to change the AIs of our allied units (different modes range from aggressive to supporting styles) */
   UPROPERTY(BlueprintReadOnly, Category = "AI Mode", meta = (AllowPrivateAccess = "true"))
   EAllyBehavioralMode currentAllyBehavior;

   UPROPERTY()
   AAlly* allyRef;

   /** If this unit was part of a group tactic assignment, then set its tactic mode here */
   AllyGroupTacticsMode tacticsBehavior;

   static const FText INVALID_TARGET_TEXT;

   static const int NUM_BEHAVIORAL_MODES = 5;
};
