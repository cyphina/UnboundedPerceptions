#pragma once
#include "IUnitState.h"

/**
 * Represents state when unit is attacking (with auto attacks)
 * Some notable details about this state:
 * Initiated when we call BeginAttack() in the unit controller
 * -- Certain AI tasks can circumvent the standard attack process. Some units don't use a targeted attack
 * -- as handled by AttackState but just use a technique similar to an ability but the technique scales based off the attack speed.
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
 * -- -- b. If the unit runs out of the buffer range during the attack animation the animation will be canceled.
 * -- -- -- i. If this occurs, we have to chase the enemy unit if it is still in vision.
 * -- -- -- ii. If it leaves vision, then we change states into chasing state which involves different algorithms to search for a unit in the FOW
 * -- 4. Once the attack notification occurs, the timer resets back to 0 (meaning we can start up a new action (even spell casting)
 * -- -- during the animation back swing (known as animation cancelling, but AIs don't leverage this)
 * -- 5. The timer fills up in between attacks (as soon as the damage notify plays) 
 */
class MYPROJECT_API AttackState : public IUnitState
{
 public:
   AttackState();
   ~AttackState();
   virtual void Enter() override;
   virtual void Exit() override;
   virtual void Update(, float deltaSeconds) override;

   virtual EUnitState GetName() const override { return EUnitState::STATE_ATTACKING; }

   friend class AttackMoveState; // Friendship with attack move state since it also uses similar attacking logic

 private:
   AUnit&           agent;                   // Unit whose behavior is specified through the behavioral logic within this state
   static const int shortAttRngBuff = 25.f;  // Distance that an attack in progress will cancel since it is out of range
   static const int attRngCnclBuff  = 350.f; // Distance that an attack in progress will cancel since it is out of range
   float            currentAttTime  = 0.f;   // Keeps track of how long we wait between autoattacks

   /** Game time representing what time we leave this state so that if we start it again, we can figure out if enough time has
    * elapsed for us to attack again without having to move the update logic outside AttackState */
   float stopTime = 0.f;

   /** True if our target is in attack range + cancel buffer and we're in attack state */
   bool bTgtInAttBuff = false;

   /** True if this unit is adjusting its position to be in range of the target which has moved out of range while we are attacking
    * Used to short circuit the update logic (since we don't need to check for anything besides vision if we're repositioning while attacking) */
   bool bMvingTowdTarg = false;

   bool attackAnimationPlaying = false;

   void        OnHitEvent();
   inline void ResetAttTimer();
   inline bool AttemptReposition();
   /** Called if the repositioning is sucessful (requires us to be in range and called only once turn suceeds or we're already facing target) */
   void        OnFinishReposition();
   inline void UpdateAttackTimer(float deltaSeconds);
   /** If any of the cancel conditions become true, the unit will transition to another state */
   bool        CheckAndHandleCancelConditions() const;
   inline bool CheckTargetVisionLost() const;
   inline bool CheckTargetAttackable() const;
   inline void TransitionToChaseState() const;

   inline void  StopAgent() const;
   bool         AttackInitiationSetup();
   void         PlayAttackAnimation();
   bool         AttackSpeedTimerCheck() const;
   inline float AttackTimerThreshold() const;
   /** When initiating an attack, a target needs to face the enemy before it can even reach this state. However, once the unit reaches this state
    * and is in range of the enemy, it will keep locked onto the enemy (keep facing the enemy without having any turn animation).
    */
   void LockOnTarget() const;
   bool CheckAndHandleTargetOutsideAnimationRange();

   inline float   AgentAttackRange() const;
   inline float   AgentAttackSpeed() const;
   inline float   AgentBufferAttackRange() const;
   inline FVector AgentTargetLocation() const;
   inline AUnit*  AgentTargetUnit() const;
};
