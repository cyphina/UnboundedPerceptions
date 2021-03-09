// Created 12/26/20 7:11 PM

#include "MyProject.h"
#include "TargetedAttackComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "RTSProjectileStrategy.h"
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
                                                         TEXT("Prints out messages when we start attacking and when we actually hit the unit."));
}

UTargetedAttackComponent::UTargetedAttackComponent()
{
   attackAnimStrategy = UNullAttackAnim::StaticClass();
}

void UTargetedAttackComponent::BeginAttack(AUnit* target)
{
   if(USpellDataLibrary::IsAttackable(target->GetAbilitySystemComponent()) && !USpellDataLibrary::IsStunned(agent->GetAbilitySystemComponent()))
   {
      agent->GetTargetComponent()->SetTarget(target);
      if(URTSStateComponent* stateComp = agent->GetUnitController()->FindComponentByClass<URTSStateComponent>())
      {
         if(stateComp->GetState() != EUnitState::STATE_ATTACK_MOVE)
         {
            stateComp->ChangeState(EUnitState::STATE_ATTACKING);
         }
      }
      GetWorld()->GetTimerManager().SetTimer(attackChecksHandle, this, &UTargetedAttackComponent::FollowAndTryAttackTarget, 0.05f, true, 0.f);
   }
}

void UTargetedAttackComponent::BeginAttackMove(FVector targetLocation)
{
   if(!USpellDataLibrary::IsStunned(agent->GetAbilitySystemComponent()))
   {
      attackMoveLocation = targetLocation;

      if(!bAttackAnimationPlaying)
      {
         agent->GetUnitController()->AdjustPosition(agent->GetUnitController()->smallMoveIgnoreRange, attackMoveLocation, [this]() {
            if(UUpAIHelperLibrary::IsTargetInRange(agent, attackMoveLocation, agent->GetUnitController()->smallMoveIgnoreRange))
            {
               agent->GetUnitController()->FinishCurrentAction();
            }
         });
      }

      if(URTSStateComponent* stateComp = agent->GetUnitController()->FindComponentByClass<URTSStateComponent>())
      {
         stateComp->ChangeState(EUnitState::STATE_ATTACK_MOVE);
      }

      GetWorld()->GetTimerManager().SetTimer(targetSearchHandle, this, &UTargetedAttackComponent::SearchForTargetInRange, .1f, true, 0.f);
   }
}

void UTargetedAttackComponent::OverrideAttackWithSpell(TSubclassOf<UMySpell> overridingSpell)
{
}

void UTargetedAttackComponent::BeginPlay()
{
   Super::BeginPlay();

   GetWorld()->GetTimerManager().SetTimerForNextTick([this]() {
      agent = Cast<AUnitController>(GetOwner())->GetUnitOwner();
      agent->GetUnitController()->OnUnitStopped().AddUObject(this, &UTargetedAttackComponent::OnUnitStopped);

      if(attackAnimStrategy)
      {
         animAttackStrategyInst = NewObject<UObject>(this, attackAnimStrategy.Get());
         if(IAttackAnim* attackAnimInterface = Cast<IAttackAnim>(animAttackStrategyInst))
         {
            attackAnim.SetObject(animAttackStrategyInst);
            attackAnim.SetInterface(attackAnimInterface);

            if(FOnHitNotify* hitDelegate = attackAnimInterface->OnAttackNotify(agent))
            {
               hitDelegate->AddUObject(this, &UTargetedAttackComponent::OnUnitAttackSwingDone);
               attackAnimInterface->OnAttackAnimFinished()->AddWeakLambda(this, [this]() {
                  GetWorld()->GetTimerManager().SetTimer(
                      attackUpdateHandle,
                      [this]() {
                         bReadyToAttack = true;
                      },
                      AttackTimerThreshold(), false);
                  bAttackAnimationPlaying = false;
               });
            }
         }
      }
   });
}

void UTargetedAttackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
   if(GetWorld())
   {
      GetWorld()->GetTimerManager().ClearTimer(attackUpdateHandle);
      StopAttackAnim();
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
   return agent->GetUnitController()->AdjustPosition(AgentAttackRange(), AgentTargetUnit(), [this]() {
      OnFinishReposition();
   });
}

void UTargetedAttackComponent::OnUnitAttackSwingDone()
{
   if(AttackCVars::bPrintAttackTimings)
   {
      GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, "Hit Unit!");
   }

   bReadyToAttack = false;
   OnAttackSwingDoneEffect();

   const FAIMessage msg(UnitMessages::AIMessage_AttackExecuted, agent);
   FAIMessage::Send(agent->GetUnitController(), msg);

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
            if(bReadyToAttack) PlayAttackAnimation();
         }
      }
      else
      {
         LockOnTarget();
         CheckAndHandleTargetOutsideAnimationRange();
      }
   }
}

void UTargetedAttackComponent::OnFinishReposition()
{
   bMvingTwdTarg = false;
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
   if(AUnit* targetUnit = agent->GetTargetComponent()->GetTargetUnit(); IsValid(targetUnit))
   {
      return !targetUnit->GetVisionComponent()->IsUnitVisible();
   }
   return true;
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

void UTargetedAttackComponent::StopAgent()
{
   OnUnitStopped();
   if(agent->GetUnitController()->GetState() != EUnitState::STATE_ATTACK_MOVE)
   {
      agent->GetUnitController()->FinishCurrentAction();
   }
   else
   {
      BeginAttackMove(attackMoveLocation);
   }
}

void UTargetedAttackComponent::StopAttackAnim()
{
   if(attackAnim)
   {
      if(agent)
      {
         attackAnim->StopAttackAnimation(agent);
         bAttackAnimationPlaying = false;
      }
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
      if(agent)
      {
         attackAnim->PlayAttackAnimation(agent);
      }
   }
}

float UTargetedAttackComponent::AttackTimerThreshold() const
{
   // Similar to DOTA's formula where initial attack speed is 100, IAS (Increased Attack Speed) is our stat, and BAT is 2 for all units
   return 2 / ((AgentAttackSpeed() + 100) * 0.01);
}

void UTargetedAttackComponent::OnUnitStopped()
{
   if(agent)
   {
      bMvingTwdTarg = false;
      StopAttackAnim();
      GetWorld()->GetTimerManager().ClearTimer(attackChecksHandle);
      GetWorld()->GetTimerManager().ClearTimer(targetSearchHandle);
   }
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
   bool bDamageEffectSucceeded = false;
   switch(agent->GetCombatInfo()->combatStyle)
   {
      case ECombatType::Melee:
      {
         bDamageEffectSucceeded = agent->GetAbilitySystemComponent()->ApplyDamageToTarget(agent->GetTargetComponent()->GetTargetUnit()->GetAbilitySystemComponent(),
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

   if(!agent->GetCombatInfo()->bMissLastHit)
   {
      agent->GetStatComponent()->ModifyStats(agent->GetStatComponent()->GetVitalCurValue(EVitals::Mana) + 5, EVitals::Mana);
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
   const FTransform transform     = FTransform{FVector(agentLocation.X, agentLocation.Y, agentLocation.Z + agent->GetCapsuleComponent()->GetScaledCapsuleHalfHeight())};
   ARTSProjectile*  projectile =
       ARTSProjectile::MakeRTSProjectile(GetWorld(), agent->GetTargetComponent(), transform, ARTSProjectile::StaticClass(), projectileStrategyClass.GetDefaultObject());

   projectile->hitEffects.Add(agent->GetAbilitySystemComponent()->MakeDamageEffect(projectileDamageScalars, GetAttackElement()));
}

bool UTargetedAttackComponent::CheckAndHandleTargetOutsideAnimationRange()
{
   if(!UUpAIHelperLibrary::IsTargetInRangeOfActor(agent, agent->GetTargetComponent()->GetTargetUnit(), AgentBufferAttackRange()))
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
      AUnit* unit = agent->GetTargetComponent()->GetTargetUnit();
      if(IsValid(unit) && unit->IsEnabled())
      {
         return agent->GetTargetComponent()->GetTargetUnit();
      }
   }
   return nullptr;
}
