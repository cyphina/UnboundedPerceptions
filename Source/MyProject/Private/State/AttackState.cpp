#include "MyProject.h"

#include "AttackState.h"

#include "AttackAnim.h"
#include "RTSGameState.h"

#include "Unit.h"
#include "UnitMessages.h"
#include "UnitController.h"
#include "UserInput.h"
#include "BrainComponent.h"

#include "SpellDataLibrary.h"
#include "TargetComponent.h"

#include "UpAIHelperLibrary.h"
#include "UpStatComponent.h"
#include "RTSVisionComponent.h"

AttackState::AttackState()
{
   agent.OnUnitHit().AddSP(this, &AttackState::OnHitEvent);
}

void AttackState::OnHitEvent()
{
   ResetAttTimer();
   FAIMessage msg(UnitMessages::AIMessage_AttackReady, &agent);
   FAIMessage::Send(agent.unitController, msg);
}

void AttackState::ResetAttTimer()
{
   currentAttTime = 0;
}

AttackState::~AttackState()
{
}

void AttackState::Enter()
{
   attackAnimationPlaying = false;
   if(AttemptReposition()) {
      bMvingTwdTarg = true;
   } else {
      bMvingTwdTarg = false;
   }
}

void AttackState::Exit()
{
   ResetAttTimer();
   stopTime               = UGameplayStatics::GetTimeSeconds(&agent);
   bMvingTwdTarg         = false;
   attackAnimationPlaying = false;
}

void AttackState::Update(float deltaSeconds)
{
   UpdateAttackTimer(deltaSeconds);

   if(bMvingTwdTarg) {
      if(CheckAndHandleCancelConditions()) return;
   } else {
      if(CheckAndHandleCancelConditions()) {
         if(!attackAnimationPlaying) {
            if(AttackInitiationSetup()) PlayAttackAnimation();
         } else {
            LockOnTarget();
            CheckAndHandleTargetOutsideAnimationRange();
         }
      }
   }
}

bool AttackState::AttemptReposition()
{
   return agent.unitController->AdjustPosition(AgentAttackRange(), AgentTargetUnit(), [this]() { OnFinishReposition(); });
}

void AttackState::OnFinishReposition()
{
   bMvingTwdTarg         = false;
   attackAnimationPlaying = true;
   PlayAttackAnimation();
}

void AttackState::UpdateAttackTimer(float deltaSeconds)
{
   currentAttTime = FMath::Min(currentAttTime + deltaSeconds * agent.controllerRef->GetGameState()->GetGameSpeed(), AttackTimerThreshold());
}

bool AttackState::CheckAndHandleCancelConditions() const
{
   if(UNLIKELY(CheckTargetVisionLost())) {
      TransitionToChaseState();
      return false;
   }
   if(UNLIKELY(!CheckTargetAttackable())) {
      StopAgent();
      return false;
   }
   return true;
}

bool AttackState::CheckTargetVisionLost() const
{
   return !agent.targetComponent->GetTargetUnit()->GetVisionComponent()->IsUnitVisible();
}

bool AttackState::CheckTargetAttackable() const
{
   return USpellDataLibrary::IsAttackable(*agent.GetAbilitySystemComponent());
}

void AttackState::TransitionToChaseState() const
{
   agent.state->ChangeState(EUnitState::STATE_CHASING);
}

void AttackState::StopAgent() const
{
   agent.unitController->Stop();
}

bool AttackState::AttackInitiationSetup()
{
   if(AttemptReposition()) { return false; }
   return true;
}

void AttackState::PlayAttackAnimation()
{
   if(AttackSpeedTimerCheck()) {
      attackAnimationPlaying = true;
      agent.attackAnim->PlayAttackAnimation();
   }
}

bool AttackState::AttackSpeedTimerCheck() const
{
   return currentAttTime < AttackTimerThreshold() ? true : false;
}

float AttackState::AttackTimerThreshold() const
{
   // Similar to DOTA's formula where initial attack speed is 100, IAS (Increased Attack Speed) is our stat, and BAT is 2 for all units
   return 2 / ((AgentAttackSpeed() + 100) * 0.01);
}

void AttackState::LockOnTarget() const
{
   if(!UUpAIHelperLibrary::IsFacingTarget(agent, AgentTargetLocation())) {
      agent.SetActorRotation(UUpAIHelperLibrary::FindLookRotation(agent, AgentTargetLocation()));
   }
}

bool AttackState::CheckAndHandleTargetOutsideAnimationRange()
{
   if(!UUpAIHelperLibrary::IsTargetInRange(agent, AgentTargetLocation(), AgentBufferAttackRange())) {
      agent.attackAnim->StopAttackAnimation();
      attackAnimationPlaying = false;
      return false;
   }
   return true;
}

float AttackState::AgentAttackRange() const
{
   return agent.statComponent->GetMechanicAdjValue(EMechanics::AttackRange);
}

float AttackState::AgentAttackSpeed() const
{
   return agent.statComponent->GetSkillAdjValue(EUnitScalingStats::Attack_Speed);
}

float AttackState::AgentBufferAttackRange() const
{
   return AgentAttackRange() + bTgtInAttBuff;
}

FVector AttackState::AgentTargetLocation() const
{
   return agent.FindComponentByClass<UTargetComponent>()->GetTargetLocation();
}

AUnit* AttackState::AgentTargetUnit() const
{
   return agent.FindComponentByClass<UTargetComponent>()->GetTargetUnit();
}
