// Created 12/26/20 5:59 PM

#pragma once
#include "GameplayEffectTypes.h"
#include "Components/ActorComponent.h"
#include "TargetedAttackComponent.generated.h"

struct FDamageScalarStruct;
class URTSProjectileStrategy;
class UMySpell;
class IGameSpeedContext;
class IAttackAnim;
class AUnit;

/**
 * Initiated when we call BeginAttack()
 * -- Certain AI tasks can circumvent the standard attack process. Some units don't use a targeted attack
 * -- as handled by this component but just use a technique similar to an ability.
 * Before entering this state (handled in BeginAttack()), the setup involves:
 * -- 1. Walking into range
 * -- 2. Turning towards the target
 * The behavior of the state is as so (also see the game design doc for more information):
 * -- 1. Check for preconditions:
 * -- -- a. Check to see if the target is still targetable (things like phase shift or ethereal form will disable attacks)
 * -- -- b. Check to see if the target is still within (range + short buffer range) before initiating the attack animation (the enemy may have run out between attacks).
 * -- -- -- i. The short buffer range is particularly important for moving targets because we have a short delay before the attack state begins and the turn time where attacking
 * -- -- -- -- a moving target is difficult because the target will escape our attack range by the time we've adjusted our turn.
 * -- -- -- ii. If the target not in range, move to spot in range. While moving, pause attack updates and resume attack using an adjust position callback
 * -- 2. Once the unit is in range, it will lock onto its target causing it to always face the target. The only time turning matters is on the attack initiation
 * -- -- a. Note that targeting a new unit starts up a new initiation which calls Stop() meaning we return to idle state, changes targets, then returns to this state with our new target
 * -- 3. Initiate an attack if the attack timer is already filled (if not then wait)
 * -- -- a. Initiation of the attack may involve an animation in which the damage is only applied on the animation's damage notify event (which may delay the actual damage)
 * -- -- b. Look up https://dota2.gamepedia.com/Attack_animation. Basically the hit event is at the attack point and the rest is backswing.
 * -- -- c. If the unit runs out of the buffer range during the attack animation the animation will be canceled.
 * -- -- -- i. If this occurs, we have to chase the enemy unit if it is still in vision.
 * -- -- -- ii. If it leaves vision, then we change states into chasing state which involves different algorithms to search for a unit in the FOW
 * -- 4. Once the attack notification occurs, the timer resets back to 0 (meaning we can start up a new action (even spell casting)
 * -- -- during the animation back swing (known as animation cancelling, but AIs don't leverage this)
 * -- 5. The timer fills up in between attacks (as soon as the damage notify plays) 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UTargetedAttackComponent : public UActorComponent
{
   GENERATED_BODY()

 public:
   UTargetedAttackComponent();

   /**
    * @brief Initiates a targeted attack command which will cause a unit to keep auto-attacking a target until it dies or escapes or the attacking unit gets stopped.
    */
   UFUNCTION(BlueprintCallable, Category = "Action")
   virtual void BeginAttack(AUnit* target);

   /**
    * @brief Moves a unit to a location and attacks any enemies that is in range along the way
    * @param targetLocation - Location to move to
    */
   UFUNCTION(BlueprintCallable, Category = "Action")
   void BeginAttackMove(FVector targetLocation);

   /**
   * @brief Allows us to replace the next attack with a spell (allows for effects like AA's chilling touch or Viper Strike). \n
   * Can have charges also like flak cannon.
   * TODO: Implement this
   */
   UFUNCTION(BlueprintCallable)
   virtual void OverrideAttackWithSpell(TSubclassOf<UMySpell> overridingSpell);

   bool GetIsReadyToAttack() const { return bReadyToAttack; }
   
 protected:
   void BeginPlay() override;
   void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

   UPROPERTY(EditDefaultsOnly, meta = (MustImplement = "AttackAnim"))
   TSubclassOf<UObject> attackAnimStrategy = nullptr;

   UPROPERTY()
   UObject* animAttackStrategyInst;

   UPROPERTY(EditDefaultsOnly)
   TSubclassOf<URTSProjectileStrategy> projectileStrategyClass = nullptr;

   TScriptInterface<IAttackAnim> attackAnim;

 private:
   void OnUnitStopped();

   /**
    * When initiating an attack, a target needs to face the enemy before it can even reach this state. However, once the unit reaches this state
    * and is in range of the enemy, it will keep locked onto the enemy (keep facing the enemy without having any turn animation).
    */
   void LockOnTarget() const;

   /**
    * @brief Logic called after the "necessary part" of the attack animation finishes (meaning the player can animation cancel the remaining and still pull off the damage)
    */
   void OnUnitAttackSwingDone();

   /**
    * @brief Applies damage or generates a projectile when the attack animation finishes
    */
   void OnAttackSwingDoneEffect();

   /**
    * @brief TODO: System for special effects whenever we auto attack (maybe like passive attack modifiers, ability chance triggers, etc.)
    */
   void HandleAutoAttackModifierTags();

   FGameplayTag GetAttackElement() const;

   /**
    * @brief Logic to generate a projectile after the animation for the attack finishes (like an arrow after a bowstring draw)
    * @param projectileDamageScalars Values which determine how much damage this projectile will inflict
    */
   void CreateRangedProjectile(FDamageScalarStruct projectileDamageScalars);

   bool CheckAndHandleTargetOutsideAnimationRange();

   void FollowAndTryAttackTarget();

   void SearchForTargetInRange();

   bool AttemptReposition();

   /** Called if the repositioning is successful (requires us to be in range and called only once turn succeeds or we're already facing target) */
   void OnFinishReposition();

   /** If any of the cancel conditions become true, the unit will transition to another state */
   bool CheckAndHandleCancelConditions();

   bool CheckTargetVisionLost() const;

   bool CheckTargetAttackable() const;

   void TransitionToChaseState();

   void StopAgent();

   void StopAttackAnim();

   /**
    * @brief Checks to see if we need to reposition to attack and if so, repositions us.
    * @return Returns true if we need to adjust the owner's positoin, or false otherwise
    */
   bool HandlePositionalAdjustments();

   void PlayAttackAnimation();

   float AttackTimerThreshold() const;

   inline float   AgentAttackRange() const;
   inline float   AgentAttackSpeed() const;
   inline float   AgentBufferAttackRange() const;
   inline FVector AgentTargetLocation() const;
   inline AUnit*  AgentTargetUnit() const;

   /**
    *True if this unit is adjusting its position to be in range of the target which has moved out of range while we are attacking
    * Used to short circuit the update logic (since we don't need to check for anything besides vision if we're repositioning while attacking)
    */
   bool bMvingTwdTarg = false;

   bool bAttackAnimationPlaying = false;

   bool bReadyToAttack = true;

   FVector attackMoveLocation;

   AUnit*           agent;                   // Unit whose behavior is specified through the behavioral logic within this state
   static const int shortAttRngBuff = 25.f;  // Distance that an attack in progress will cancel since it is out of range
   static const int attRngCnclBuff  = 350.f; // Distance that an attack in progress will cancel since it is out of range

   // Looping search for units around us when performing an attack move.
   FTimerHandle targetSearchHandle;

   // While we are attacking a unit, we have to keep facing them and checking if they are valid
   FTimerHandle attackChecksHandle;

   // This is time between attacks (recharges even if we're not auto attacking)
   FTimerHandle attackUpdateHandle;
};
