<<<<<<< HEAD
#include "MyProject.h"

#include "WorldObjects/Unit.h"

#include "UserInput.h"
=======

#include "MyProject.h"

#include "WorldObjects/Unit.h"
>>>>>>> componentrefactor

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BasePlayer.h"
#include "GameplayDelegateContext.h"

#include "SpellSystem/MySpell.h"
#include "PatrolComponent.h"
#include "TargetComponent.h"
#include "UpStatComponent.h"
<<<<<<< HEAD

#include "MoveVisitorDefs.inl"
#include "RTSAttackExecution.h"
#include "RTSDeathExecution.h"
#include "RTSStateComponent.h"
#include "SpellDataLibrary.h"

AUnitController::AUnitController()
=======

#include "MoveVisitorDefs.inl"
#include "RTSAttackExecution.h"
#include "RTSDeathExecution.h"
#include "RTSIngameWidget.h"
#include "RTSMoveExecution.h"

#include "RTSStateComponent.h"
#include "SpellDataLibrary.h"
#include "UnitMessages.h"
#include "UserInput.h"
#include "Navigation/CrowdFollowingComponent.h"

AUnitController::AUnitController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
>>>>>>> componentrefactor
{
   SetActorTickInterval(0.f);
   ConstructorHelpers::FObjectFinder<UCurveFloat> loadedCurve(TEXT("/Game/RTS_Tutorial/HUDs/ActionUI/StandardLinear"));
   if(loadedCurve.Succeeded()) turnCurve = loadedCurve.Object;
}

void AUnitController::OnPossess(APawn* InPawn)
{
   Super::OnPossess(InPawn);
   ownerRef = Cast<AUnit>(GetPawn());
   GetUnitOwner()->OnUnitDamageReceived().AddUObject(this, &AUnitController::OnDamageReceived);
   GetUnitOwner()->OnUnitHealingReceived().AddUObject(this, &AUnitController::OnHealingReceived);
}

void AUnitController::SetupTurnPointTimeline()
{
   FOnTimelineFloat turnProgressFunction;
   turnProgressFunction.BindUFunction(this, FName("Turn"));
   turnTimeline.AddInterpFloat(turnCurve, turnProgressFunction);
   turnTimeline.SetTimelineLength(1.f);
   turnTimeline.SetLooping(false);
<<<<<<< HEAD

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

=======

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

>>>>>>> componentrefactor
   FOnTimelineEvent onTimeLineActorFinished;
   onTimeLineActorFinished.BindUFunction(this, "OnActorTurnFinished");
   turnActorTimeline.SetTimelineFinishedFunc(onTimeLineActorFinished);
}

void AUnitController::BeginPlay()
{
   Super::BeginPlay();

<<<<<<< HEAD
   if(turnCurve) {
      SetupTurnPointTimeline();
      SetupTurnActorTimeline();
   }

   GetUnitOwner()->OnUnitDamageReceived().AddUObject(this, &AUnitController::OnDamageReceived);
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
   if(Result.IsSuccess()) { queuedTurnAction(); }
}

void AUnitController::Attack()
{
   customAttackLogic.GetDefaultObject()->Execute();
}

void AUnitController::OnDamageReceived(const FUpDamage& d)
{
   GetUnitOwner()->GetStatComponent()->ModifyStats<false>(GetUnitOwner()->GetStatComponent()->GetVitalCurValue(EVitals::Health) - d.damage, EVitals::Health);
   if(GetUnitOwner()->GetStatComponent()->GetVitalAdjValue(EVitals::Health) < 0) { Die(); }
}

void AUnitController::Die()
{
   customDeathLogic.GetDefaultObject()->Execute();
   GetUnitOwner()->SetSelected(false);
   GetUnitOwner()->OnUnitDie().Broadcast();
   // Eventually this object will get GC'd. If we have something like resurrection, store unit data in the OnUnitDieEvent as opposed to keeping around a deactivated copy.
   SetLifeSpan(5.f);
=======
   if(turnCurve)
   {
      SetupTurnPointTimeline();
      SetupTurnActorTimeline();
   }

   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UGameplayDelegateContext>()->OnUnitDieGlobal().AddWeakLambda(this, [this](AUnit* deadUnit) {
      UTargetComponent* targetComp = GetUnitOwner()->GetTargetComponent();
      if(targetComp->IsTargetingUnit())
      {
         if(deadUnit == targetComp->GetTargetUnit())
         {
            targetComp->ResetTarget();
            if(GetState() != EUnitState::STATE_IDLE)
            {
               StopCurrentAction();
            }
         }
      }
   });
}

void AUnitController::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);
   turnTimeline.TickTimeline(deltaSeconds);
   turnActorTimeline.TickTimeline(deltaSeconds);

   // TODO: Probably move this to its own component
   // Process only at the end of the frame so if we get toggled to idle in between actions it won't activate the queue.
   if(!commandQueue.IsEmpty() && !GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::LeftShift) && GetState() == EUnitState::STATE_IDLE)
   {
      TFunction<void()> command;
      commandQueue.Dequeue(command);
      --queueCount;
      command();
   }
>>>>>>> componentrefactor
}

EUnitState AUnitController::GetState() const
{
<<<<<<< HEAD
   if(USpellDataLibrary::IsStunned(GetUnitOwner()->GetAbilitySystemComponent())) {
      Stop();
      // Shift location a little bit if we're moving multiple units so they can group together ok
      const FVector shiftedLocation = newLocation - ownerRef->GetActorLocation().GetSafeNormal() * ownerRef->GetCapsuleComponent()->GetScaledCapsuleRadius() / 2;
      ownerRef->GetTargetComponent()->SetTarget(shiftedLocation);
      const EPathFollowingRequestResult::Type result = MoveToLocation(shiftedLocation, 10, true, true, false, true);
      if(result == EPathFollowingRequestResult::RequestSuccessful) {
         ownerRef->FindComponentByClass<URTSStateComponent>()->ChangeState(EUnitState::STATE_MOVING);
      } else
         TurnTowardsPoint(shiftedLocation);
      return result;
=======
   if(URTSStateComponent* stateComp = FindComponentByClass<URTSStateComponent>())
   {
      return stateComp->GetState();
>>>>>>> componentrefactor
   }
   return EUnitState::STATE_IDLE;
}

void AUnitController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
<<<<<<< HEAD
   if(!USpellDataLibrary::IsStunned(GetUnitOwner()->GetAbilitySystemComponent())) {
      Stop();
      ownerRef->GetTargetComponent()->SetTarget(targetActor);
      EPathFollowingRequestResult::Type result = MoveToActor(targetActor);

      if(result == EPathFollowingRequestResult::RequestSuccessful) { ownerRef->FindComponentByClass<URTSStateComponent>()->ChangeState(EUnitState::STATE_MOVING); }
      return result;
=======
   Super::OnMoveCompleted(RequestID, Result);

   // Check to see if we need to turn towards a point or a target after we're done moving.
   // Moving naturally rotates us to our desired point/target, but it will stop prematurely if we're in range.
   // We only do this on success because if the action is canceled we don't want us to keep going.
   if(Result.IsSuccess())
   {
      if(queuedTurnAction)
      {
         queuedTurnAction();
      }
>>>>>>> componentrefactor
   }
}

<<<<<<< HEAD
void AUnitController::Stop()
{
   OnUnitStoppedEvent.Broadcast();
   onPosAdjDoneAct  = nullptr;
   queuedTurnAction = nullptr;
   turnActorTimeline.Stop();
   turnTimeline.Stop();
   ownerRef->GetTargetComponent()->ResetTarget();
   StopMovement();
   ownerRef->StopAnimMontage();
=======
void AUnitController::Attack()
{
   GetUnitOwner()->GetAbilitySystemComponent()->TryActivateAbilityByClass(GetUnitOwner()->GetCustomAttackLogic());
}

void AUnitController::OnDamageReceived(const FUpDamage& d)
{
   if(!d.DidMiss())
   {
      d.sourceUnit->GetCombatInfo()->bMissLastHit = false;
      GetUnitOwner()->GetStatComponent()->ModifyStats<false>(GetUnitOwner()->GetStatComponent()->GetVitalCurValue(EVitals::Health) - d.damage, EVitals::Health);

      if(d.effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Lifesteal")))
      {
         d.sourceUnit->GetStatComponent()->ModifyStats<false>(d.targetUnit->GetStatComponent()->GetVitalCurValue(EVitals::Health) + d.damage, EVitals::Health);
      }

      if(GetUnitOwner()->GetStatComponent()->GetVitalAdjValue(EVitals::Health) <= 0)
      {
         Die();
      }
   }
   else
   {
      d.sourceUnit->GetCombatInfo()->bMissLastHit = true;
   }
}

void AUnitController::OnHealingReceived(const FUpDamage& d)
{
   GetUnitOwner()->GetStatComponent()->ModifyStats<false>(GetUnitOwner()->GetStatComponent()->GetVitalCurValue(EVitals::Health) + d.damage, EVitals::Health);
}

void AUnitController::Die()
{
   GetUnitOwner()->GetAbilitySystemComponent()->TryActivateAbilityByClass(GetUnitOwner()->GetCustomDeathLogic());
   GetUnitOwner()->SetUnitSelected(false);
   GetUnitOwner()->SetEnabled(false);
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UGameplayDelegateContext>()->OnUnitDieGlobal().Broadcast(GetUnitOwner());

   FGameplayEffectQuery removeAllEffectsQuery;
   removeAllEffectsQuery.EffectSource = this;
   GetUnitOwner()->GetAbilitySystemComponent()->RemoveActiveEffects(removeAllEffectsQuery);

   GetUnitOwner()->OnUnitDie().Broadcast();
   // Eventually this object will get GC'd. If we have something like resurrection, store unit data in the OnUnitDieEvent as opposed to keeping around a deactivated copy.
   // Also we need to destroy this unit in a delay else our vision system in the other thread will have some data races.
   SetLifeSpan(5.f);
}

void AUnitController::PauseCurrentMovement()
{
   PauseMove(GetCurrentMoveRequestID());
}

void AUnitController::ResumeCurrentMovement()
{
   ResumeMove(GetCurrentMoveRequestID());
>>>>>>> componentrefactor
}

EPathFollowingRequestResult::Type AUnitController::Move(FVector newLocation, float stopRange)
{
<<<<<<< HEAD
   if(UBehaviorTreeComponent* behaviorTreeComp = FindComponentByClass<UBehaviorTreeComponent>()) {
      if(behaviorTreeComp->IsRunning()) behaviorTreeComp->StopTree();
=======
   EPathFollowingRequestResult::Type moveRequestResult = EPathFollowingRequestResult::Type::Failed;

   if(!USpellDataLibrary::IsStunned(GetUnitOwner()->GetAbilitySystemComponent()))
   {
      if(ABasePlayer* basePlayer = GetWorld()->GetFirstPlayerController()->GetPlayerState<ABasePlayer>())
      {
         FVector shiftedLocation = newLocation; // Shift location a little bit if we're moving multiple units so they can group together ok
         if(basePlayer->GetSelectedUnits().Num() > 1)
         {
            shiftedLocation = newLocation - ownerRef->GetActorLocation().GetSafeNormal() * ownerRef->GetCapsuleComponent()->GetScaledCapsuleRadius() / 2;
         }

         ownerRef->GetTargetComponent()->SetTarget(shiftedLocation);
         if(!AdjustPosition(stopRange, shiftedLocation, moveRequestResult))
         {
            if(URTSStateComponent* stateComponent = FindComponentByClass<URTSStateComponent>())
            {
               stateComponent->ChangeState(EUnitState::STATE_MOVING);
            }
         }
      }
>>>>>>> componentrefactor
   }
   return moveRequestResult;
}

<<<<<<< HEAD
=======
EPathFollowingRequestResult::Type AUnitController::MoveActor(AActor* targetActor, float stopRange)
{
   EPathFollowingRequestResult::Type moveRequestResult = EPathFollowingRequestResult::Type::Failed;
   if(!USpellDataLibrary::IsStunned(GetUnitOwner()->GetAbilitySystemComponent()))
   {
      StopCurrentAction();
      ownerRef->GetTargetComponent()->SetTarget(targetActor);

      if(!AdjustPosition(stopRange, targetActor, moveRequestResult))
      {
         if(URTSStateComponent* stateComponent = FindComponentByClass<URTSStateComponent>())
         {
            stateComponent->ChangeState(EUnitState::STATE_MOVING);
         }
      }
   }
   return moveRequestResult;
}

void AUnitController::StopCurrentAction()
{
   OnUnitStoppedEvent.Broadcast();
   onPosAdjDoneAct  = nullptr;
   queuedTurnAction = nullptr;
   turnActorTimeline.Stop();
   turnTimeline.Stop();
   ownerRef->StopAnimMontage();
}

void AUnitController::FinishCurrentAction()
{
   if(URTSStateComponent* stateComp = FindComponentByClass<URTSStateComponent>())
   {
      stateComp->ChangeState(EUnitState::STATE_IDLE);
   }

   const FAIMessage msg(UnitMessages::AIMessage_ActionFinished, this);
   FAIMessage::Send(this, msg);
}

void AUnitController::HaltUnit()
{
   StopCurrentAction();
   ClearCommandQueue();
   StopAutomation();
}

void AUnitController::StopAutomation() const
{
   if(UBehaviorTreeComponent* behaviorTreeComp = FindComponentByClass<UBehaviorTreeComponent>())
   {
      if(behaviorTreeComp->IsRunning())
      {
         behaviorTreeComp->StopTree();
      }
   }
}

>>>>>>> componentrefactor
void AUnitController::TurnTowardsPoint(FVector targetLocation)
{
   StopMovement();
   startRotation = ownerRef->GetActorRotation().Quaternion();
   turnRotator   = UUpAIHelperLibrary::FindLookRotation(GetUnitOwner(), targetLocation);
   turnTimeline.SetPlayRate(rotationRate * ownerRef->GetCharacterMovement()->RotationRate.Yaw / FMath::Abs(ownerRef->GetActorQuat().AngularDistance(turnRotator)));
   turnTimeline.PlayFromStart();
}

void AUnitController::TurnTowardsActor(AActor* targetActor)
{
   StopMovement();
   startRotation = ownerRef->GetActorRotation().Quaternion();
   turnRotator   = UUpAIHelperLibrary::FindLookRotation(GetUnitOwner(), targetActor->GetActorLocation());
   turnActorTimeline.SetPlayRate(rotationRate * ownerRef->GetCharacterMovement()->RotationRate.Yaw / FMath::Abs(ownerRef->GetActorQuat().AngularDistance(turnRotator)));
   turnActorTimeline.PlayFromStart();
}

void AUnitController::OnActorTurnFinished()
{
   // If we didn't successfully turn towards the actor, try again (this happens if they moved)
   AActor* targetActor = ownerRef->GetTargetComponent()->GetTargetActorOrUnit();
<<<<<<< HEAD
   if(!UUpAIHelperLibrary::IsFacingTarget(GetUnitOwner(), targetActor->GetActorLocation())) {
      TurnTowardsActor(targetActor);
   } else
      onPosAdjDoneAct();
=======
   if(targetActor)
   {
      if(!UUpAIHelperLibrary::IsFacingTarget(GetUnitOwner(), targetActor->GetActorLocation()))
      {
         TurnTowardsActor(targetActor);
      }
      else
      {
         if(onPosAdjDoneAct)
         {
            onPosAdjDoneAct();
         }

         const FAIMessage msg(UnitMessages::AIMessage_TurnFinished, this);
         FAIMessage::Send(this, msg);
      }
   }
>>>>>>> componentrefactor
}

void AUnitController::OnPointTurnFinished()
{
<<<<<<< HEAD
   onPosAdjDoneAct();
=======
   if(onPosAdjDoneAct)
   {
      onPosAdjDoneAct();
   }

   const FAIMessage msg(UnitMessages::AIMessage_TurnFinished, this);
   FAIMessage::Send(this, msg);
>>>>>>> componentrefactor
}

void AUnitController::TurnActor(float turnValue)
{
<<<<<<< HEAD
   if(AActor* targetActor = ownerRef->GetTargetComponent()->GetTargetActorOrUnit()) {
      if(!UUpAIHelperLibrary::IsFacingTarget(GetUnitOwner(), targetActor->GetActorLocation())) {
         const FQuat rotation = FQuat::Slerp(startRotation, turnRotator, turnValue);
         ownerRef->SetActorRelativeRotation(rotation);
      } else {
=======
   if(AActor* targetActor = GetUnitOwner()->GetTargetComponent()->GetTargetActorOrUnit())
   {
      if(!UUpAIHelperLibrary::IsFacingTarget(GetUnitOwner(), targetActor->GetActorLocation()))
      {
         const FQuat rotation = FQuat::Slerp(startRotation, turnRotator, turnValue);
         ownerRef->SetActorRelativeRotation(rotation);
      }
      else
      {
>>>>>>> componentrefactor
         turnActorTimeline.Stop();
         // Call this explicitly because stop does not trigger the timeline finished event.
         OnActorTurnFinished();
      }
   }
}

void AUnitController::Turn(float turnValue)
{
   const FQuat rotation = FQuat::Slerp(startRotation, turnRotator, turnValue);
   ownerRef->SetActorRelativeRotation(rotation);
}

void AUnitController::QueueTurnAfterMovement(FVector targetLoc)
{
<<<<<<< HEAD
   queuedTurnAction = [this, &targetLoc]() {
      if(!UUpAIHelperLibrary::IsFacingTarget(GetUnitOwner(), targetLoc))
         TurnTowardsPoint(targetLoc);
      else
         onPosAdjDoneAct();
=======
   queuedTurnAction = [this, targetLoc]() {
      if(!UUpAIHelperLibrary::IsFacingTarget(GetUnitOwner(), targetLoc))
         TurnTowardsPoint(targetLoc);
      else
      {
         if(onPosAdjDoneAct)
         {
            onPosAdjDoneAct();
         }
      }
>>>>>>> componentrefactor
   };
}

void AUnitController::QueueTurnAfterMovement(AActor* targetActor)
{
<<<<<<< HEAD
   queuedTurnAction = [this, &targetActor]() {
      if(!UUpAIHelperLibrary::IsFacingTarget(GetUnitOwner(), targetActor->GetActorLocation()))
         TurnTowardsActor(targetActor);
      else
         onPosAdjDoneAct();
=======
   queuedTurnAction = [this, targetActor]() {
      if(!UUpAIHelperLibrary::IsFacingTarget(GetUnitOwner(), targetActor->GetActorLocation()))
      {
         TurnTowardsActor(targetActor);
      }
      else
      {
         if(onPosAdjDoneAct)
         {
            onPosAdjDoneAct();
         }
      }
>>>>>>> componentrefactor
   };
}

bool AUnitController::AdjustPosition(float range, FVector targetLoc)
{
<<<<<<< HEAD
   if(!UUpAIHelperLibrary::IsTargetInRange(GetUnitOwner(), targetLoc, range)) {
      MoveToLocation(targetLoc, range);
      QueueTurnAfterMovement(targetLoc);
      return false;
   } else {
      if(!UUpAIHelperLibrary::IsFacingTarget(GetUnitOwner(), targetLoc, .02f)) {
         TurnTowardsPoint(targetLoc);
         return false;
      }
=======
   if(!UUpAIHelperLibrary::IsTargetInRange(GetUnitOwner(), targetLoc, range))
   {
      MoveToLocation(targetLoc, range, false, true, false, true);
      QueueTurnAfterMovement(targetLoc);
      return false;
   }

   StopMovement();

   if(!UUpAIHelperLibrary::IsFacingTarget(GetUnitOwner(), targetLoc, .02f))
   {
      TurnTowardsPoint(targetLoc);
      return false;
>>>>>>> componentrefactor
   }
   return true;
}

bool AUnitController::AdjustPosition(float range, FVector targetLoc, EPathFollowingRequestResult::Type& outPathReqRes)
{
<<<<<<< HEAD
   if(!UUpAIHelperLibrary::IsTargetInRange(GetUnitOwner(), targetActor->GetActorLocation(), range)) {
      MoveToActor(targetActor, range);
      QueueTurnAfterMovement(targetActor);
      return false;
   }

   if(!UUpAIHelperLibrary::IsFacingTarget(GetUnitOwner(), targetActor->GetActorLocation())) {
      TurnTowardsActor(targetActor);
      return false;
   }

   return true;
}

bool AUnitController::AdjustPosition(const float range, FVector targetLocation, TFunction<void()> finishedTurnAction)
{
   this->onPosAdjDoneAct = MoveTemp(finishedTurnAction);
   if(AdjustPosition(range, targetLocation)) {
=======
   this->onPosAdjDoneAct = [this]() {
      FinishCurrentAction();
   };

   if(!UUpAIHelperLibrary::IsTargetInRange(GetUnitOwner(), targetLoc, range))
   {
      outPathReqRes = MoveToLocation(targetLoc, range, false, true, false, false, nullptr, true);
      QueueTurnAfterMovement(targetLoc);
      return false;
   }

   StopMovement();

   if(!UUpAIHelperLibrary::IsFacingTarget(GetUnitOwner(), targetLoc, .02f))
   {
      TurnTowardsPoint(targetLoc);
      return false;
   }

   onPosAdjDoneAct();
   return true;
}

bool AUnitController::AdjustPosition(float range, AActor* targetActor)
{
   // The move functionality automatically accounts for the target actor radius and the moving actor's radius (See UPathFollowingComponent::HasReachedInternal), but we have to add that into our own manual range check
   if(!UUpAIHelperLibrary::IsTargetInRangeOfActor(GetUnitOwner(), targetActor, range))
   {
      const AActor* previousMoveGoal = GetPathFollowingComponent()->GetMoveGoal();
      if(!previousMoveGoal || previousMoveGoal != targetActor)
      {
         MoveToActor(targetActor, range, false, true, false);
         QueueTurnAfterMovement(targetActor);
      }
      return false;
   }

   StopMovement();

   if(!UUpAIHelperLibrary::IsFacingTarget(GetUnitOwner(), targetActor->GetActorLocation()))
   {
      TurnTowardsActor(targetActor);
      return false;
   }

   return true;
}

bool AUnitController::AdjustPosition(float range, AActor* targetActor, EPathFollowingRequestResult::Type& outPathReqRes)
{
   // The move functionality automatically accounts for the target actor radius and the moving actor's radius (See UPathFollowingComponent::HasReachedInternal), but we have to add that into our own manual range check
   if(!UUpAIHelperLibrary::IsTargetInRangeOfActor(GetUnitOwner(), targetActor, range))
   {
      const AActor* previousMoveGoal = GetPathFollowingComponent()->GetMoveGoal();
      if(!previousMoveGoal || previousMoveGoal != targetActor)
      {
         outPathReqRes = MoveToActor(targetActor, range, false, true, false);
         QueueTurnAfterMovement(targetActor);
      }
      return false;
   }

   StopMovement();

   if(!UUpAIHelperLibrary::IsFacingTarget(GetUnitOwner(), targetActor->GetActorLocation()))
   {
      TurnTowardsActor(targetActor);
      return false;
   }

   return true;
}

bool AUnitController::AdjustPosition(const float range, FVector targetLocation, TFunction<void()> finishedTurnAction)
{
   this->onPosAdjDoneAct = MoveTemp(finishedTurnAction);
   if(AdjustPosition(range, targetLocation))
   {
      onPosAdjDoneAct();
      return true;
   }
   return false;
}

bool AUnitController::AdjustPosition(const float range, AActor* targetActor, TFunction<void()> finishedTurnAction)
{
   this->onPosAdjDoneAct = MoveTemp(finishedTurnAction);
   if(AdjustPosition(range, targetActor))
   {
>>>>>>> componentrefactor
      onPosAdjDoneAct();
      return true;
   }
   return false;
}

<<<<<<< HEAD
bool AUnitController::AdjustPosition(const float range, AActor* targetActor, TFunction<void()> finishedTurnAction)
{
   this->onPosAdjDoneAct = MoveTemp(finishedTurnAction);
   if(AdjustPosition(range, targetActor)) {
      onPosAdjDoneAct();
      return true;
   }
   return false;
=======
void AUnitController::QueueAction(TFunction<void()> actionToQueue)
{
   if(queueCount < 20)
   {
      GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Emerald, TEXT("SUCESSFUL QUEUE"));
      commandQueue.Enqueue(actionToQueue);
      ++queueCount;
   }
   else
   {
      URTSIngameWidget::NativeDisplayHelpText(GetWorld(), FILLED_QUEUE_TEXT);
   }
>>>>>>> componentrefactor
}
