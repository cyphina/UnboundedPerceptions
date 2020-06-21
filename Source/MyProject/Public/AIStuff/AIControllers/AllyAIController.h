// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UnitController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "UnitTargetData.h"

#include "AllyAIController.generated.h"

struct FGameplayAbilityTargetDataHandle;

/**
 * AI Controller of Ally.
 * Doesn't need perception because enemies viewed are shared
 */

UENUM(BlueprintType)
enum class AllyBehavioralMode : uint8 {
   ABM_Passive,   // Heal and buff allies but run away of enemies are nearby
   ABM_Defensive, // Avoid enemies, focus on healing and attack if the enemy isn't attacking back
   ABM_Neutral,   // Player controlled behavior (may change this to doing both controlled attack and defensive options)
   ABM_Offensive, // Attack closest enemy and try to use spells as efficiently as possible
   ABM_Aggressive // Attack lowest hp enemy and drop all spells on weak enemy
};

UENUM(BlueprintType)
enum class AllyGroupTacticsMode : uint8 {
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

class UBehaviorTreeComponent;
class AUserInput;
class AAlly;

UCLASS()
class MYPROJECT_API AAllyAIController : public AUnitController
{
   GENERATED_BODY()

   /** Behavior tree contains logic of our AI.  Will swap depending on tactic and behavioral mode */
   UPROPERTY(EditAnywhere)
   TArray<UBehaviorTree*> behaviorTrees;

   /** Blackboard key value name */
   const FName blackboardEnemyKey = FName("Target");

   /** Current Behavior mode */
   UPROPERTY(BlueprintReadOnly, Category = "AI Mode", meta = (AllowPrivateAccess = "true"))
   AllyBehavioralMode currentAllyBehavior;

   /** If this unit was part of a group tactic assignment, then set its tactic mode here */
   AllyGroupTacticsMode tacticsBehavior;

   UPROPERTY()
   AAlly* allyRef;

   /** Unlike the curent spell, this is the one selected by the player, but it may not be the one being channeled */
   TSubclassOf<UMySpell> currentlySelectedSpell;

   /** Index of the spell that is selected by the player (stored after the player presses the key down). It is then saved in ally once the player clicks to confirm
    * the spell target so that this can be modified (if the player wants to queue up another spell while the current is being casted or something like that */
   int currentlySelectedSpellIndex;

   UFUNCTION()
   void OnPerceptionUpdated(TArray<AActor*> UpdatedActors);

 public:
   const int NUM_BEHAVIORAL_MODES = 5;

   AAllyAIController();

   virtual void OnPossess(APawn* InPawn) override;

   /** Gets spell that has been selected, but may not be channeled*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spells")
   FORCEINLINE TSubclassOf<UMySpell> GetCurrentlySelectedSpell() const { return currentlySelectedSpell; }

   /** Change how an ally should behave by either clicking on the actionbar or group command bar*/
   UFUNCTION(BlueprintCallable, Category = "AI Mode")
   void SwitchAIModes(AllyBehavioralMode newMode);

   /** When spell hotkey is presssed down with this ally selected.  Returns true when spell sucessfully set up or casted.  Used for item usage.*/
   bool PressedCastSpell(TSubclassOf<UMySpell> spellToCast);

   /** When spell hotkey is presssed down with this ally selected.  Returns true when spell sucessfully set up or casted.  Exposed to blueprints for actionbar usage.*/
   UFUNCTION(BlueprintCallable, Category = "Spells")
   bool PressedCastSpell(int spellToCastIndex);

   void AreaManualTargetting(FHitResult& result, TSubclassOf<UMySpell> spellClass, UMySpell* spell);

   bool InteractableManualTargetting(FHitResult& result, TSubclassOf<UMySpell> spellClass, FName spellTargetting);

   /** Always returns false since we targetted something we were not supposed with this spell*/
   bool InvalidTarget() const;

   void CastSpellOnSelf(TSubclassOf<UMySpell> spellClass);

   /** Used to check the bIsEnemy relationship matches for targets */
   bool CheckUnitTarget(FName spellTargetting, AUnit* unit) const;

   bool UnitManualTargetting(FHitResult& result, TSubclassOf<UMySpell> spellClass, FName spellTargetting);

   /** Called right after we press our spell key and then click on a target to confirm our spell
    * From a raycast (left click) we can test to see if the target clicked on is a proper target for our spell
    * @param result - Result of the target we found with a click
    * @param spellClass - Pass in class because we can technically setup targetting for a new spell while casting a spell (and thus can't use currentSpell)
    */
   UFUNCTION(BlueprintCallable, Category = "Spells")
   bool SetupSpellTargetting(UPARAM(ref) FHitResult& result, TSubclassOf<UMySpell> spellClass);

   /** Helper to setup currentSpell and spellIndex used when we have to save these properties so that the spell can be casted after the ally moves to the target*/
   FORCEINLINE void SetupDelayedSpellProps(TSubclassOf<UMySpell> spellToCast) const;

 private:
   void FinalizeSpellTargetting(TSubclassOf<UMySpell> spellClass, AActor* targetActor);
   void FinalizeSpellTargetting(TSubclassOf<UMySpell> spellClass, AUnit* targetUnit);
   void FinalizeSpellTargetting(TSubclassOf<UMySpell> spellClass, FVector targetLocation);

 public:
   virtual void Stop() override;

   void BeginAttack(AUnit* target) override;
   bool BeginCastSpell(TSubclassOf<UMySpell> spellToCastIndex) override;
   void DeselectSpell();
};
