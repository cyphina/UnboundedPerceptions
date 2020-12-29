// Created 12/26/20 7:11 PM

#include "MyProject.h"
#include "TargetedAttackComponent.h"

#include "BaseCharacter.h"
#include "BrainComponent.h"
#include "NullAttackAnim.h"
#include "RTSStateComponent.h"
#include "SpellDataLibrary.h"
#include "TargetComponent.h"
#include "Unit.h"
#include "UnitController.h"
#include "RTSAbilitySystemComponent.h"
#include "RTSGameState.h"
#include "RTSVisionComponent.h"
#include "UpAIHelperLibrary.h"
#include "UpStatComponent.h"

UTargetedAttackComponent::UTargetedAttackComponent()
{
   agent->OnUnitHit().AddSP(this, &UTargetedAttackComponent::OnHitEvent);
   attackAnimClass = UNullAttackAnim::StaticClass();
}

void UTargetedAttackComponent::BeginAttack(AUnit* target)
{
   if(USpellDataLibrary::IsAttackable(*target->GetAbilitySystemComponent()) && !USpellDataLibrary::IsStunned(*agent->GetAbilitySystemComponent())) {
      agent->GetUnitController()->Stop();
      agent->GetTargetComponent()->SetTarget(target);
      agent->FindComponentByClass<URTSStateComponent>()->ChangeState(EUnitState::STATE_ATTACKING);
      InitializeAttackParams();
   }
}

void UTargetedAttackComponent::BeginAttackMove(FVector targetLocation)
{
   agent->GetUnitController()->MoveToLocation(targetLocation);
   GetWorld()->GetTimerManager().SetTimer(targetSearchHandle, this, &UTargetedAttackComponent::SearchForTargetInRange, .1f, true, 0);
}

void UTargetedAttackComponent::BeginPlay()
{
   agent = Cast<AUnitController>(GetOwner())->GetUnitOwner();
   agent->GetUnitController()->OnUnitStopped().AddUObject(this, &UTargetedAttackComponent::OnUnitStopped);
}

void UTargetedAttackComponent::OnHitEvent()
{
   readyToAttack = false;
   const FAIMessage msg(UnitMessages::AIMessage_AttackReady, agent);
   FAIMessage::Send(agent->GetUnitController(), msg);
   GetWorld()->GetTimerManager().SetTimer(attackUpdateHandle, this, &UTargetedAttackComponent::FollowAndTryAttackTarget, AttackTimerThreshold(), true, 0.f);
}

void UTargetedAttackComponent::InitializeAttackParams()
{
   attackAnimationPlaying = false;
   if(AttemptReposition()) {
      bMvingTwdTarg = true;
   } else {
      bMvingTwdTarg = false;
   }
}

void UTargetedAttackComponent::FollowAndTryAttackTarget()
{
   if(bMvingTwdTarg) {
      if(CheckAndHandleCancelConditions()) return;
   } else {
      if(CheckAndHandleCancelConditions()) {
         if(!attackAnimationPlaying) {
            if(HandlePositionalAdjustments()) {
               if(readyToAttack) PlayAttackAnimation();
            }
         } else {
            LockOnTarget();
            CheckAndHandleTargetOutsideAnimationRange();
         }
      }
   }
}

void UTargetedAttackComponent::SearchForTargetInRange()
{
   if(AUnit* closestUnit = UUpAIHelperLibrary::FindClosestUnit(*agent, agent->GetVisionComponent()->GetPossibleVisibleEnemies())) {
      BeginAttack(closestUnit);
      GetWorld()->GetTimerManager().ClearTimer(targetSearchHandle);
   }
}

bool UTargetedAttackComponent::AttemptReposition()
{
   return agent->GetUnitController()->AdjustPosition(AgentAttackRange(), AgentTargetUnit(), [this]() { OnFinishReposition(); });
}

void UTargetedAttackComponent::OnFinishReposition()
{
   bMvingTwdTarg          = false;
   attackAnimationPlaying = true;
   PlayAttackAnimation();
}

bool UTargetedAttackComponent::CheckAndHandleCancelConditions() const
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

bool UTargetedAttackComponent::CheckTargetVisionLost() const
{
   return !agent->GetTargetComponent()->GetTargetUnit()->GetVisionComponent()->IsUnitVisible();
}

bool UTargetedAttackComponent::CheckTargetAttackable() const
{
   return USpellDataLibrary::IsAttackable(*agent->GetAbilitySystemComponent());
}

void UTargetedAttackComponent::TransitionToChaseState() const
{
   agent->FindComponentByClass<URTSStateComponent>()->ChangeState(EUnitState::STATE_CHASING);
}

void UTargetedAttackComponent::StopAgent() const
{
   agent->GetUnitController()->Stop();
}

bool UTargetedAttackComponent::HandlePositionalAdjustments()
{
   if(AttemptReposition()) { return true; }
   return false;
}

void UTargetedAttackComponent::PlayAttackAnimation()
{
   attackAnimationPlaying = true;
   attackAnim->PlayAttackAnimation();
}

float UTargetedAttackComponent::AttackTimerThreshold() const
{
   // Similar to DOTA's formula where initial attack speed is 100, IAS (Increased Attack Speed) is our stat, and BAT is 2 for all units
   return 2 / ((AgentAttackSpeed() + 100) * 0.01);
}

void UTargetedAttackComponent::OnUnitStopped()
{
   bMvingTwdTarg          = false;
   attackAnimationPlaying = false;
   GetWorld()->GetTimerManager().ClearTimer(attackUpdateHandle);
   GetWorld()->GetTimerManager().ClearTimer(targetSearchHandle);
}

void UTargetedAttackComponent::LockOnTarget() const
{
   if(!UUpAIHelperLibrary::IsFacingTarget(*agent, AgentTargetLocation())) {
      agent->SetActorRotation(UUpAIHelperLibrary::FindLookRotation(*agent, AgentTargetLocation()));
   }
}

bool UTargetedAttackComponent::CheckAndHandleTargetOutsideAnimationRange()
{
   if(!UUpAIHelperLibrary::IsTargetInRange(*agent, AgentTargetLocation(), AgentBufferAttackRange())) {
      attackAnim->StopAttackAnimation();
      attackAnimationPlaying = false;
      return false;
   }
   return true;
}

float UTargetedAttackComponent::AgentAttackRange() const
{
   return agent->GetStatComponent()->GetMechanicAdjValue(EMechanics::AttackRange);
}

float UTargetedAttackComponent::AgentAttackSpeed() const
{
   return agent->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Attack_Speed);
}

float UTargetedAttackComponent::AgentBufferAttackRange() const
{
   return AgentAttackRange() + attRngCnclBuff;
}

FVector UTargetedAttackComponent::AgentTargetLocation() const
{
   return agent->FindComponentByClass<UTargetComponent>()->GetTargetLocation();
}

AUnit* UTargetedAttackComponent::AgentTargetUnit() const
{
   return agent->FindComponentByClass<UTargetComponent>()->GetTargetUnit();
}
