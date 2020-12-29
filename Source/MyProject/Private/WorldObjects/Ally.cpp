// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Ally.h"
#include "SpellSystem/MySpell.h"
#include "RTSGameState.h"
#include "RTSPawn.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "UI/HUDManager.h"
#include "UI/UserWidgets/RTSIngameWidget.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CombatParameters.h"
#include "AIStuff/AIControllers/AllyAIController.h"
#include "PatrolComponent.h"
#include "Enemy.h"
#include "RTSVisionComponent.h"
#include "PartyDelegateStore.h"

AAlly::AAlly(const FObjectInitializer& oI) : AUnit(oI)
{
   GetCapsuleComponent()->SetCollisionProfileName("Ally");
   GetMesh()->CustomDepthStencilValue = 254; // Green Highlights when Hovering
   visionComponent->SetHiddenInGame(true);

   AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAlly::BeginPlay()
{
   Super::BeginPlay();
}

void AAlly::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);

   if(!controllerRef->IsInputKeyDown(EKeys::LeftShift) && GetState() == EUnitState::STATE_IDLE && !commandQueue.IsEmpty()) {
      TFunction<void()> command;
      commandQueue.Dequeue(command);
      --queueCount;
      command();
   }
}

void AAlly::EndPlay(const EEndPlayReason::Type eReason)
{
   Super::EndPlay(eReason);
}

void AAlly::PossessedBy(AController* newAllyControllerRef)
{
   Super::PossessedBy(newAllyControllerRef);
   allyController = Cast<AAllyAIController>(newAllyControllerRef);
}

void AAlly::SetEnabled(bool bEnabled)
{
   Super::SetEnabled(bEnabled);
   if(ULocalPlayer* player = Cast<ULocalPlayer>(controllerRef->Player)) { player->GetSubsystem<UPartyDelegateStore>()->OnAllyActiveChanged().Broadcast(this, bEnabled); }
}

void AAlly::SetSelected(bool value)
{
   Super::SetSelected(value);
   if(value) {
      controllerRef->GetBasePlayer()->selectedAllies.AddUnique(this);
   } else {
      controllerRef->GetBasePlayer()->selectedAllies.RemoveSingle(this);
   }
}

bool AAlly::GetOverlappingObjects(TArray<FHitResult>& hits)
{
   const FVector start = controllerRef->GetCameraPawn()->GetCameraComponent()->GetComponentLocation();
   const FVector end   = GetActorLocation();

   //// For debugging trace
   // const FName TraceTag("MyTraceTag");
   // GetWorld()->DebugDrawTraceTag = TraceTag;
   // FCollisionQueryParams params;
   // params.TraceTag = TraceTag;

   FQuat cameraRotation = controllerRef->GetCameraPawn()->GetCameraComponent()->GetComponentQuat();
   cameraRotation.W *= -1;
   GetWorld()->SweepMultiByChannel(hits, start, end, cameraRotation, FADE_CHANNEL, FCollisionShape::MakeBox(FVector(200, 200, 100)));
   if(!hits.Num()) return false;
   return true;
}

void AAlly::QueueAction(TFunction<void()> actionToQueue)
{
   if(queueCount < 20) {
      GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Emerald, TEXT("SUCESSFUL QUEUE"));
      commandQueue.Enqueue(actionToQueue);
      ++queueCount;
   } else
      URTSIngameWidget::NativeDisplayHelpText(GetWorld(), FILLED_QUEUE_TEXT);
}

const TSet<AUnit*>& AAlly::GetSeenEnemies() const
{
   return possibleEnemiesInRadius;
}
