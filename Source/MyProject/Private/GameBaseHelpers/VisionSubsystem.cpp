// Created 9/19/20 1:43 PM

#include "MyProject.h"
#include "VisionSubsystem.h"

#include "ParallelFor.h"
#include "RTSGameMode.h"
#include "RTSGameState.h"
#include "SpellDataLibrary.h"
#include "Transform.h"
#include "Unit.h"
#include "WindowsCriticalSection.h"

DECLARE_CYCLE_STAT(TEXT("Unit Vision Update"), STAT_UnitVision, STATGROUP_RTSUnits)

namespace VisionCVars
{
   static float VisionUpdateLoopTime = .1f;
   static FAutoConsoleVariableRef
       CVarNumSecondsBeforeShowingInMinutes(TEXT("VisionUpdateLoopTime"), VisionUpdateLoopTime,
                                            TEXT("How long between vision updates in seconds"),
                                            ECVF_Default);
}

UVisionSubsystem::UVisionSubsystem()
{
   queryParamVision.AddObjectTypesToQuery(VISION_BLOCKER_CHANNEL); // Query vision blockers only
}

TUniquePtr<UVisionSubsystem> UVisionSubsystem::Create(UObject* outer)
{
   auto newVisionManager = NewObject<UVisionSubsystem>(outer);
   if(ARTSGameMode* gameModeRef = Cast<ARTSGameMode>(newVisionManager->GetWorld()->GetAuthGameMode())) {
      newVisionManager->StartUpdating();
      gameModeRef->OnLevelLoaded.AddDynamic(newVisionManager, &UVisionSubsystem::ResumeUpdating);
      gameModeRef->OnLevelAboutToUnload.AddDynamic(newVisionManager, &UVisionSubsystem::StopUpdating);

      newVisionManager->StartUpdating();
      return TUniquePtr<UVisionSubsystem>(newVisionManager);
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
}

void UVisionSubsystem::AddVisibleAlly(AUnit* newAlly)
{
   FRWScopeLock lockPlayers(visiblePlayersMutex, FRWScopeLockType::SLT_Write);
   visiblePlayerUnits.Add(newAlly);
}

void UVisionSubsystem::AddVisibleEnemy(AUnit* newEnemy)
{
   FRWScopeLock lock(visibleMutex, FRWScopeLockType::SLT_Write);
   visibleEnemies.Add(newEnemy);
}

TSet<URTSVisionComponent*> UVisionSubsystem::GetFriendlyVisionComps() const
{
   TSet<URTSVisionComponent*> friendlyVisionComps;
   Algo::TransformIf(
       gameStateRef->GetAllFriendlyUnits(), friendlyVisionComps,
       [](const AUnit*& friendlyUnit) { return friendlyUnit->GetVisionComponent()->possibleVisibleEnemies.Num() > 0; },
       [](const AUnit*& friendlyUnit) { return friendlyUnit->GetVisionComponent(); });
   return friendlyVisionComps;
}

TSet<URTSVisionComponent*> UVisionSubsystem::GetEnemyVisionComps() const
{
   TSet<URTSVisionComponent*> enemyVisionComps;
   Algo::TransformIf(
       gameStateRef->GetAllEnemyUnits(), enemyVisionComps, [](const AUnit*& enemyUnit) { return enemyUnit->GetVisionComponent()->possibleVisibleEnemies.Num() > 0; },
       [](const AUnit*& enemyUnit) { return enemyUnit->GetVisionComponent(); });
   return enemyVisionComps;
}

void UVisionSubsystem::UpdateVisibleEnemies()
{
   /** Uses 3 optimzations:
    * Keeps track of what possible enemies allies know about with overlap events so we don't have to trace between every ally - enemy pair
    * Keeps track of what enemies passed the test so we don't have to trace if a pair already exists for an enemy
    * Loops through only the visible enemies at the beginning of the test (before we add more) to see which ones are not visible anymore (removed cause we use sets)
    */

   SCOPE_CYCLE_COUNTER(STAT_UnitVision)
   {
      TSet<AUnit*>                     lastVisibleEnemies;
      const TSet<URTSVisionComponent*> allyVisionToIterateOver = GetFriendlyVisionComps();

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
             URTSVisionComponent* allyVision = allyVisionToIterateOver[FSetElementId::FromInteger(curIndx)];
             allyVision->visionMutex.ReadLock();
             for(AUnit* enemy : allyVision->possibleVisibleEnemies) {
                if(CheckUnitInVision(enemy, allyVision, visibleMutex, visibleEnemies)) AddVisibleEnemy(enemy);
             }
             allyVision->visionMutex.ReadUnlock();
          },
          [this, &lastVisibleEnemies]() { StoreEnemiesVisibleLastCall(lastVisibleEnemies); });

      /// --Non multi-threaded code below runs after parallel work--

      MakeEnemiesInVisionVisible();
      MakeEnemiesOutOfVisionInvisible(lastVisibleEnemies);
   }
}

void UVisionSubsystem::UpdateVisiblePlayerUnits()
{
   visiblePlayerUnits.Empty(visiblePlayerUnits.Num());
   const TSet<URTSVisionComponent*> enemyVisionToIterateOver = GetEnemyVisionComps();

   ParallelFor(gameStateRef->GetAllEnemyUnits().Num(), [this, &enemyVisionToIterateOver](const int32 curIndx) {
      URTSVisionComponent* enemyVision = enemyVisionToIterateOver[FSetElementId::FromInteger(curIndx)];
      enemyVision->visionMutex.ReadLock();

      for(AUnit* ally : enemyVision->possibleVisibleEnemies) {
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

bool UVisionSubsystem::CheckUnitInVision(AUnit* unit, URTSVisionComponent* visionComp, FWindowsRWLock& unitListMutex, TSet<AUnit*>& visibleUnits)
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

bool UVisionSubsystem::LineOfSightToNonInvisUnit(AUnit* unit, URTSVisionComponent* allyVision)
{
   if(UNLIKELY(!USpellDataLibrary::IsInvisible(*unit->GetAbilitySystemComponent()))) {
      if(!GetWorld()->LineTraceSingleByChannel(visionHitResult, allyVision->GetOwner()->GetActorLocation(), unit->GetActorLocation(), UNIT_VISION_CHANNEL)) {
         return true;
      }
   }
   return false;
}

void UVisionSubsystem::MakeEnemiesInVisionVisible()
{
   for(auto enemy : visibleEnemies) {
      if(!enemy->GetCapsuleComponent()->IsUnitVisible()) {
         // Can't change flags like visibility in threads
         enemy->GetCapsuleComponent()->SetVisibility(true, true);
         enemy->GetCapsuleComponent()->SetCollisionResponseToChannel(SELECTABLE_BY_CLICK_CHANNEL, ECollisionResponse::ECR_Block); // not selectable by clicks
      }
   }
}

void UVisionSubsystem::MakeEnemiesOutOfVisionInvisible(TSet<AUnit*>& lastVisibleEnemies) const
{
   for(AUnit* visibleEnemy : lastVisibleEnemies) {
      if(!visibleEnemies.Contains(visibleEnemy)) {
         visibleEnemy->GetCapsuleComponent()->SetVisibility(false, true);
         visibleEnemy->GetCapsuleComponent()->SetCollisionResponseToChannel(SELECTABLE_BY_CLICK_CHANNEL, ECollisionResponse::ECR_Ignore);
      }
   }
}
