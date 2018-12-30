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
#include "ResourceManager.h"
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

   abilities.SetNum(MAX_NUM_SPELLS); //Size of abilities that can be used on actionbar
  
   //Set collision to friendly type and block traces made on the AllyTrace channel
   GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel9);
   GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel10, ECollisionResponse::ECR_Block); 
   
   visionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAlly::OnVisionSphereOverlap);
   visionSphere->OnComponentEndOverlap.AddDynamic(this, &AAlly::OnVisionSphereEndOverlap);
   visionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap); // Enemy
   visionSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECollisionResponse::ECR_Overlap);

#if UE_EDITOR
   visionSphere->SetHiddenInGame(false);
#endif

   AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAlly::BeginPlay()
{
   Super::BeginPlay();

   Cast<ABasePlayer>(controllerRef->PlayerState)->allies.Add(this);
   Cast<ARTSGameState>(GetWorld()->GetGameState())->allyList.Add(this);
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
   switch(eReason)
   {
      case EEndPlayReason::Destroyed:
      controllerRef->GetBasePlayer()->allies.Remove(this);
      controllerRef->GetGameState()->allyList.Remove(this);
      break;
      default: break;
   }
}

void AAlly::PossessedBy(AController* newAllyControllerRef)
{
   Super::PossessedBy(newAllyControllerRef);
   allyController = Cast<AAllyAIController>(newAllyControllerRef);
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
   if (Super::CastSpell(spellToCast)) {
      if (controllerRef->GetBasePlayer()->focusedUnit == this) controllerRef->GetHUDManager()->GetActionHUD()->ShowSkillVisualCD(spellIndex);
      return true;
   }
   return false;
}

float AAlly::CalculateTargetRisk()
{
   int          targetNum = 0;
   for (AEnemy* e : controllerRef->GetGameState()->visibleEnemies) {
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
         AEnemy* enemy = Cast<AEnemy>(otherActor);
         // TODO: Check for invisibility
         possibleEnemiesInRadius.Add(enemy);
         enemy->IncVisionCount();
         break;
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
         AEnemy* enemy = Cast<AEnemy>(otherActor);
         possibleEnemiesInRadius.Remove(enemy);
         enemy->DecVisionCount();
         if (!enemy->GetVisionCount()) {
            enemy->GetCapsuleComponent()->SetVisibility(false, true);
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
   for (FVector v : ResourceManager::BoundsPointMapping2D) {
      FVector   corner     = origin + extent * v;
      FVector   cornerDist = corner - GetActorLocation();
      visionBlockerCorners.Add(corner);
   }
}

void AAlly::RemoveCornersInRange(AActor* overlapEndActor)
{
   FVector origin = FVector();
   FVector extent = FVector();
   overlapEndActor->GetActorBounds(true, origin, extent);
   for (FVector v : ResourceManager::BoundsPointMapping2D) {
      FVector   corner     = origin + extent * v;
      FVector   cornerDist = corner - GetActorLocation();
      visionBlockerCorners.Remove(corner);
   }
}

void AAlly::FindVisibilityPoints()
{
   SCOPE_CYCLE_COUNTER(STAT_AllyVision)
   {
      auto pred = [this](const FVector& a, const FVector& b)
      {
         return ResourceManager::FindOrientation(a - GetActorLocation()) > ResourceManager::FindOrientation(b - GetActorLocation());
      };
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
         FVector   dist = c - GetActorLocation();
         if (dist.SizeSquared2D() < GetVisionRadius() * GetVisionRadius() + 1) {
            // traceHandlers.Add(GetWorld()->AsyncLineTraceByObjectType(EAsyncTraceType::Single,GetActorLocation(), c, gameState->queryParamVision));
            FHitResult r;
            if (GetWorld()->LineTraceSingleByObjectType(r, GetActorLocation(), c, gameState->queryParamVision)) {
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
