#pragma once
#include "IUnitState.h"

/**
 * Represents state when unit is attacking (with auto attacks)
 * Some notable details about this state:

 */
class MYPROJECT_API AttackState : public IUnitState
{
 public:
   AttackState();
   ~AttackState();

   virtual void Enter() override;
   virtual void Exit() override;
   virtual void Update(float deltaSeconds) override;

   virtual EUnitState GetName() const override { return EUnitState::STATE_ATTACKING; }

   friend class AttackMoveState; // Friendship with attack move state since it also uses similar attacking logic

 private:
   AUnit&           agent;                   // Unit whose behavior is specified through the behavioral logic within this state
   static const int shortAttRngBuff = 25.f;  // Distance that an attack in progress will cancel since it is out of range
   static const int attRngCnclBuff  = 350.f; // Distance that an attack in progress will cancel since it is out of range
   float            currentAttTime  = 0.f;   // Keeps track of how long we wait between auto attacks

   /** Game time representing what time we leave this state so that if we start it again, we can figure out if enough time has
    * elapsed for us to attack again without having to move the update logic outside AttackState */
   float stopTime = 0.f;

   /** True if our target is in attack range + cancel buffer and we're in attack state */
   bool bTgtInAttBuff = false;

   /** True if this unit is adjusting its position to be in range of the target which has moved out of range while we are attacking
    * Used to short circuit the update logic (since we don't need to check for anything besides vision if we're repositioning while attacking) */
   bool bMvingTwdTarg = false;

   bool attackAnimationPlaying = false;

   void OnHitEvent();

   inline void ResetAttTimer();

   inline bool AttemptReposition();

   /** Called if the repositioning is successful (requires us to be in range and called only once turn succeeds or we're already facing target) */
   void OnFinishReposition();

   inline void UpdateAttackTimer(float deltaSeconds);

   /** If any of the cancel conditions become true, the unit will transition to another state */
   bool CheckAndHandleCancelConditions() const;

   inline bool CheckTargetVisionLost() const;

   inline bool CheckTargetAttackable() const;

   inline void TransitionToChaseState() const;

   inline void StopAgent() const;

   bool AttackInitiationSetup();

   void PlayAttackAnimation();

   bool AttackSpeedTimerCheck() const;

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
