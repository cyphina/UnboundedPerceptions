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
<<<<<<< HEAD
#include "WindowsCriticalSection.h"
=======
>>>>>>> componentrefactor

DECLARE_CYCLE_STAT(TEXT("Unit Vision Update"), STAT_UnitVision, STATGROUP_RTSUnits)

namespace VisionCVars
{
<<<<<<< HEAD
   static float VisionUpdateLoopTime = .1f;
   static FAutoConsoleVariableRef
       CVarNumSecondsBeforeShowingInMinutes(TEXT("VisionUpdateLoopTime"), VisionUpdateLoopTime,
                                            TEXT("How long between vision updates in seconds"),
                                            ECVF_Default);
=======
   static float                   VisionUpdateLoopTime = .1f;
   static FAutoConsoleVariableRef CVarNumSecondsBeforeShowingInMinutes(TEXT("VisionUpdateLoopTime"), VisionUpdateLoopTime,
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
>>>>>>> componentrefactor
}

UVisionSubsystem::UVisionSubsystem()
{
   queryParamVision.AddObjectTypesToQuery(VISION_BLOCKER_CHANNEL); // Query vision blockers only
}

UVisionSubsystem* UVisionSubsystem::Create(UObject* outer)
{
<<<<<<< HEAD
   auto newVisionManager = NewObject<UVisionSubsystem>(outer);
   if(ARTSGameMode* gameModeRef = Cast<ARTSGameMode>(newVisionManager->GetWorld()->GetAuthGameMode())) {
      newVisionManager->StartUpdating();
      gameModeRef->OnLevelLoaded.AddDynamic(newVisionManager, &UVisionSubsystem::ResumeUpdating);
      gameModeRef->OnLevelAboutToUnload.AddDynamic(newVisionManager, &UVisionSubsystem::StopUpdating);

      newVisionManager->StartUpdating();
      return newVisionManager;
   }
   return nullptr;
}

void UVisionSubsystem::StartUpdating()
{
   GetWorld()->GetTimerManager().SetTimer(allyVisionUpdateTimerHandle, this, &UVisionSubsystem::UpdateVisibleEnemies, VisionCVars::VisionUpdateLoopTime, true);
   GetWorld()->GetTimerManager().SetTimer(enemyVisionUpdateTimerHandle, this, &UVisionSubsystem::UpdateVisiblePlayerUnits, VisionCVars::VisionUpdateLoopTime, true);
}

void UVisionSubsystem::ResumeUpdating()
{
   GetWorld()->GetTimerManager().UnPauseTimer(allyVisionUpdateTimerHandle);
   GetWorld()->GetTimerManager().UnPauseTimer(enemyVisionUpdateTimerHandle);
}

void UVisionSubsystem::StopUpdating()
{
   GetWorld()->GetTimerManager().PauseTimer(allyVisionUpdateTimerHandle);
   GetWorld()->GetTimerManager().PauseTimer(enemyVisionUpdateTimerHandle);

   FRWScopeLock lock(visibleMutex, FRWScopeLockType::SLT_Write);
   FRWScopeLock lockPlayers(visiblePlayersMutex, FRWScopeLockType::SLT_Write);

   visibleEnemies.Empty();
   visiblePlayerUnits.Empty();
=======
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
>>>>>>> componentrefactor
}

void UVisionSubsystem::AddVisibleAlly(AUnit* newAlly)
{
<<<<<<< HEAD
   FRWScopeLock lockPlayers(visiblePlayersMutex, FRWScopeLockType::SLT_Write);
=======
>>>>>>> componentrefactor
   visiblePlayerUnits.Add(newAlly);
}

void UVisionSubsystem::AddVisibleEnemy(AUnit* newEnemy)
{
<<<<<<< HEAD
   FRWScopeLock lock(visibleMutex, FRWScopeLockType::SLT_Write);
=======
>>>>>>> componentrefactor
   visibleEnemies.Add(newEnemy);
}

TSet<const URTSVisionComponent*> UVisionSubsystem::GetFriendlyVisionComps() const
{
   TSet<const URTSVisionComponent*> friendlyVisionComps;
<<<<<<< HEAD
   const auto friendlyUnitSeesSomeone = [](const AUnit* friendlyUnit) { return friendlyUnit->GetVisionComponent()->GetPossibleVisibleEnemies().Num() > 0; };
   const auto getFriendlyVisionComp = [](const AUnit* friendlyUnit) { return friendlyUnit->GetVisionComponent(); };
   Algo::TransformIf(gameStateRef->GetAllFriendlyUnits(), friendlyVisionComps, friendlyUnitSeesSomeone, getFriendlyVisionComp);
   return friendlyVisionComps;

=======
   const auto                       friendlyUnitSeesSomeone = [](const AUnit* friendlyUnit) {
      return friendlyUnit->GetVisionComponent()->GetPossibleVisibleEnemies().Num() > 0;
   };
   const auto getFriendlyVisionComp = [](const AUnit* friendlyUnit) {
      return friendlyUnit->GetVisionComponent();
   };
   Algo::TransformIf(gameStateRef->GetAllAllyUnits(), friendlyVisionComps, friendlyUnitSeesSomeone, getFriendlyVisionComp);
   return friendlyVisionComps;
>>>>>>> componentrefactor
}

TSet<const URTSVisionComponent*> UVisionSubsystem::GetEnemyVisionComps() const
{
   TSet<const URTSVisionComponent*> enemyVisionComps;
<<<<<<< HEAD
   const auto                 enemySeesSomeone   = [](const AUnit* enemyUnit) { return enemyUnit->GetVisionComponent()->GetPossibleVisibleEnemies().Num() > 0; };
   const auto                 getEnemyVisionComp = [](const AUnit* enemyUnit) { return enemyUnit->GetVisionComponent(); };
=======
   const auto                       enemySeesSomeone = [](const AUnit* enemyUnit) {
      return enemyUnit->GetVisionComponent()->GetPossibleVisibleEnemies().Num() > 0;
   };

   const auto getEnemyVisionComp = [](const AUnit* enemyUnit) {
      return enemyUnit->GetVisionComponent();
   };
>>>>>>> componentrefactor
   Algo::TransformIf(gameStateRef->GetAllEnemyUnits(), enemyVisionComps, enemySeesSomeone, getEnemyVisionComp);
   return enemyVisionComps;
}

void UVisionSubsystem::UpdateVisibleEnemies()
{
<<<<<<< HEAD
   /** Uses 3 optimzations:
    * Keeps track of what possible enemies allies know about with overlap events so we don't have to trace between every ally - enemy pair
    * Keeps track of what enemies passed the test so we don't have to trace if a pair already exists for an enemy
    * Loops through only the visible enemies at the beginning of the test (before we add more) to see which ones are not visible anymore (removed cause we use sets)
    */

   SCOPE_CYCLE_COUNTER(STAT_UnitVision)
   {
      TSet<AUnit*>                     lastVisibleEnemies;
      const TSet<const URTSVisionComponent*> allyVisionToIterateOver = GetFriendlyVisionComps();

      /** By reference capture according to this https://en.cppreference.com/w/cpp/language/lambda in Lambda Capture section
         * A thread is launched for each number from {0:Num)
         * Each thread tries to get a pointer to an allied unit inside allyList (accessing the set by order using FSetElementId::FromInteger
         * If a unit dies while we are trying to access it, the unit won't be instantly GC'd so the pointer should be ok
         * But it will update the allyList so that we don't iterate it again
         * -- Possible Race Condition --
         * After some kind of delay (hopefully the delay is long enough) the unit will actually be GC'd. But the thread referencing it should be long gone
         * This won't work if the thread referencing the unit is still running after it is GC'd. This means we have way too many enemies to loop through
         * Or too many ally threads were spawned
      */

      ParallelForWithPreWork(
      allyVisionToIterateOver.Num(),
      [this, &allyVisionToIterateOver](const int32 curIndx) {
         const URTSVisionComponent* allyVision = allyVisionToIterateOver[FSetElementId::FromInteger(curIndx)];
         allyVision->visionMutex.ReadLock();
         for(AUnit* enemy : allyVision->GetPossibleVisibleEnemies()) {
            if(CheckUnitInVision(enemy, allyVision, visibleMutex, visibleEnemies))
               AddVisibleEnemy(enemy);
         }
         allyVision->visionMutex.ReadUnlock();
      },
      [this, &lastVisibleEnemies]() { StoreEnemiesVisibleLastCall(lastVisibleEnemies); });

      /// --Non multi-threaded code below runs after parallel work--

      MakeEnemiesInVisionVisible();
      MakeEnemiesOutOfVisionInvisible(lastVisibleEnemies);
=======
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
>>>>>>> componentrefactor
   }
}

void UVisionSubsystem::UpdateVisiblePlayerUnits()
{
<<<<<<< HEAD
   visiblePlayerUnits.Empty(visiblePlayerUnits.Num());
   const TSet<const URTSVisionComponent*> enemyVisionToIterateOver = GetEnemyVisionComps();

   ParallelFor(gameStateRef->GetAllEnemyUnits().Num(), [this, &enemyVisionToIterateOver](const int32 curIndx) {
      const URTSVisionComponent* enemyVision = enemyVisionToIterateOver[FSetElementId::FromInteger(curIndx)];
      enemyVision->visionMutex.ReadLock();

      for(AUnit* ally : enemyVision->GetPossibleVisibleEnemies()) {
         if(CheckUnitInVision(ally, enemyVision, visibleMutex, visiblePlayerUnits)) AddVisibleAlly(ally);
      }
      enemyVision->visionMutex.ReadUnlock();
   });
}

void UVisionSubsystem::StoreEnemiesVisibleLastCall(TSet<AUnit*>& lastCallCache)
{
   visibleMutex.WriteLock();
   Swap(lastCallCache, visibleEnemies);
   visibleMutex.WriteUnlock();
}

bool UVisionSubsystem::CheckUnitInVision(AUnit* unit, const URTSVisionComponent* visionComp, FWindowsRWLock& unitListMutex, TSet<AUnit*>& visibleUnits)
{
   unit->GetVisionComponent()->visionMutex.ReadLock();
   // If enemy hasn't been checked yet so we don't do it twice.  Also make sure we weren't waiting for enemy destruction with the IsValid check.
   // Once the writer mutex has been freed, the enemy should no longer be valid
   unitListMutex.ReadLock();
   if(IsValid(unit) && !visibleUnits.Contains(unit)) {
      unitListMutex.ReadUnlock();
      if(LineOfSightToNonInvisUnit(unit, visionComp)) return true;

   } else
      unitListMutex.ReadUnlock();
   unit->GetVisionComponent()->visionMutex.ReadUnlock();
   return false;
}

bool UVisionSubsystem::LineOfSightToNonInvisUnit(AUnit* unit, const URTSVisionComponent* allyVision)
{
   if(UNLIKELY(!USpellDataLibrary::IsInvisible(unit->GetAbilitySystemComponent()))) {
      if(!GetWorld()->LineTraceSingleByChannel(visionHitResult, allyVision->GetOwner()->GetActorLocation(), unit->GetActorLocation(), UNIT_VISION_CHANNEL)) {
=======
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
>>>>>>> componentrefactor
         return true;
      }
   }
   return false;
}

<<<<<<< HEAD
void UVisionSubsystem::MakeEnemiesInVisionVisible()
{
   for(auto enemy : visibleEnemies) {
      if(!enemy->GetCapsuleComponent()->IsVisible()) {
         // Can't change flags like visibility in threads
         enemy->GetCapsuleComponent()->SetVisibility(true, true);
         enemy->GetCapsuleComponent()->SetCollisionResponseToChannel(SELECTABLE_BY_CLICK_CHANNEL, ECollisionResponse::ECR_Block); // not selectable by clicks
=======
void UVisionSubsystem::MakeUnitsInVisionVisible(TSet<AUnit*>& unitsEligibleForHidingVisibleLastCheck)
{
   for(AUnit* unit : unitsEligibleForHidingVisibleLastCheck)
   {
      if(unit)
      {
         unit->SetIsUnitHidden(false);
>>>>>>> componentrefactor
      }
   }
}

<<<<<<< HEAD
void UVisionSubsystem::MakeEnemiesOutOfVisionInvisible(TSet<AUnit*>& lastVisibleEnemies) const
{
   for(AUnit* visibleEnemy : lastVisibleEnemies) {
      if(!visibleEnemies.Contains(visibleEnemy)) {
         visibleEnemy->GetCapsuleComponent()->SetVisibility(false, true);
         visibleEnemy->GetCapsuleComponent()->SetCollisionResponseToChannel(SELECTABLE_BY_CLICK_CHANNEL, ECollisionResponse::ECR_Ignore);
=======
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
>>>>>>> componentrefactor
      }
   }
}
