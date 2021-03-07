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
   visionManager = UVisionSubsystem::Create(this);
   GetWorld()->GetTimerManager().SetTimer(updateCachedVisibleUnitsTimerHandle, [this]()
   {
      visibleEnemies = enemyList.FilterByPredicate([](const AUnit* enemy)
      {
         return !enemy->GetIsUnitHidden();
      });

      visiblePlayerUnits = allyList.FilterByPredicate([](const AUnit* ally)
      {
         return !ally->GetIsUnitHidden();
      });
   }, 0.1f, true, 0.f);

   clock = MakeUnique<FUpGameClock>(*this, FUpTime(), FUpDate());
   OnGameSpeedUpdated().AddDynamic(this, &ARTSGameState::UpdateGameSpeed);

   // Don't really use this ATM but it's there if we want to play around with it and actually make it a thing we can increase the precision of the model by adding more polygons to the plane
   // and performing more line traces. However it is quite inefficient since we would have to do this for every ally...
   // Implemented via https://www.redblobgames.com/articles/visibility/.
   // ! Not using this currently however...
   GetWorld()->SpawnActor<AFogOfWarPlane>(FOWplaneClass, FVector(0, 0, 0), FRotator());

   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateContext>()->OnAllyActiveChanged().AddUObject(this, &ARTSGameState::OnAllyActiveChanged);
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateContext>()->OnEnemyActiveChanged().AddUObject(this, &ARTSGameState::OnEnemyActiveChanged);
   Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode())->OnLevelAboutToUnload.AddDynamic(this, &ARTSGameState::CleanupUnitLists);
}

void ARTSGameState::CleanupUnitLists()
{
   Cast<ARTSGameState>(GetWorld()->GetGameState())->enemyList.Shrink();
   Cast<ARTSGameState>(GetWorld()->GetGameState())->allyList.Shrink();
}

void ARTSGameState::ShowEnemyPerspective()
{
   visionManager->ToggleEnemyPerspective();
}

const TArray<AUnit*>& ARTSGameState::GetVisibleEnemies() const
{
   return visibleEnemies;
}

const TArray<AUnit*>& ARTSGameState::GetVisiblePlayerUnits() const
{
   return visiblePlayerUnits;
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
   } else
   {
      UnRegisterFriendlyUnit(allyRef);
   }
}

void ARTSGameState::OnEnemyActiveChanged(AEnemy* enemyRef, bool isActive)
{
   if(isActive)
   {
      RegisterEnemyUnit(enemyRef);
   } else
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
