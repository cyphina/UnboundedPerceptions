// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Ally.h"
#include "SpellSystem/MySpell.h"
#include "RTSGameState.h"
#include "RTSPawn.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "UI/HUDManager.h"
#include "UI/UserWidgets/ActionbarInterface.h"
#include "UI/UserWidgets/MainWidget.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AIStuff/AIControllers/AllyAIController.h"
#include "Enemy.h"
#include "UpResourceManager.h"
#include "InteractableBase.h"

DECLARE_CYCLE_STAT(TEXT("Ally Vision"), STAT_AllyVision, STATGROUP_RTSUnits)

FText const AAlly::notEnoughManaText = NSLOCTEXT("HelpMessages", "Mana", "Not Enough Mana!");
FText const AAlly::invalidTargetText = NSLOCTEXT("HelpMessages", "Target", "Invalid Target!");
FText const AAlly::onCooldownText    = NSLOCTEXT("HelpMessages", "CD", "Spell is on Cooldown!");
FText const AAlly::stunnedText       = NSLOCTEXT("HelpMessages", "Stun", "Currently Stunned!");
FText const AAlly::filledQueueText   = NSLOCTEXT("HelpMessages", "Queue", "Command Queue Filled!");

AAlly::AAlly(const FObjectInitializer& oI) : AUnit(oI)
{
   state = TUniquePtr<StateMachine>(new StateMachine(this));

   combatParams.isEnemy = false;

   abilities.SetNum(MAX_NUM_SPELLS); // Size of abilities that can be used on actionbar

   // Set collision to friendly type and block traces made on the AllyTrace channel
   GetCapsuleComponent()->SetCollisionProfileName("Ally");
   visionSphere->SetCollisionProfileName("FriendlyVision");
   visionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAlly::OnVisionSphereOverlap);
   visionSphere->OnComponentEndOverlap.AddDynamic(this, &AAlly::OnVisionSphereEndOverlap);

   GetMesh()->CustomDepthStencilValue = 254;

#if UE_EDITOR
   visionSphere->SetHiddenInGame(false);
#endif

   AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAlly::BeginPlay()
{
   Super::BeginPlay();

   queryParamVision.AddObjectTypesToQuery(ECC_GameTraceChannel6); // VisionBlockers
   fadeChannel = ECollisionChannel::ECC_GameTraceChannel12;       // FadeTrace
}

void AAlly::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);

   if (!controllerRef->IsInputKeyDown(EKeys::LeftShift) && GetState() == EUnitState::STATE_IDLE && !commandQueue.IsEmpty()) {
      // GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString(TEXT("Command Dequeued")));
      TFunction<void()> command;
      commandQueue.Dequeue(command);
      --queueCount;
      command();
   }
}

void AAlly::EndPlay(const EEndPlayReason::Type eReason)
{
   Super::EndPlay(eReason);
   // Errors when we stop play in editor since the vision check keeps going on even after these actors are destroyed
}

void AAlly::PossessedBy(AController* newAllyControllerRef)
{
   Super::PossessedBy(newAllyControllerRef);
   allyController = Cast<AAllyAIController>(newAllyControllerRef);
}

void AAlly::SetEnabled(bool bEnabled)
{
   Super::SetEnabled(bEnabled);
   if (bEnabled) {
      controllerRef->GetBasePlayer()->allies.Add(this);
      controllerRef->GetGameState()->allyList.Add(this);
   } else {
      controllerRef->GetBasePlayer()->allies.Remove(this);
      controllerRef->GetGameState()->allyList.Remove(this);
   }
}

void AAlly::Die_Implementation()
{
   // Allies that aren't heroes can get destroyed when they die
   Super::Die_Implementation();
   controllerRef->GetBasePlayer()->allies.Remove(this);
   Destroy();
}

void AAlly::Attack_Implementation()
{
   Super::Attack_Implementation();
   // If they die (when this current attack kills them) and the targets get canceled out, then targetUnit can be nulled
   if (IsValid(targetData.targetUnit))
      if (!targetData.targetUnit->IsVisible()) GetAllyAIController()->Stop();
}

void AAlly::SetSelected(bool value)
{
   Super::SetSelected(value);
   if (value) {
      controllerRef->GetBasePlayer()->selectedAllies.AddUnique(this);
   } else {
      controllerRef->GetBasePlayer()->selectedAllies.Remove(this);
   }
}

void AAlly::SetSpellIndex(int index)
{
#if UE_EDITOR
   check(index >= 0 && index < MAX_NUM_SPELLS)
#endif
       spellIndex = index;
}

UGameplayAbility* AAlly::GetSpellInstance(TSubclassOf<UMySpell> spellClass) const
{
   if (spellClass) {
      if (GetAbilitySystemComponent()->FindAbilitySpecFromClass(spellClass)->Ability) return GetAbilitySystemComponent()->FindAbilitySpecFromClass(spellClass)->GetAbilityInstances()[0];
   }
   return nullptr;
}

bool AAlly::CastSpell(TSubclassOf<UMySpell> spellToCast)
{
   bool invis = GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Invisibility"));
   if (Super::CastSpell(spellToCast)) {
      if (controllerRef->GetBasePlayer()->focusedUnit == this) {
         controllerRef->GetHUDManager()->GetActionHUD()->ShowSkillVisualCD(spellIndex);
         // Cancel AI targetting if enemy turns invisible
         if (IsValid(targetData.targetUnit) && !gameState->visibleEnemies.Contains(targetData.targetUnit)) GetAllyAIController()->Stop();
         // Reveal self if invisile when spell casted.  If we don't check this before spell casted, we could just end up canceling an invisibility spell being cast
         if (invis) GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Effect.Invisibility")));
         return true;
      }
   }
   return false;
}

float AAlly::CalculateTargetRisk()
{
   int targetNum = 0;
   for (AUnit* e : controllerRef->GetGameState()->visibleEnemies) {
      if (e->GetTarget() == this) ++targetNum;
   }
   const float targetRiskValue = FMath::Clamp(diminishFunc(targetNum), 0.f, 1.f);
   return targetRiskValue;
}

bool AAlly::GetOverlappingObjects(TArray<FHitResult>& hits)
{
   // Sweep from box around hero to camera location
   FVector start = controllerRef->GetCameraPawn()->camera->GetComponentLocation();
   FVector end   = GetActorLocation();

   //// For debugging trace
   // const FName TraceTag("MyTraceTag");
   // GetWorld()->DebugDrawTraceTag = TraceTag;
   // FCollisionQueryParams params;
   // params.TraceTag = TraceTag;

   FQuat cameraRotation = controllerRef->GetCameraPawn()->camera->GetComponentQuat();
   cameraRotation.W *= -1;
   GetWorld()->SweepMultiByChannel(hits, start, end, cameraRotation, fadeChannel, FCollisionShape::MakeBox(FVector(200, 200, 100)));
   if (!hits.Num()) return false;
   return true;
}

void AAlly::QueueAction(TFunction<void()> actionToQueue)
{
   if (queueCount < 20) {
      GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Emerald, TEXT("SUCESSFUL QUEUE"));
      commandQueue.Enqueue(actionToQueue);
      ++queueCount;
   } else
      controllerRef->GetHUDManager()->GetMainHUD()->DisplayHelpText(filledQueueText);
}

void AAlly::OnVisionSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int otherBodyIndex, bool fromSweep, const FHitResult& sweepRes)
{
   switch (otherActor->GetRootComponent()->GetCollisionObjectType()) {
      case ECC_GameTraceChannel6: {
         GetCornersInRange(otherActor);
         break;
      }
      case ECollisionChannel::ECC_GameTraceChannel2: {
#if UE_EDITOR
         checkf(otherActor->GetClass()->IsChildOf(AEnemy::StaticClass()), TEXT("Actor %s is not a valid enemy"), *otherActor->GetName());
#endif
         AUnit* enemy = Cast<AUnit>(otherActor);
         // TODO: Check for invisibility
         possibleEnemiesInRadius.Add(enemy);
         enemy->IncVisionCount();
      }
      default: break;
   }
}

void AAlly::OnVisionSphereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
   switch (otherActor->GetRootComponent()->GetCollisionObjectType()) {
      case ECC_GameTraceChannel6: {
         RemoveCornersInRange(otherActor);
         break;
      }
      case ECollisionChannel::ECC_GameTraceChannel2: {
         AUnit* enemy = Cast<AUnit>(otherActor);
         possibleEnemiesInRadius.Remove(enemy);
         enemy->DecVisionCount();
         if (!enemy->GetVisionCount()) {
            enemy->GetCapsuleComponent()->SetVisibility(false, true);
            enemy->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel7, ECollisionResponse::ECR_Ignore);
            gameState->visibleEnemies.Remove(enemy);
         }
         break;
      }
      default: break;
   }
}

void AAlly::GetCornersInRange(AActor* overlapActor)
{
   FVector origin = FVector();
   FVector extent = FVector();
   overlapActor->GetActorBounds(true, origin, extent);
   for (FVector v : UpResourceManager::BoundsPointMapping2D) {
      FVector corner     = origin + extent * v;
      FVector cornerDist = corner - GetActorLocation();
      visionBlockerCorners.Add(corner);
   }
}

void AAlly::RemoveCornersInRange(AActor* overlapEndActor)
{
   FVector origin = FVector();
   FVector extent = FVector();
   overlapEndActor->GetActorBounds(true, origin, extent);
   for (FVector v : UpResourceManager::BoundsPointMapping2D) {
      FVector corner     = origin + extent * v;
      FVector cornerDist = corner - GetActorLocation();
      visionBlockerCorners.Remove(corner);
   }
}

void AAlly::FindVisibilityPoints()
{
   SCOPE_CYCLE_COUNTER(STAT_AllyVision)
   {
      auto pred = [this](const FVector& a, const FVector& b) { return UpResourceManager::FindOrientation(a - GetActorLocation()) > UpResourceManager::FindOrientation(b - GetActorLocation()); };
      static const float root2Over2 = .70710f;

      visionPolygonVertices.Empty();

      // add extra points to make resulting shape close to a circle
      TSet<FVector> calculationCorners = TSet<FVector>(visionBlockerCorners);
      calculationCorners.Add(GetActorLocation() + FVector(0, GetVisionRadius(), 0));
      calculationCorners.Add(GetActorLocation() + FVector(GetVisionRadius() * root2Over2, GetVisionRadius() * root2Over2, 0));
      calculationCorners.Add(GetActorLocation() + FVector(GetVisionRadius(), 0, 0));
      calculationCorners.Add(GetActorLocation() + FVector(GetVisionRadius() * root2Over2, GetVisionRadius() * -root2Over2, 0));
      calculationCorners.Add(GetActorLocation() + FVector(0, -GetVisionRadius(), 0));
      calculationCorners.Add(GetActorLocation() + FVector(GetVisionRadius() * -root2Over2, GetVisionRadius() * -root2Over2, 0));
      calculationCorners.Add(GetActorLocation() + FVector(-GetVisionRadius(), 0, 0));
      calculationCorners.Add(GetActorLocation() + FVector(GetVisionRadius() * -root2Over2, GetVisionRadius() * root2Over2, 0));

      // sort the points by angle
      calculationCorners.Sort(pred);
      visionPolygonVertices.Add(GetActorLocation());

      for (FVector c : calculationCorners) {
         FVector dist = c - GetActorLocation();
         if (dist.SizeSquared2D() < GetVisionRadius() * GetVisionRadius() + 1) {
            // traceHandlers.Add(GetWorld()->AsyncLineTraceByObjectType(EAsyncTraceType::Single,GetActorLocation(), c, gameState->queryParamVision));
            FHitResult r;
            if (GetWorld()->LineTraceSingleByChannel(r, GetActorLocation(), c, ECollisionChannel::ECC_GameTraceChannel13)) {
               // DrawDebugLine(GetWorld(), GetActorLocation(), r.ImpactPoint, FColor::Red, false, 5, 0, 10);
               visionPolygonVertices.Add(r.ImpactPoint);
            } else {
               // DrawDebugLine(GetWorld(), GetActorLocation(), c, FColor::Red, false, 5, 0, 10);
               visionPolygonVertices.Add(c);
            }
         }
      }
   }
}

bool AAlly::IsVisible()
{
   return gameState->visibleAllies.Contains(this);
}

TSet<AUnit*>* AAlly::GetSeenEnemies()
{
   return &possibleEnemiesInRadius;
}

void AAlly::GetTraceResults()
{
   auto traceIt = traceHandlers.CreateIterator();
   while (traceIt) {
      if (GetWorld()->QueryTraceData(*traceIt, traceResults)) {
         visionPolygonVertices.Add(traceResults.End);
         traceHandlers.Remove(*traceIt);
         ++traceIt;
      }
   }
}
