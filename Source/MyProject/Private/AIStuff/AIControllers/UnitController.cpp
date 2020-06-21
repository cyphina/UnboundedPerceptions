// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"

#include "WorldObjects/Unit.h"
#include "UnitController.h"

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

void AUnitController::BeginPlay()
{
   Super::BeginPlay();
   CPCRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());

   // Setup AI listening
   if(canProtect)
      protectListener = FAIMessageObserver::Create(this, AIMessage_Help, FOnAIMessage::CreateUObject(this, &AUnitController::Protect));

   if(turnCurve) {
      FOnTimelineFloat turnProgressFunction;
      turnProgressFunction.BindUFunction(this, FName("Turn"));
      turnTimeline.AddInterpFloat(turnCurve, turnProgressFunction);
      turnTimeline.SetTimelineLength(1.f);
      turnTimeline.SetLooping(false);

      FOnTimelineEvent onTimeLineFinished;
      onTimeLineFinished.BindUFunction(this, "ActionAfterTurning");
      turnTimeline.SetTimelineFinishedFunc(onTimeLineFinished);

      FOnTimelineFloat turnActorProgressFunction;
      turnActorProgressFunction.BindUFunction(this, FName("TurnActor"));
      turnActorTimeline.AddInterpFloat(turnCurve, turnActorProgressFunction);
      turnActorTimeline.SetTimelineLength(1.f);
      turnTimeline.SetLooping(false);

      FOnTimelineEvent onTimeLineActorFinished;
      onTimeLineActorFinished.BindUFunction(this, "OnActorTurnFinished");
      turnActorTimeline.SetTimelineFinishedFunc(onTimeLineActorFinished);
   }
}

void AUnitController::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);
   turnTimeline.TickTimeline(deltaSeconds);
   turnActorTimeline.TickTimeline(deltaSeconds);
}

void AUnitController::FindBestAOELocation(bool isSupport)
{
   FEnvQueryRequest queryRequest;
   if(isSupport)
      queryRequest = FEnvQueryRequest(findBestAOESupportLocation, ownerRef);
   else
      queryRequest = FEnvQueryRequest(findBestAOEAssaultLocation, ownerRef);

   // queryRequest.SetFloatParam("CircleRadius", ownerRef->abilities[spellIndex].GetDefaultObject()->GetAOE(ownerRef->GetAbilitySystemComponent()));
   queryRequest.SetFloatParam("SimpleGrid.GridSize", 700);
   queryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &AUnitController::OnAOELocationFound);
}

void AUnitController::FindWeakestTarget(bool isSupport)
{
   FEnvQueryRequest queryRequest;
   if(isSupport)
      queryRequest = FEnvQueryRequest(findWeakestAllyTarget, ownerRef);
   else
      queryRequest = FEnvQueryRequest(findWeakestEnemyTarget, ownerRef);

   queryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &AUnitController::OnWeakestTargetFound);
}

void AUnitController::FindBestSpellTarget(FGameplayTag targettingTag, bool isSupport)
{
   if(targettingTag == FGameplayTag::RequestGameplayTag("Skill.Targetting.Area")) {
      FindBestAOELocation(isSupport);
   } else if(targettingTag == FGameplayTag::RequestGameplayTag("Skill.Targetting.None")) {
      BeginCastSpell(ownerRef->abilities[spellToCastIndex]);
   } else if(targettingTag.MatchesTag(FGameplayTag::RequestGameplayTag("Skill.Targetting.Single"))) {
      FindWeakestTarget(isSupport);
   }
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

void AUnitController::OnAOELocationFound(TSharedPtr<FEnvQueryResult> result)
{
   if(result->IsSuccsessful()) {
      ownerRef->SetTargetLocation(result->GetItemAsLocation(0));
      BeginCastSpell(ownerRef->abilities[spellToCastIndex]);
   } else
      // Aborting means it needs some cleanup while fail just ends the node
      Cast<UBTTaskNode>(behaviorTreeComp->GetActiveNode())->FinishLatentTask(*behaviorTreeComp, EBTNodeResult::Failed);
}

void AUnitController::OnWeakestTargetFound(TSharedPtr<FEnvQueryResult> result)
{
   GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("FOUND WEAKEST TARGET!"));
   if(result.IsValid())
   // Need to actually check here since our filter could end up with us having no heroes to target like if all heroes are full hp
   {
      ownerRef->SetTargetUnit(Cast<AUnit>(result->GetItemAsActor(0)));
      BeginCastSpell(ownerRef->abilities[spellToCastIndex]);
   } else
      Cast<UBTTaskNode>(behaviorTreeComp->GetActiveNode())->FinishLatentTask(*behaviorTreeComp, EBTNodeResult::Failed);
}

FQuat AUnitController::FindLookRotation(FVector targetPoint)
{
   FVector difference         = targetPoint - ownerRef->GetActorLocation();
   FVector projectedDirection = FVector(difference.X, difference.Y, 0);
   return FRotationMatrix::MakeFromX(FVector(projectedDirection)).ToQuat();
}

void AUnitController::Protect(UBrainComponent* BrainComp, const FAIMessage& Message)
{
   // if we figure out that we are needed on the protect job -- abort the task
   Cast<UBTTaskNode>(behaviorTreeComp->GetActiveNode())->FinishLatentTask(*behaviorTreeComp, EBTNodeResult::Failed);
   blackboardComp->SetValueAsBool(blackboardProtectKey, true);
   GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "PROTECTING WITH ME LIFE!");
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
         TurnTowardsTarget(shiftedLocation);
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
   if(ownerRef->targetData.followTarget) {
      MoveActor(ownerRef->targetData.followTarget);
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

void AUnitController::Search()
{
}

void AUnitController::Roam()
{
}

void AUnitController::Run()
{
}

void AUnitController::Chase()
{
   ownerRef->GetUnitController()->AdjustPosition(CHASE_RANGE, ownerRef->GetTargetUnit()->GetActorLocation());
}

bool AUnitController::ChasingQuit()
{
   return true;
}

void AUnitController::Stop()
{
   // Stop will not only stop unit in its track, but will also make it forget its target, and set its state to idle so it can transition to something else
   ownerRef->unitProperties.turnAction.Unbind();
   turnActorTimeline.Stop();
   turnTimeline.Stop();
   ownerRef->SetCurrentSpell(nullptr);
   ownerRef->targetData.ResetTarget();
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
   if(target->GetCanTarget() && !ownerRef->IsStunned()) {
      Stop(); // Stop any other turnActions we're doing
      ownerRef->state->ChangeState(EUnitState::STATE_ATTACKING);
      ownerRef->SetTargetUnit(target);
      ownerRef->unitProperties.turnAction.BindUObject(this, &AUnitController::PrepareAttack);

      //If only we don't have to adjust our position, then attack.  Else adjust position and let callbacks handle what's next
      if(AdjustPosition(ownerRef->GetMechanicAdjValue(EMechanics::AttackRange), target))
         ownerRef->unitProperties.turnAction.Execute();
   }
}

void AUnitController::CastTurnAction()
{
   IncantationCheck(ownerRef->GetCurrentSpell());
}

void AUnitController::AdjustCastingPosition(TSubclassOf<UMySpell> spellClass, FVector spellTargetLocation)
{
   GetUnitOwner()->unitProperties.turnAction.BindUObject(this, &AUnitController::CastTurnAction);
   GetUnitOwner()->state->ChangeState(EUnitState::STATE_CASTING);
   if(AdjustPosition(spellClass.GetDefaultObject()->GetRange(GetUnitOwner()->GetAbilitySystemComponent()), spellTargetLocation))
      IncantationCheck(spellClass);
}

void AUnitController::AdjustCastingPosition(TSubclassOf<UMySpell> spellClass, AActor* spellTargetActor)
{
   GetUnitOwner()->unitProperties.turnAction.BindUObject(this, &AUnitController::CastTurnAction);
   GetUnitOwner()->state->ChangeState(EUnitState::STATE_CASTING);
   if(AdjustPosition(spellClass.GetDefaultObject()->GetRange(GetUnitOwner()->GetAbilitySystemComponent()), spellTargetActor))
      IncantationCheck(spellClass);
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

void AUnitController::AttackMove(FVector moveLocation)
{
   Stop();
   TArray<FVector> targetLoc = {moveLocation};
   Patrol(targetLoc);
}

bool AUnitController::IsTargetInRange(float range, FVector targetLocation)
{
   FVector currentLocation = ownerRef->GetActorLocation();
   FVector difference      = currentLocation - targetLocation;
   difference.Z            = 0;

   if(FVector::DotProduct(difference, difference) <= range * range)
      return true;
   return false;
}

bool AUnitController::IsFacingTarget(FVector targetLocation, float angleErrorCutoff)
{
   // Lets ensure the vector between our location and the target location is close to the same direction we're facing
   FVector difference = (targetLocation - ownerRef->GetActorLocation()).GetSafeNormal2D();
   float   dot        = FVector::DotProduct(ownerRef->GetActorForwardVector(), difference);

   if(dot > 1.f - angleErrorCutoff) // .05 is 18 degrees lenient by default (only from right side).
      return true;
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

void AUnitController::TurnTowardsTarget(FVector targetLocation)
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
   if(!IsFacingTarget(targetActor->GetActorLocation())) {
      TurnTowardsActor(targetActor);
   } else
      ownerRef->unitProperties.turnAction.ExecuteIfBound();
}

void AUnitController::ActionAfterTurning()
{
   ownerRef->unitProperties.turnAction.ExecuteIfBound();
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

bool AUnitController::AdjustPosition(float range, FVector targetLoc)
{
   if(!IsTargetInRange(range, targetLoc)) {
      MoveToLocation(targetLoc, range);
      return false;
   } else {
      if(!IsFacingTarget(targetLoc, .02f)) {
         TurnTowardsTarget(targetLoc);
         return false;
      }
   }
   return true;
}

bool AUnitController::AdjustPosition(float range, AActor* targetActor)
{
   // While moving to the actor, our rotation will be controlled through movement so don't force a rotation until after we are in position
   // Now after we're in position, we rotate using OnMoveCompleted() as our final rotation check
   FVector targetActorLocation = targetActor->GetActorLocation();
   if(!IsTargetInRange(range, targetActorLocation)) {
      MoveToActor(targetActor, range);
      return false;
   } else {
      // If we're already in position, turn towards the target.
      if(!IsFacingTarget(targetActorLocation)) {
         TurnTowardsActor(targetActor);
         return false;
      }
   }
   return true;
}

bool AUnitController::AdjustPosition(float range, FVector targetLocation, EUnitState newState, TFunction<void()> turnAction)
{
   GetUnitOwner()->state->ChangeState(newState);
   GetUnitOwner()->unitProperties.turnAction.BindLambda(turnAction);
   return AdjustPosition(range, targetLocation);
}

bool AUnitController::AdjustPosition(float range, AActor* targetActor, EUnitState newState, TFunction<void()> turnAction)
{
   GetUnitOwner()->state->ChangeState(newState);
   GetUnitOwner()->unitProperties.turnAction.BindLambda(turnAction);
   return AdjustPosition(range, targetActor);
}

void AUnitController::IncantationCheck(TSubclassOf<UMySpell> spellToCast) const
{
   float castTime = spellToCast.GetDefaultObject()->GetCastTime(ownerRef->GetAbilitySystemComponent());
   if(UNLIKELY(!castTime)) // If there isn't a cast time
      ownerRef->CastSpell(spellToCast);
   else {
      ownerRef->unitSpellData.channelTime = castTime;
      ownerRef->state->ChangeState(EUnitState::STATE_INCANTATION); //Start channeling
   }
}

void AUnitController::SpellChanneling(TSubclassOf<UMySpell> spellToCast)
{
}
