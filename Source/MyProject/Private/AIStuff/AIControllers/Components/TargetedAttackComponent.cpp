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

namespace AttackCVars
{
   bool                           bPrintAttackTimings = false;
   static FAutoConsoleVariableRef CVarPrintAttackTimings(TEXT("printAttackTimings"), bPrintAttackTimings,
                                                         TEXT("Prints out messagse when we start attacking and when we actually hit the unit."));
}

UTargetedAttackComponent::UTargetedAttackComponent()
{
   attackAnimClass = UNullAttackAnim::StaticClass();
}

void UTargetedAttackComponent::BeginAttack(AUnit* target)
{
   if(USpellDataLibrary::IsAttackable(target->GetAbilitySystemComponent()) && !USpellDataLibrary::IsStunned(agent->GetAbilitySystemComponent()))
   {
      agent->GetUnitController()->StopCurrentAction();
      agent->GetTargetComponent()->SetTarget(target);
      if(URTSStateComponent* stateComp = agent->FindComponentByClass<URTSStateComponent>())
      {
         stateComp->ChangeState(EUnitState::STATE_ATTACKING);
      }
      GetWorld()->GetTimerManager().SetTimer(attackChecksHandle, this, &UTargetedAttackComponent::FollowAndTryAttackTarget, 0.05f, true, 0.f);
   }
}

void UTargetedAttackComponent::BeginAttackMove(FVector targetLocation)
{
   agent->GetUnitController()->MoveToLocation(targetLocation);
   GetWorld()->GetTimerManager().SetTimer(targetSearchHandle, this, &UTargetedAttackComponent::SearchForTargetInRange, .1f, true, 0.f);
}

void UTargetedAttackComponent::OverrideAttackWithSpell(TSubclassOf<UMySpell> overridingSpell)
{
}

void UTargetedAttackComponent::BeginPlay()
{
   Super::BeginPlay();
   UObject* animInstance = NewObject<UObject>(this, attackAnimClass.Get());

   if(IAttackAnim* attackAnimInterface = Cast<IAttackAnim>(animInstance))
   {
      attackAnim.SetObject(animInstance);
      attackAnim.SetInterface(attackAnimInterface);
      attackAnimInterface->OnAttackNotify().AddUObject(this, &UTargetedAttackComponent::OnUnitAttackSwingDone);
      attackAnimInterface->OnAttackAnimFinished().AddWeakLambda(this, [this]() { bAttackAnimationPlaying = false; });
   }

   agent = Cast<AUnitController>(GetOwner())->GetUnitOwner();
   agent->GetUnitController()->OnUnitStopped().AddUObject(this, &UTargetedAttackComponent::OnUnitStopped);
}

void UTargetedAttackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
   if(GetWorld())
   {
      GetWorld()->GetTimerManager().ClearTimer(attackUpdateHandle);
   }
}

void UTargetedAttackComponent::SearchForTargetInRange()
{
   if(AUnit* closestUnit = UUpAIHelperLibrary::FindClosestUnit(agent->GetActorLocation(), agent->GetVisionComponent()->GetPossibleVisibleEnemies()))
   {
      BeginAttack(closestUnit);
      GetWorld()->GetTimerManager().ClearTimer(targetSearchHandle);
   }
}

bool UTargetedAttackComponent::AttemptReposition()
{
   return agent->GetUnitController()->AdjustPosition(AgentAttackRange(), AgentTargetUnit(), [this]() { OnFinishReposition(); });
}

void UTargetedAttackComponent::OnUnitAttackSwingDone()
{
   if(AttackCVars::bPrintAttackTimings)
   {
      GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, "Hit Unit!");
   }

   readyToAttack = false;
   OnAttackSwingDoneEffect();

   const FAIMessage msg(UnitMessages::AIMessage_Hit, agent);
   FAIMessage::Send(agent->GetUnitController(), msg);

   GetWorld()->GetTimerManager().SetTimer(
   attackUpdateHandle, [this]() { readyToAttack = true; }, AttackTimerThreshold(), false);

   if(USpellDataLibrary::IsInvisible(agent->GetAbilitySystemComponent()))
   {
      agent->GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Effect.Invisibility")));
   }
}

void UTargetedAttackComponent::FollowAndTryAttackTarget()
{
   if(CheckAndHandleCancelConditions())
   {
      if(!bAttackAnimationPlaying)
      {
         if(HandlePositionalAdjustments())
         {
            if(readyToAttack)
               PlayAttackAnimation();
         }
      } else
      {
         LockOnTarget();
         CheckAndHandleTargetOutsideAnimationRange();
      }
   }
}

void UTargetedAttackComponent::OnFinishReposition()
{
   bMvingTwdTarg           = false;
   bAttackAnimationPlaying = true;
   PlayAttackAnimation();
}

bool UTargetedAttackComponent::CheckAndHandleCancelConditions()
{
   if(LIKELY(AgentTargetUnit()))
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
   StopAgent();
   return false;
}

bool UTargetedAttackComponent::CheckTargetVisionLost() const
{
   return !agent->GetTargetComponent()->GetTargetUnit()->GetVisionComponent()->IsUnitVisible();
}

bool UTargetedAttackComponent::CheckTargetAttackable() const
{
   return USpellDataLibrary::IsAttackable(agent->GetAbilitySystemComponent());
}

void UTargetedAttackComponent::TransitionToChaseState()
{
   if(URTSStateComponent* stateComp = agent->FindComponentByClass<URTSStateComponent>())
   {
      stateComp->ChangeState(EUnitState::STATE_CHASING);
   }

   StopAttackAnim();
}

void UTargetedAttackComponent::StopAgent() const
{
   agent->GetUnitController()->StopCurrentAction();
}

void UTargetedAttackComponent::StopAttackAnim()
{
   if(attackAnim)
   {
      attackAnim->StopAttackAnimation();
      bAttackAnimationPlaying = false;
   }
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
   if(AttackCVars::bPrintAttackTimings)
   {
      GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, "Starting Swing Animation!");
   }

   bAttackAnimationPlaying = true;
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
   bMvingTwdTarg           = false;
   bAttackAnimationPlaying = false;
   attackAnim->StopAttackAnimation();
   GetWorld()->GetTimerManager().ClearTimer(attackChecksHandle);
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
   if(ABaseHero* hero = Cast<ABaseHero>(agent))
   {
      if(hero->GetEquipment()->GetWeaponId() > 0)
      {
         UItemFunctionLibrary::GetEquipInfo(hero->GetEquipment()->GetWeaponId()).stats.element;
      }
   }
   return FGameplayTag::RequestGameplayTag("Combat.Element.None");
}

void UTargetedAttackComponent::CreateRangedProjectile(FDamageScalarStruct projectileDamageScalars)
{
   const FVector    agentLocation = agent->GetActorLocation();
   const FTransform transform = FTransform{FVector(agentLocation.X, agentLocation.Y, agentLocation.Z + agent->GetCapsuleComponent()->GetScaledCapsuleHalfHeight())};
   ARTSProjectile*  projectile = ARTSProjectile::MakeRTSProjectile(GetWorld(), agent->GetTargetComponent(), transform, ARTSProjectile::StaticClass(), projectileStrategy);

   projectile->hitEffects.Add(agent->GetAbilitySystemComponent()->MakeDamageEffect(projectileDamageScalars, GetAttackElement()));
}

bool UTargetedAttackComponent::CheckAndHandleTargetOutsideAnimationRange()
{
   if(!UUpAIHelperLibrary::IsTargetInRange(agent, AgentTargetLocation(), AgentBufferAttackRange()))
   {
      StopAttackAnim();
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
   return agent->GetTargetComponent()->GetTargetUnit()->GetActorLocation();
}

AUnit* UTargetedAttackComponent::AgentTargetUnit() const
{
   if(agent->GetTargetComponent()->IsTargetingUnit())
   {
      return agent->GetTargetComponent()->GetTargetUnit();
   }
   return nullptr;
}
