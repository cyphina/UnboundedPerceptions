// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSGameState.h"

#include "GameSpeedWidget.h"
#include "HUDManager.h"
#include "ParallelFor.h"
#include "PartyDelegateContext.h"
#include "RTSGameMode.h"
#include "RTSIngameWidget.h"
#include "UserInput.h"
#include "WorldObjects/Ally.h"
#include "WorldObjects/Enemies/Enemy.h"
#include "FogOfWar/FogOfWarPlane.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h"
#include "VisionSubsystem.h"
#include "Kismet/KismetMathLibrary.h"

ARTSGameState::ARTSGameState()
{
   defaultGameTimeSpeed = 60;
}

void ARTSGameState::BeginPlay()
{
   Super::BeginPlay();
   StartVisionChecks();
   GetWorld()->GetTimerManager().SetTimer(
       updateCachedVisibleUnitsTimerHandle,
       [this]() {
          visibleEnemies = enemyList.FilterByPredicate([](const AUnit* enemy) {
             return !enemy->GetIsUnitHidden();
          });

          visiblePlayerUnits = allyList.FilterByPredicate([](const AUnit* ally) {
             return !ally->GetIsUnitHidden();
          });
       },
       0.1f, true, 0.f);

   clock = MakeUnique<FUpGameClock>(*this, FUpTime(), FUpDate());
   OnGameSpeedUpdated().AddDynamic(this, &ARTSGameState::UpdateGameSpeed);

   // Don't really use this ATM but it's there if we want to play around with it and actually make it a thing we can increase the precision of the model by adding more polygons to the plane
   // and performing more line traces. However it is quite inefficient since we would have to do this for every ally...
   // Implemented via https://www.redblobgames.com/articles/visibility/.
   // ! Not using this currently however...
   GetWorld()->SpawnActor<AFogOfWarPlane>(FOWplaneClass, FVector(0, 0, 0), FRotator());

   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateContext>()->OnAllyActiveChanged().AddUObject(this, &ARTSGameState::OnAllyActiveChanged);
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateContext>()->OnEnemyActiveChanged().AddUObject(this, &ARTSGameState::OnEnemyActiveChanged);

   if(HasAuthority() || !IsRunningDedicatedServer())
   {
      if(ARTSGameMode* RTSGameModeRef = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode()))
      {
         RTSGameModeRef->OnLevelAboutToUnload.AddDynamic(this, &ARTSGameState::CleanupUnitLists);
      }
   }

#if WITH_EDITOR
   if(HasAuthority() || !IsRunningDedicatedServer())
   {
      if(ARTSGameMode* RTSGameModeRef = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode()))
      {
         RTSGameModeRef->OnLevelLoaded.AddDynamic(this, &ARTSGameState::CheckMapBounds);
      }
   }
#endif
}

void ARTSGameState::CleanupUnitLists()
{
   Cast<ARTSGameState>(GetWorld()->GetGameState())->enemyList.Shrink();
   Cast<ARTSGameState>(GetWorld()->GetGameState())->allyList.Shrink();
}

#if WITH_EDITOR
void ARTSGameState::CheckMapBounds()
{
   GetWorld()->GetTimerManager().SetTimerForNextTick([this]() {
      if(cameraBoundX.X <= 0 && cameraBoundX.Y <= 0 || cameraBoundY.X <= 0 && cameraBoundY.Y <= 0)
      {
         UE_LOG(
             LogTemp, Warning,
             TEXT("Map limits set to be less than or equal to 0. Camera will not be unable to move until this is changed in level blueprint. Is this what you wanted?"));
      }
   });
}
#endif

void ARTSGameState::Up_PrintOutVisibleUnits()
{
   static FTimerHandle debugHandle;
   static bool         isPrinting = false;

   if(!isPrinting)
   {
      GetWorld()->GetTimerManager().SetTimer(
          debugHandle,
          [this]() {
             FString visibleAllyNames = "Allies: \n";
             for(AUnit* visibleAlly : visiblePlayerUnits)
             {
                if(!visibleAlly->GetGameName().IsEmpty())
                {
                   visibleAllyNames += visibleAlly->GetGameName().ToString() + "\n";
                }
                else
                {
                   visibleAllyNames += visibleAlly->GetName();
                }
             }

             FString visibleEnemyNames = "Enemies: \n";
             for(AUnit* visibleEnemy : visibleEnemies)
             {
                if(!visibleEnemy->GetGameName().IsEmpty())
                {
                   visibleEnemyNames += visibleEnemy->GetGameName().ToString() + "\n";
                }
                else
                {
                   visibleEnemyNames += visibleEnemy->GetName();
                }
             }
             GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::White, visibleAllyNames + visibleEnemyNames);
          },
          0.2f, true, 0.f);

      isPrinting = true;
   }
   else
   {
      GetWorld()->GetTimerManager().ClearTimer(debugHandle);
      isPrinting = false;
   }
}

const TArray<AUnit*>& ARTSGameState::GetVisibleEnemies() const
{
   return visibleEnemies;
}

const TArray<AUnit*>& ARTSGameState::GetVisiblePlayerUnits() const
{
   return visiblePlayerUnits;
}

void ARTSGameState::StartVisionChecks()
{
   if(!visionManager)
   {
      visionManager = UVisionSubsystem::Create(this);
   }
}

void ARTSGameState::StopVisionChecks()
{
   visionManager->ConditionalBeginDestroy();
}

void ARTSGameState::AddGameTime(FUpTime timeToAdd, FUpDate daysToAdd)
{
   clock->AddGameTime(timeToAdd, daysToAdd);
}

void ARTSGameState::SetGameTime(FUpTime timeToAdd, FUpDate daysToAdd)
{
   clock->SetGameTime(timeToAdd, daysToAdd);
}

void ARTSGameState::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);
   clock->TickClock(deltaSeconds);
}

void ARTSGameState::UpdateGameSpeed(float newSpeedMultiplier)
{
   UE_LOG(LogTemp, Warning, TEXT("Setting game speed to: %f"), newSpeedMultiplier);
   speedModifier = newSpeedMultiplier;
   UGameplayStatics::SetGlobalTimeDilation(GetWorld(), newSpeedMultiplier);
}

void ARTSGameState::Client_EndMinigame_Implementation(EMinigameType MinigameType)
{
   OnMinigameStartedEvent.Broadcast(MinigameType);
}

void ARTSGameState::Client_StartMinigame_Implementation(EMinigameType MinigameType)
{
   OnMinigameStartedEvent.Broadcast(MinigameType);
}

void ARTSGameState::OnAllyActiveChanged(AAlly* allyRef, bool isActive)
{
   if(isActive)
   {
      RegisterFriendlyUnit(allyRef);
   }
   else
   {
      UnRegisterFriendlyUnit(allyRef);
   }
}

void ARTSGameState::OnEnemyActiveChanged(AEnemy* enemyRef, bool isActive)
{
   if(isActive)
   {
      RegisterEnemyUnit(enemyRef);
   }
   else
   {
      UnRegisterEnemyUnit(enemyRef);
   }
}

void ARTSGameState::RegisterFriendlyUnit(AAlly* friendlyUnit)
{
   allyList.Add(friendlyUnit);
}

void ARTSGameState::RegisterEnemyUnit(AEnemy* enemyUnit)
{
   enemyList.Add(enemyUnit);
}

void ARTSGameState::UnRegisterFriendlyUnit(AAlly* friendlyUnit)
{
   allyList.Remove(friendlyUnit);
}

void ARTSGameState::UnRegisterEnemyUnit(AEnemy* enemyUnit)
{
   enemyList.Remove(enemyUnit);
}
