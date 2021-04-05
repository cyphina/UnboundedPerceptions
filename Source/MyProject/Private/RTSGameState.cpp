// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSGameState.h"

#include "GameSpeedWidget.h"
#include "HUDManager.h"
#include "ParallelFor.h"
<<<<<<< HEAD
#include "RTSGameMode.h"
#include "RTSIngameWidget.h"
#include "UserInput.h"
#include "VisionSubsystem.h"
=======
#include "PartyDelegateContext.h"
#include "RTSGameMode.h"
#include "RTSIngameWidget.h"
#include "UserInput.h"
>>>>>>> componentrefactor
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
<<<<<<< HEAD
   visionManager = UVisionSubsystem::Create(this);
=======
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

>>>>>>> componentrefactor
   clock = MakeUnique<FUpGameClock>(*this, FUpTime(), FUpDate());
   OnGameSpeedUpdated().AddDynamic(this, &ARTSGameState::UpdateGameSpeed);

   // Don't really use this ATM but it's there if we want to play around with it and actually make it a thing we can increase the precision of the model by adding more polygons to the plane
   // and performing more line traces. However it is quite inefficient since we would have to do this for every ally...
   // Implemented via https://www.redblobgames.com/articles/visibility/.
   // ! Not using this currently however...
   GetWorld()->SpawnActor<AFogOfWarPlane>(FOWplaneClass, FVector(0, 0, 0), FRotator());
<<<<<<< HEAD

   Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode())->OnLevelAboutToUnload.AddDynamic(this, &ARTSGameState::CleanupUnitLists);
}

void ARTSGameState::CleanupUnitLists()
{
   Cast<ARTSGameState>(GetWorld()->GetGameState())->enemyList.Shrink();
   Cast<ARTSGameState>(GetWorld()->GetGameState())->enemyList.Compact();
   Cast<ARTSGameState>(GetWorld()->GetGameState())->allyList.Shrink();
   Cast<ARTSGameState>(GetWorld()->GetGameState())->allyList.Compact();
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
   // Even if we remove the item there will be some kind of empty space in the set that can be hit when iterating, so compact it.
   allyList.CompactStable();
}

void ARTSGameState::UnRegisterEnemyUnit(AEnemy* enemyUnit)
{
   enemyList.Remove(enemyUnit);
   // Even if we remove the item there will be some kind of empty space in the set that can be hit when iterating, so compact it.
   enemyList.CompactStable();
}

const TSet<AUnit*>& ARTSGameState::GetVisibleEnemies() const
{
   return visionManager->GetVisibleEnemies();
}

const TSet<AUnit*>& ARTSGameState::GetVisiblePlayerUnits() const
{
   return visionManager->GetVisiblePlayerUnits();
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

void ARTSGameState::OnAllyActiveChanged(AAlly* allyRef, bool isActive)
{
   if(isActive)
      RegisterFriendlyUnit(allyRef);
   else
      UnRegisterFriendlyUnit(allyRef);
}

void ARTSGameState::OnEnemyActiveChanged(AEnemy* enemyRef, bool isActive)
{
   if(isActive)
      RegisterEnemyUnit(enemyRef);
   else
      UnRegisterEnemyUnit(enemyRef);
=======

   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateContext>()->OnAllyActiveChanged().AddUObject(this, &ARTSGameState::OnAllyActiveChanged);
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateContext>()->OnEnemyActiveChanged().AddUObject(this, &ARTSGameState::OnEnemyActiveChanged);
   Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode())->OnLevelAboutToUnload.AddDynamic(this, &ARTSGameState::CleanupUnitLists);
}

void ARTSGameState::CleanupUnitLists()
{
   Cast<ARTSGameState>(GetWorld()->GetGameState())->enemyList.Shrink();
   Cast<ARTSGameState>(GetWorld()->GetGameState())->allyList.Shrink();
}

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
>>>>>>> componentrefactor
}
