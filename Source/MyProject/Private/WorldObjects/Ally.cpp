#include "MyProject.h"
#include "Ally.h"
#include "SpellSystem/MySpell.h"
#include "RTSGameState.h"
#include "RTSPawn.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "UI/HUDManager.h"
#include "UI/UserWidgets/RTSIngameWidget.h"
#include "AIStuff/AIControllers/AllyAIController.h"
#include "Enemy.h"
#include "RTSVisionComponent.h"
#include "PartyDelegateContext.h"

const FText AAlly::STUNNED_TEXT = NSLOCTEXT("HelpMessages", "Stun", "Currently Stunned!");

AAlly::AAlly(const FObjectInitializer& oI) : AUnit(oI)
{
   GetCapsuleComponent()->SetCollisionProfileName("Ally");
   GetMesh()->CustomDepthStencilValue = 252; // Green Highlights when Hovering
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

void AAlly::Restart()
{
   Super::Restart();
}

void AAlly::SetEnabled(bool bEnabled)
{
   Super::SetEnabled(bEnabled);

   if(!HasAuthority() || !IsRunningDedicatedServer())
   {
      if(ULocalPlayer* player = Cast<ULocalPlayer>(controllerRef->Player))
      {
         player->GetSubsystem<UPartyDelegateContext>()->OnAllyActiveChanged().Broadcast(this, bEnabled);
      }
   }
}

void AAlly::SwitchAIModes_Implementation(EAllyBehavioralMode newMode)
{
   if(HasAuthority())
   {
      if(currentAllyBehavior != EAllyBehavioralMode::ABM_Neutral)
      {
         GetAllyAIController()->SwitchAIModes(newMode);
      }
      currentAllyBehavior = newMode;
   }
}

void AAlly::SetUnitSelected(bool value)
{
   Super::SetUnitSelected(value);
   if(value)
   {
      if(controllerRef->GetBasePlayer()->GetSelectedAllies().Num() < 16)
      {
         controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnAllySelectedDelegate.Broadcast(this);
      }
   }
   else
   {
      controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnAllySelectedDelegate.Broadcast(this);
   }
}

bool AAlly::GetOverlappingObjects(TArray<FHitResult>& hits)
{
   if(!HasAuthority() || !IsRunningDedicatedServer())
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
      if(!hits.Num())
      {
         return false;
      }
      return true;
   }
   return false;
}

const TSet<AUnit*>& AAlly::GetSeenEnemies() const
{
   return possibleEnemiesInRadius;
}

const TArray<AUnit*>* AAlly::GetVisibleEnemies_Impl() const
{
   const auto& gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState());
   return &gameStateRef->GetVisibleEnemies();
}

const TArray<AUnit*>* AAlly::GetAllies_Impl() const
{
   const auto& gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState());
   return &gameStateRef->GetAllAllyUnits();
}

const TArray<AUnit*>* AAlly::GetEnemies_Impl() const
{
   const auto& gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState());
   return &gameStateRef->GetAllEnemyUnits();
}
