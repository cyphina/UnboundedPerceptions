// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"

#include "WorldObjects/Unit.h"

#include "UserInput.h"
#include "UpResourceManager.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "BehaviorTree/BTTaskNode.h"

#include "AbilitySystemBlueprintLibrary.h"

#include "SpellSystem/MySpell.h"
#include "PatrolComponent.h"
#include "TargetComponent.h"
#include "UpStatComponent.h"

#include "MoveVisitorDefs.inl"
#include "RTSAttackExecution.h"
#include "SpellDataLibrary.h"

const FName AUnitController::AIMessage_Help = TEXT("HelpMe!");
// ! Requires us to have the proper current spell already set

AUnitController::AUnitController()
{
   behaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
   blackboardComp   = CreateDefaultSubobject<UBlackboardComponent>(FName("BlackboardComp"));

   SetActorTickInterval(0.f);

   ConstructorHelpers::FObjectFinder<UCurveFloat> loadedCurve(TEXT("/Game/RTS_Tutorial/HUDs/ActionUI/StandardLinear"));
   if(loadedCurve.Succeeded())
      turnCurve = loadedCurve.Object;
}

void AUnitController::OnPossess(APawn* InPawn)
{
   Super::OnPossess(InPawn);
   ownerRef = Cast<AUnit>(GetPawn());
}

void AUnitController::SetupTurnPointTimeline()
{
   FOnTimelineFloat turnProgressFunction;
   turnProgressFunction.BindUFunction(this, FName("Turn"));
   turnTimeline.AddInterpFloat(turnCurve, turnProgressFunction);
   turnTimeline.SetTimelineLength(1.f);
   turnTimeline.SetLooping(false);

   FOnTimelineEvent onTimeLineFinished;
   onTimeLineFinished.BindUFunction(this, "OnPointTurnFinished");
   turnTimeline.SetTimelineFinishedFunc(onTimeLineFinished);
}

void AUnitController::SetupTurnActorTimeline()
{
   FOnTimelineFloat turnActorProgressFunction;
   turnActorProgressFunction.BindUFunction(this, FName("TurnActor"));
   turnActorTimeline.AddInterpFloat(turnCurve, turnActorProgressFunction);
   turnActorTimeline.SetTimelineLength(1.f);
   turnActorTimeline.SetLooping(false);

   FOnTimelineEvent onTimeLineActorFinished;
   onTimeLineActorFinished.BindUFunction(this, "OnActorTurnFinished");
   turnActorTimeline.SetTimelineFinishedFunc(onTimeLineActorFinished);
}

void AUnitController::BeginPlay()
{
   Super::BeginPlay();
   CPCRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());

   if(turnCurve) {
      SetupTurnPointTimeline();
      SetupTurnActorTimeline();
   }

   GetUnitOwner()->OnUnitDamageReceivedEvent.BindUObject(this, &AUnitController::OnDamageReceived);
}

void AUnitController::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);
   turnTimeline.TickTimeline(deltaSeconds);
   turnActorTimeline.TickTimeline(deltaSeconds);
}

void AUnitController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
   Super::OnMoveCompleted(RequestID, Result);

   // Check to see if we need to turn towards a point or a target after we're done moving.
   // Moving naturally rotates us to our desired point/target, but it will stop prematurely if we're in range.
   // We only do this on success because if the action is canceled we don't want us to keep going.
   if(Result.IsSuccess()) {
      QueuedTurnAction();
   }
}





void AUnitController::Attack()
{
   customAttackLogic.GetDefaultObject()Execute();
   GetUnitOwner()->OnUnitAttackEvent.Broadcast();
}

bool AUnitController::SearchAndCastSpell(const FGameplayTagContainer& spellRequirementTags)
{
   UMySpell* spellToCast = nullptr;
   for(int i = 0; i < ownerRef->abilities.Num(); ++i) {
      if(IsValid(ownerRef->abilities[i])) {
         if(ownerRef->abilities[i].GetDefaultObject()->AbilityTags.HasAny(spellRequirementTags) && ownerRef->CanCast(ownerRef->abilities[i])) {
            spellToCastIndex = i;
            spellToCast      = ownerRef->abilities[i].GetDefaultObject();
         }
      }
   }

   if(spellToCast) {
      FindBestSpellTarget(spellToCast->GetTargetting(), true);
      return true;
   }
   return false;
}

void AUnitController::OnDamageReceived(const FUpDamage& d)
{
   GetUnitOwner()->statComponent->ModifyStats<false>(GetUnitOwner()->statComponent->GetVitalCurValue(EVitals::Health) - d.damage, EVitals::Health);
   if(GetUnitOwner()->statComponent->GetVitalAdjValue(EVitals::Health) < 0) {
      Die();
   }
}

void AUnitController::Die()
{
   deathExec->Execute();
   GetUnitOwner()->OnUnitDieEvent.Broadcast(*GetUnitOwner());
}

EPathFollowingRequestResult::Type AUnitController::Move(FVector newLocation)
{
   if(!ownerRef->IsStunned()) {
      // cancel any targets we may have since our focus with this command is pure movement
      Stop();
      // shift location a little bit if we're moving multiple units so they can group together ok
      FVector shiftedLocation = newLocation - ownerRef->GetActorLocation().GetSafeNormal() * ownerRef->GetCapsuleComponent()->GetScaledCapsuleRadius() / 2;
      // setup end rotation here in case we decide to do a ministep where we want to turn but not move
      ownerRef->SetTargetLocation(shiftedLocation);
      EPathFollowingRequestResult::Type result = MoveToLocation(shiftedLocation, 10, true, true, false, true);
      if(result == EPathFollowingRequestResult::RequestSuccessful) {
         ownerRef->state->ChangeState(EUnitState::STATE_MOVING);
      } else
         TurnTowardsPoint(shiftedLocation);
      return result;
   }
   return EPathFollowingRequestResult::Failed;
}

EPathFollowingRequestResult::Type AUnitController::MoveActor(AActor* targetActor)
{
   if(!ownerRef->IsStunned()) {
      Stop();
      ownerRef->SetTargetActor(targetActor);
      EPathFollowingRequestResult::Type result = MoveToActor(targetActor);

      if(result == EPathFollowingRequestResult::RequestSuccessful) {
         ownerRef->state->ChangeState(EUnitState::STATE_MOVING);
      }
      return result;
   }
   return EPathFollowingRequestResult::Failed;
}

void AUnitController::Follow()
{
   if(ownerRef->targetData->followTarget) {
      MoveActor(ownerRef->targetData->followTarget);
   }
}

void AUnitController::Patrol(TArray<FVector> newPatrolPoints)
{
   UPatrolComponent* patrolComp = GetUnitOwner()->FindComponentByClass<UPatrolComponent>();
   if(ensure(patrolComp != nullptr)) {
      patrolComp->patrolPoints = newPatrolPoints;
      if(patrolComp) {
         if(behaviorTreeComp->IsRunning())
            behaviorTreeComp->StopTree();
         UseBlackboard(patrolComp->patrolTree->BlackboardAsset, blackboardComp);
         behaviorTreeComp->RestartTree();
         behaviorTreeComp->StartTree(*patrolComp->patrolTree);
      }
   } else {
      UE_LOG(LogTemp, Error, TEXT("Cannot patrol or attack move on a unit that does not have a patrol component!"));
   }
}

void AUnitController::Stop()
{
   // Stop will not only stop unit in its track, but will also make it forget its target, and set its state to idle so it can transition to something else
   onPosAdjDoneAct  = nullptr;
   QueuedTurnAction = nullptr;
   turnActorTimeline.Stop();
   turnTimeline.Stop();
   ownerRef->SetCurrentSpell(nullptr);
   ownerRef->targetData->ResetTarget();
   ownerRef->combatParams.readyToAttack  = false;
   ownerRef->combatParams.currentAttTime = 0.f;
   ownerRef->unitController->StopMovement();
   ownerRef->state->ChangeState(EUnitState::STATE_IDLE);
   ownerRef->StopAnimMontage();
}

void AUnitController::StopAutomation() const
{
   if(behaviorTreeComp->IsRunning())
      behaviorTreeComp->StopTree();
}

void AUnitController::BeginAttack(AUnit* target)
{
   if(USpellDataLibrary::IsAttackable(*target->GetAbilitySystemComponent()) && !USpellDataLibrary::IsStunned(*ownerRef->GetAbilitySystemComponent()) {
      Stop();
      ownerRef->targetComponent->SetTargetUnit(target);
      ownerRef->state->ChangeState(EUnitState::STATE_ATTACKING);
   }
}

void AUnitController::CastTurnAction()
{
   IncantationCheck(ownerRef->GetCurrentSpell());
}

bool AUnitController::BeginCastSpell(TSubclassOf<UMySpell> spellToCast)
{
   UMySpell* spell = spellToCast.GetDefaultObject();

   if(IsValid(spell)) {
      if(GetUnitOwner()->CanCast(spellToCast)) {
         Stop();
         GetUnitOwner()->SetCurrentSpell(spellToCast); //Keep track of the spell since we may cast it later (we may have to move into position first)

         if(spell->GetTargetting().GetTagName() == "Skill.Targetting.None") //Non-targetted?  Then just cast it
         {
            IncantationCheck(GetUnitOwner()->GetCurrentSpell());
            return true;
         } else { //This spell requires a ground or unit target

            if(spell->GetTargetting().MatchesTag(FGameplayTag::RequestGameplayTag("Skill.Targetting.Area"))) {
               // If the target position is extremely close (like we're targetting the ground underneath us which isn't a problem with click confirm because it would)
               // register the click trace as hitting ourself
               if(FVector::Dist2D(GetUnitOwner()->GetTargetLocation(), GetUnitOwner()->GetActorLocation()) < 5.f) {
                  IncantationCheck(GetUnitOwner()->GetCurrentSpell());
                  return true;
               }
               AdjustCastingPosition(spellToCast, GetUnitOwner()->GetTargetLocation());
            } else {
               // If the target unit is ourself
               if(IsTargettingSelf(GetUnitOwner()->GetTargetUnit())) {
                  IncantationCheck(GetUnitOwner()->GetCurrentSpell());
                  return true;
               }
               AdjustCastingPosition(spellToCast, GetUnitOwner()->GetTargetUnit());
            }
            // Since we made it here, we have a legit target that is not ourself or a point very close so pre
            return true;
         }
      }
   }
   // if spell cannot be cast or is invalid
   Stop();
   return false;
}

void AUnitController::PrepareAttack()
{
   // When this is called, we have typically finished moving and turning at the right position and we want
   // to start the attack animation.  We do one last final check and then set readyToAttack to let the
   // attack update (inside the state machine) know we're ready to start the attack animation.
   if(!ownerRef->combatParams.readyToAttack && !ownerRef->IsStunned() && ownerRef->GetTargetUnit()) {
      // check that target is still alive and we can act
      if(ownerRef->GetTargetUnit()->GetCanTarget()) {
         // check that target can even be targetted

         FVector targetLoc   = ownerRef->GetTargetUnit()->GetActorLocation();
         float   attackRange = ownerRef->GetMechanicAdjValue(EMechanics::AttackRange);

         if(IsTargetInRange(attackRange + ownerRef->combatParams.attackRangeCancel, targetLoc)) {
            ownerRef->combatParams.readyToAttack = true;
         } else {
            AdjustPosition(ownerRef->GetMechanicAdjValue(EMechanics::AttackRange), ownerRef->GetTargetUnit());
         }
      } else {
         ownerRef->SetTargetUnit(nullptr);
      }
   } else {
      // necessary or else Attack() will use a nullptr target
      Stop();
   }
}

void AUnitController::TurnTowardsPoint(FVector targetLocation)
{
   startRotation = ownerRef->GetActorRotation().Quaternion();
   turnRotator   = FindLookRotation(targetLocation);
   turnTimeline.SetPlayRate(rotationRate * ownerRef->GetCharacterMovement()->RotationRate.Yaw / FMath::Abs(ownerRef->GetActorQuat().AngularDistance(turnRotator)));
   turnTimeline.PlayFromStart();
}

void AUnitController::TurnTowardsActor(AActor* targetActor)
{
   startRotation = ownerRef->GetActorRotation().Quaternion();
   turnRotator   = FindLookRotation(targetActor->GetActorLocation());
   turnActorTimeline.SetPlayRate(rotationRate * ownerRef->GetCharacterMovement()->RotationRate.Yaw / FMath::Abs(ownerRef->GetActorQuat().AngularDistance(turnRotator)));
   turnActorTimeline.PlayFromStart();
}

void AUnitController::OnActorTurnFinished()
{
   // If we didn't successfully turn towards the actor, try again (this happens if they moved)
   AActor* targetActor = ownerRef->GetTargetActorOrUnit();
   if(!UUpPositionalMoveLibrary::IsFacingTarget(*GetUnitOnwer(), targetActor->GetActorLocation())) {
      TurnTowardsActor(targetActor);
   } else
      onPosAdjDoneAct();
}

void AUnitController::OnPointTurnFinished()
{
   onPosAdjDoneAct();
}

void AUnitController::TurnActor(float turnValue)
{
   // Continually checks to see if actor walked back to face target and ensures there is na actor or unit set
   AActor* targetActor = ownerRef->GetTargetActorOrUnit();
   if(!IsFacingTarget(targetActor->GetActorLocation())) {
      const FQuat rotation = FQuat::Slerp(startRotation, turnRotator, turnValue);
      ownerRef->SetActorRelativeRotation(rotation);
   } else {
      turnActorTimeline.Stop();
      // Call this explicitly because stop does not
      OnActorTurnFinished();
   }
}

void AUnitController::Turn(float turnValue)
{
   const FQuat rotation = FQuat::Slerp(startRotation, turnRotator, turnValue);
   ownerRef->SetActorRelativeRotation(rotation);
}

void AUnitController::QueueTurnAfterMovement(FVector targetLoc)
{
   QueuedTurnAction = [this, &targetLoc]() {
      if(!UUpPositionalMoveLibrary::IsFacingTarget(*GetUnitOwner(), targetLoc))
         TurnTowardsPoint(targetLoc);
      else
         onPosAdjDoneAct();
   };
}

void AUnitController::QueueTurnAfterMovement(AActor* targetActor)
{
   QueuedTurnAction = [this, &targetActor]() {
      if(!UUpPositionalMoveLibrary::IsFacingTarget(*GetUnitOwner(), targetActor->GetActorLocation()))
         TurnTowardsActor(targetActor);
      else
         onPosAdjDoneAct();
   };
}

bool AUnitController::AdjustPosition(float range, FVector targetLoc)
{
   if(!UUpPositionalMoveLibrary::IsTargetInRange(*GetUnitOwner(), targetLoc, range)) {
      MoveToLocation(targetLoc, range);
      QueueTurnAfterMovement(targetLoc);
      return false;
   } else {
      if(!UUpPositionalMoveLibrary::IsFacingTarget(*GetUnitOwner(), targetLoc, .02f)) {
         TurnTowardsPoint(targetLoc);
         return false;
      }
   }
   return true;
}

FVector AUnitController::GetLocation(AActor* targetActor)
{
   return targetActor->GetActorLocation();
}

bool AUnitController::AdjustPosition(float range, AActor* targetActor)
{
   if(!UUpPositionalMoveLibrary::IsTargetInRange(*GetUnitOwner(), GetLocation(targetActor), range)) {
      MoveToActor(targetActor, range);
      QueueTurnAfterMovement(targetActor);
      return false;
   }

   if(!UUpPositionalMoveLibrary::IsFacingTarget(*GetUnitOwner(), GetLocation(targetActor))) {
      TurnTowardsActor(targetActor);
      return false;
   }

   return true;
}

bool AUnitController::AdjustPosition(float range, FVector targetLocation, TFunction<void()>&& finishedTurnAction)
{
   this->onPosAdjDoneAct = MoveTemp(finishedTurnAction);
   return AdjustPosition(range, targetLocation);
}

bool AUnitController::AdjustPosition(float range, AActor* targetActor, TFunction<void()>&& finishedTurnAction)
{
   this->onPosAdjDoneAct = MoveTemp(finishedTurnAction);
   return AdjustPosition(range, targetActor);
}
