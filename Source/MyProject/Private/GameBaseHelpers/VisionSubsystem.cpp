﻿// Created 9/19/20 1:43 PM

#include "MyProject.h"
#include "VisionSubsystem.h"

#include "ParallelFor.h"
#include "RTSGameMode.h"
#include "RTSGameState.h"
#include "RTSVisionComponent.h"
#include "SpellDataLibrary.h"
#include "Transform.h"
#include "Unit.h"
#include "WindowsCriticalSection.h"

DECLARE_CYCLE_STAT(TEXT("Unit Vision Update"), STAT_UnitVision, STATGROUP_RTSUnits)

namespace VisionCVars
{
   static float                   VisionUpdateLoopTime = .1f;
   static FAutoConsoleVariableRef CVarNumSecondsBeforeShowingInMinutes(TEXT("VisionUpdateLoopTime"), VisionUpdateLoopTime,
                                                                       TEXT("How long between vision updates in seconds"), ECVF_Default);
}

void UVisionSubsystem::BeginDestroy()
{
   Super::BeginDestroy();
   if(visionUpdateThread)
   {
      visionUpdateThread->Kill(true);
   }
}

UVisionSubsystem::UVisionSubsystem()
{
   queryParamVision.AddObjectTypesToQuery(VISION_BLOCKER_CHANNEL); // Query vision blockers only
}

UVisionSubsystem* UVisionSubsystem::Create(UObject* outer)
{
   UVisionSubsystem* newVisionManager = NewObject<UVisionSubsystem>(outer);
   newVisionManager->gameStateRef     = Cast<ARTSGameState>(newVisionManager->GetWorld()->GetGameState());

   FCoreUObjectDelegates::PreLoadMap.AddUObject(newVisionManager, &UVisionSubsystem::OnPreLoadMap);
   FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(newVisionManager, &UVisionSubsystem::OnPostLoadMap);

   if(ARTSGameMode* gameModeRef = Cast<ARTSGameMode>(newVisionManager->GetWorld()->GetAuthGameMode()))
   {
      gameModeRef->OnLevelAboutToUnload.AddDynamic(newVisionManager, &UVisionSubsystem::OnLevelAboutToUnload);
      gameModeRef->OnLevelLoaded.AddDynamic(newVisionManager, &UVisionSubsystem::OnLevelLoaded);
   }

   newVisionManager->visionUpdateThread = FRunnableThread::Create(newVisionManager, TEXT("VisionWorker"));
   return newVisionManager;
}

uint32 UVisionSubsystem::Run()
{
   while(stopTaskCounter.GetValue() == 0)
   {
      SCOPE_CYCLE_COUNTER(STAT_UnitVision)
      {
         TSet<AUnit*>  unitsEligibleForHidingVisibleLastCheck;
         TSet<AUnit*>* unitsEligibleForHiding;

         if(bShowEnemyPerspective)
         {
            unitsEligibleForHiding = &visiblePlayerUnits;
            visibleEnemies.Empty(visibleEnemies.Num());
         }
         else
         {
            unitsEligibleForHiding = &visibleEnemies;
            visiblePlayerUnits.Empty(visiblePlayerUnits.Num());
         }

         StoreUnitsToHideThatWereVisibleLastCall(unitsEligibleForHidingVisibleLastCheck, *unitsEligibleForHiding);

         UpdateVisibleEnemies();
         UpdateVisiblePlayerUnits();

         MakeUnitsInVisionVisible(unitsEligibleForHidingVisibleLastCheck);
         MakeUnitsOutOfVisionInvisible(unitsEligibleForHidingVisibleLastCheck, *unitsEligibleForHiding);

         FPlatformProcess::SleepNoStats(0.1f);
      }
   }
   return 0;
}

void UVisionSubsystem::Stop()
{
   stopTaskCounter.Increment();
   visibleEnemies.Empty();
   visiblePlayerUnits.Empty();
}

void UVisionSubsystem::OnPreLoadMap(const FString& MapName)
{
   visionUpdateThread->Kill(true);
}

void UVisionSubsystem::OnPostLoadMap(UWorld* World)
{
   stopTaskCounter.Reset();
   visionUpdateThread = FRunnableThread::Create(this, TEXT("VisionWorker"));
}

void UVisionSubsystem::OnLevelLoaded()
{
   stopTaskCounter.Reset();
   visionUpdateThread = FRunnableThread::Create(this, TEXT("VisionWorker"));
}

void UVisionSubsystem::OnLevelAboutToUnload()
{
   visionUpdateThread->Kill(true);
}

void UVisionSubsystem::AddVisibleAlly(AUnit* newAlly)
{
   visiblePlayerUnits.Add(newAlly);
}

void UVisionSubsystem::AddVisibleEnemy(AUnit* newEnemy)
{
   visibleEnemies.Add(newEnemy);
}

TSet<const URTSVisionComponent*> UVisionSubsystem::GetFriendlyVisionComps() const
{
   TSet<const URTSVisionComponent*> friendlyVisionComps;
   const auto                       friendlyUnitSeesSomeone = [](const AUnit* friendlyUnit) {
      return friendlyUnit->GetVisionComponent()->GetPossibleVisibleEnemies().Num() > 0;
   };
   const auto getFriendlyVisionComp = [](const AUnit* friendlyUnit) {
      return friendlyUnit->GetVisionComponent();
   };
   Algo::TransformIf(gameStateRef->GetAllFriendlyUnits(), friendlyVisionComps, friendlyUnitSeesSomeone, getFriendlyVisionComp);
   return friendlyVisionComps;
}

TSet<const URTSVisionComponent*> UVisionSubsystem::GetEnemyVisionComps() const
{
   TSet<const URTSVisionComponent*> enemyVisionComps;
   const auto                       enemySeesSomeone = [](const AUnit* enemyUnit) {
      return enemyUnit->GetVisionComponent()->GetPossibleVisibleEnemies().Num() > 0;
   };

   const auto getEnemyVisionComp = [](const AUnit* enemyUnit) {
      return enemyUnit->GetVisionComponent();
   };
   Algo::TransformIf(gameStateRef->GetAllEnemyUnits(), enemyVisionComps, enemySeesSomeone, getEnemyVisionComp);
   return enemyVisionComps;
}

void UVisionSubsystem::UpdateVisibleEnemies()
{
   const TSet<const URTSVisionComponent*> allyVisionToIterateOver = GetFriendlyVisionComps();

   for(const URTSVisionComponent* allyVision : allyVisionToIterateOver)
   {
      for(AUnit* enemy : allyVision->GetPossibleVisibleEnemies())
      {
         if(CheckUnitInVision(enemy, allyVision, visibleEnemies))
         {
            AddVisibleEnemy(enemy);
         }
      }
   }
}

void UVisionSubsystem::UpdateVisiblePlayerUnits()
{
   const TSet<const URTSVisionComponent*> enemyVisionToIterateOver = GetEnemyVisionComps();

   for(const URTSVisionComponent* enemyVisionComp : enemyVisionToIterateOver)
   {
      for(AUnit* ally : enemyVisionComp->GetPossibleVisibleEnemies())
      {
         if(CheckUnitInVision(ally, enemyVisionComp, visiblePlayerUnits))
         {
            AddVisibleAlly(ally);
         }
      }
   }
}

void UVisionSubsystem::StoreUnitsToHideThatWereVisibleLastCall(TSet<AUnit*>& lastCallCache, TSet<AUnit*>& unitsToPossiblyHide)
{
   Swap(lastCallCache, unitsToPossiblyHide);
}

bool UVisionSubsystem::CheckUnitInVision(AUnit* unit, const URTSVisionComponent* visionComp, TSet<AUnit*>& visibleUnits)
{
   unit->GetVisionComponent()->visionMutex.ReadLock();

   if(IsValid(unit) && !visibleUnits.Contains(unit))
   {
      if(LineOfSightToNonInvisUnit(unit, visionComp))
      {
         return true;
      }
   }
   return false;
}

bool UVisionSubsystem::LineOfSightToNonInvisUnit(AUnit* unit, const URTSVisionComponent* allyVision)
{
   if(UNLIKELY(!USpellDataLibrary::IsInvisible(unit->GetAbilitySystemComponent())))
   {
      if(!GetWorld()->LineTraceSingleByChannel(visionHitResult, allyVision->GetOwner()->GetActorLocation(), unit->GetActorLocation(), UNIT_VISION_CHANNEL))
      {
         return true;
      }
   }
   return false;
}

void UVisionSubsystem::MakeUnitsInVisionVisible(TSet<AUnit*>& unitsEligibleForHidingVisibleLastCheck)
{
   for(AUnit* unit : unitsEligibleForHidingVisibleLastCheck)
   {
      unit->SetIsUnitHidden(false);
   }
}

void UVisionSubsystem::MakeUnitsOutOfVisionInvisible(TSet<AUnit*>& unitsEligibleForHidingVisibleLastCheck, TSet<AUnit*>& unitsDeemedVisibleAfterCheck) const
{
   for(AUnit* visibleUnit : unitsEligibleForHidingVisibleLastCheck)
   {
      if(!unitsDeemedVisibleAfterCheck.Contains(visibleUnit))
      {
         visibleUnit->SetIsUnitHidden(true);
      }
   }
}

void UVisionSubsystem::ToggleEnemyPerspective()
{
   bShowEnemyPerspective = !bShowEnemyPerspective;
   if(bShowEnemyPerspective)
   {
      for(AUnit* alliedUnit : gameStateRef->GetAllFriendlyUnits())
      {
         alliedUnit->SetIsUnitHidden(true);
      }
      for(AUnit* enemyUnit : gameStateRef->GetAllEnemyUnits())
      {
         enemyUnit->SetIsUnitHidden(false);
      }
   }
   else
   {
      for(AUnit* alliedUnit : gameStateRef->GetAllFriendlyUnits())
      {
         alliedUnit->SetIsUnitHidden(false);
      }
      for(AUnit* enemyUnit : gameStateRef->GetAllEnemyUnits())
      {
         enemyUnit->SetIsUnitHidden(true);
      }
   }
}
