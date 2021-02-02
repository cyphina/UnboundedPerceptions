// Created 12/26/20 7:11 PM

#include "MyProject.h"
#include "TargetedAttackComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseCharacter.h"
#include "BaseHero.h"
#include "BrainComponent.h"
#include "CombatParameters.h"
#include "Enemy.h"
#include "EquipmentContainer.h"
#include "ItemFunctionLibrary.h"
#include "NullAttackAnim.h"
#include "RTSStateComponent.h"
#include "SpellDataLibrary.h"
#include "TargetComponent.h"
#include "Unit.h"
#include "UnitController.h"
#include "RTSAbilitySystemComponent.h"
#include "RTSDamageEffect.h"
#include "RTSProjectile.h"
#include "UnitMessages.h"
#include "RTSVisionComponent.h"
#include "UpAIHelperLibrary.h"
#include "UpStatComponent.h"

UTargetedAttackComponent::UTargetedAttackComponent()
{
   attackAnimClass = UNullAttackAnim::StaticClass();
}

void UTargetedAttackComponent::BeginAttack(AUnit* target)
{
   if(USpellDataLibrary::IsAttackable(target->GetAbilitySystemComponent()) && !USpellDataLibrary::IsStunned(agent->GetAbilitySystemComponent()))
   {
      agent->GetUnitController()->Stop();
      agent->GetTargetComponent()->SetTarget(target);
      if(URTSStateComponent* stateComp = agent->FindComponentByClass<URTSStateComponent>())
      {
         stateComp->ChangeState(EUnitState::STATE_ATTACKING);
      }
      InitializeAttackParams();
   }
}

void UTargetedAttackComponent::BeginAttackMove(FVector targetLocation)
{
   agent->GetUnitController()->MoveToLocation(targetLocation);
   GetWorld()->GetTimerManager().SetTimer(targetSearchHandle, this, &UTargetedAttackComponent::SearchForTargetInRange, .1f, true, 0);
}

void UTargetedAttackComponent::OverrideAttackWithSpell(TSubclassOf<UMySpell> overridingSpell)
{
}

void UTargetedAttackComponent::BeginPlay()
{
   Super::BeginPlay();
   agent = Cast<AUnitController>(GetOwner())->GetUnitOwner();
   agent->GetUnitController()->OnUnitStopped().AddUObject(this, &UTargetedAttackComponent::OnUnitStopped);
   agent->OnUnitAttackSwingHit().AddUObject(this, &UTargetedAttackComponent::OnUnitAttackSwingDone);
}

void UTargetedAttackComponent::OnUnitAttackSwingDone()
{
   readyToAttack = false;
   OnAttackSwingDoneEffect();
   const FAIMessage msg(UnitMessages::AIMessage_Hit, agent);
   FAIMessage::Send(agent->GetUnitController(), msg);
   if(USpellDataLibrary::IsInvisible(agent->GetAbilitySystemComponent()))
   {
      agent->GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Effect.Invisibility")));
   }
   GetWorld()->GetTimerManager().SetTimer(attackUpdateHandle, this, &UTargetedAttackComponent::FollowAndTryAttackTarget, AttackTimerThreshold(), true, 0.f);
}

void UTargetedAttackComponent::InitializeAttackParams()
{
   attackAnimationPlaying = false;
   if(AttemptReposition())
   {
      bMvingTwdTarg = true;
   }
   else
   {
      bMvingTwdTarg = false;
   }
}

void UTargetedAttackComponent::FollowAndTryAttackTarget()
{
   if(bMvingTwdTarg)
   {
      if(CheckAndHandleCancelConditions()) return;
   }
   else
   {
      if(CheckAndHandleCancelConditions())
      {
         if(!attackAnimationPlaying)
         {
            if(HandlePositionalAdjustments())
            {
               if(readyToAttack) PlayAttackAnimation();
            }
         }
         else
         {
            LockOnTarget();
            CheckAndHandleTargetOutsideAnimationRange();
         }
      }
   }
}

void UTargetedAttackComponent::SearchForTargetInRange()
{
   if(AUnit* closestUnit = UUpAIHelperLibrary::FindClosestUnit(agent, agent->GetVisionComponent()->GetPossibleVisibleEnemies()))
   {
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
   if(UNLIKELY(CheckTargetVisionLost()))
   {
      TransitionToChaseState();
      return false;
   }
   if(UNLIKELY(!CheckTargetAttackable()))
   {
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
   return USpellDataLibrary::IsAttackable(agent->GetAbilitySystemComponent());
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
   if(AttemptReposition())
   {
      return true;
   }
   return false;
}

void UTargetedAttackComponent::PlayAttackAnimation()
{
   attackAnimationPlaying = true;
   if(attackAnim)
   {
      attackAnim->PlayAttackAnimation();
   }
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
   if(!UUpAIHelperLibrary::IsFacingTarget(agent, AgentTargetLocation()))
   {
      agent->SetActorRotation(UUpAIHelperLibrary::FindLookRotation(agent, AgentTargetLocation()));
   }
}

void UTargetedAttackComponent::OnAttackSwingDoneEffect()
{
   switch(agent->GetCombatInfo()->combatStyle)
   {
      case ECombatType::Melee:
      {
         agent->GetAbilitySystemComponent()->ApplyDamageToTarget(agent->GetTargetComponent()->GetTargetUnit()->GetAbilitySystemComponent(),
                                                                 FDamageScalarStruct(0, 100, 0, 0, 0), GetAttackElement());
         break;
      }
      case ECombatType::Ranged:
      {
         CreateRangedProjectile(FDamageScalarStruct(0, 0, 0, 100, 0));
         break;
      }
      case ECombatType::Magic:
      {
         CreateRangedProjectile(FDamageScalarStruct(0, 0, 100, 0, 0));
         break;
      }
   }
}

void UTargetedAttackComponent::HandleAutoAttackModifierTags()
{
}

FGameplayTag UTargetedAttackComponent::GetAttackElement() const
{
   // TODO: Flesh this out more...
   if(agent->GetIsEnemy())
   {
      return Cast<AEnemy>(agent)->GetInitialStats().element;
   }
   else
   {
      if(ABaseHero* hero = Cast<ABaseHero>(agent))
      {
         UItemFunctionLibrary::GetEquipInfo(hero->GetEquipment()->GetWeaponId()).stats.element;
      }
   }
   return FGameplayTag::RequestGameplayTag("Combat.Element.None");
}

void UTargetedAttackComponent::CreateRangedProjectile(FDamageScalarStruct projectileDamageScalars)
{
   const FVector    agentLocation = agent->GetActorLocation();
   const FTransform transform     = FTransform{FVector(agentLocation.X, agentLocation.Y, agentLocation.Z + agent->GetCapsuleComponent()->GetScaledCapsuleHalfHeight())};
   ARTSProjectile*  projectile = ARTSProjectile::MakeRTSProjectile(GetWorld(), agent->GetTargetComponent(), transform, ARTSProjectile::StaticClass(), projectileStrategy);

   projectile->hitEffects.Add(agent->GetAbilitySystemComponent()->MakeDamageEffect(projectileDamageScalars, GetAttackElement()));
}

bool UTargetedAttackComponent::CheckAndHandleTargetOutsideAnimationRange()
{
   if(!UUpAIHelperLibrary::IsTargetInRange(agent, AgentTargetLocation(), AgentBufferAttackRange()))
   {
      if(attackAnim)
      {
         attackAnim->StopAttackAnimation();
         attackAnimationPlaying = false;
         return false;
      }
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
