// Created 9/19/20 1:43 PM

#include "MyProject.h"
#include "VisionSubsystem.h"

#include "ParallelFor.h"
#include "RTSGameMode.h"
#include "RTSGameState.h"
#include "RTSVisionComponent.h"
#include "SpellDataLibrary.h"
#include "Transform.h"
#include "Unit.h"

DECLARE_CYCLE_STAT(TEXT("Unit Vision Update"), STAT_UnitVision, STATGROUP_RTSUnits)

namespace VisionCVars
{
   static float                   VisionUpdateLoopTime = .1f;
   static FAutoConsoleVariableRef CVarNumSecondsBeforeShowingInMinutes(TEXT("Up_DebugVisionUpdateLoopTime"), VisionUpdateLoopTime,
                                                                       TEXT("How long between vision updates in seconds"), ECVF_Default);
}

void UVisionSubsystem::BeginDestroy()
{
   Super::BeginDestroy();
   if(visionUpdateThread)
   {
      UE_LOG(LogTemp, Log, TEXT("Destroying vision subsystem..."));
      visionUpdateThread->Kill(true);
      UE_LOG(LogTemp, Log, TEXT("Vision subsystem destroyed..."));
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

   return newVisionManager;
}

uint32 UVisionSubsystem::Run()
{
   while(stopTaskCounter.GetValue() == 0)
   {
      SCOPE_CYCLE_COUNTER(STAT_UnitVision)
      {
         TSet<AUnit*> lastVisiblePlayerUnits;
         TSet<AUnit*> lastVisibleEnemies;

         StoreUnitsToHideThatWereVisibleLastCall(lastVisiblePlayerUnits, visiblePlayerUnits);
         StoreUnitsToHideThatWereVisibleLastCall(lastVisibleEnemies, visibleEnemies);

         visiblePlayerUnits.Empty(visiblePlayerUnits.Num());
         visibleEnemies.Empty(visibleEnemies.Num());

         UpdateVisibleEnemies();
         UpdateVisiblePlayerUnits();

         MakeUnitsInVisionVisible(visibleEnemies);
         MakeUnitsOutOfVisionInvisible(lastVisibleEnemies, visibleEnemies);

         MakeUnitsInVisionVisible(visiblePlayerUnits);
         MakeUnitsOutOfVisionInvisible(lastVisiblePlayerUnits, visiblePlayerUnits);

         FPlatformProcess::SleepNoStats(VisionCVars::VisionUpdateLoopTime);
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
   Stop();
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
   Stop();
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
   Algo::TransformIf(gameStateRef->GetAllAllyUnits(), friendlyVisionComps, friendlyUnitSeesSomeone, getFriendlyVisionComp);
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

   for(const URTSVisionComponent* allyVisionComp : allyVisionToIterateOver)
   {
      FRWScopeLock(allyVisionComp->visionMutex, FRWScopeLockType::SLT_ReadOnly);
      for(AUnit* enemy : allyVisionComp->GetPossibleVisibleEnemies())
      {
         if(CheckUnitInVision(enemy, allyVisionComp, visibleEnemies))
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
      FRWScopeLock(enemyVisionComp->visionMutex, FRWScopeLockType::SLT_ReadOnly);
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
   if(IsValid(unit) && !visibleUnits.Contains(unit))
   {
      if(LineOfSightToNonInvisUnit(unit, visionComp))
      {
         return true;
      }
   }
   return false;
}

bool UVisionSubsystem::LineOfSightToNonInvisUnit(AUnit* unit, const URTSVisionComponent* visionComp)
{
   if(UNLIKELY(!USpellDataLibrary::IsInvisible(unit->GetAbilitySystemComponent())))
   {
      if(!GetWorld()->LineTraceSingleByChannel(visionHitResult, visionComp->GetOwner()->GetActorLocation(), unit->GetActorLocation(), UNIT_VISION_CHANNEL))
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
      if(unit)
      {
         unit->SetIsUnitHidden(false);
      }
   }
}

void UVisionSubsystem::MakeUnitsOutOfVisionInvisible(TSet<AUnit*>& unitsEligibleForHidingVisibleLastCheck, TSet<AUnit*>& unitsDeemedVisibleAfterCheck) const
{
   for(AUnit* visibleUnit : unitsEligibleForHidingVisibleLastCheck)
   {
      if(visibleUnit)
      {
         if(!unitsDeemedVisibleAfterCheck.Contains(visibleUnit))
         {
            visibleUnit->SetIsUnitHidden(true);
         }
      }
   }
}
