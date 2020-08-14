// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSGameState.h"
#include "ParallelFor.h"
#include "RTSIngameWidget.h"
#include "WorldObjects/Ally.h"
#include "WorldObjects/Enemies/Enemy.h"
#include "FogOfWar/FogOfWarPlane.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"

DECLARE_CYCLE_STAT(TEXT("Unit Vision Update"), STAT_UnitVision, STATGROUP_RTSUnits)

ARTSGameState::ARTSGameState()
{
   seconds              = 0;
   minutes              = 0;
   hours                = 1;
   days                 = 15;
   months               = 11;
   years                = 2020;
   defaultGameTimeSpeed = 60;
   clockwork            = 0;
   queryParamVision.AddObjectTypesToQuery(ECC_GameTraceChannel6); // Query vision blockers only
}

void ARTSGameState::BeginPlay()
{
   Super::BeginPlay();
   // if there's some default settings for time, set it now
   clockwork += seconds + minutes * 60 + hours * 3600;

   // set game date
   gameDate.Init(0, arraySize);
   gameTime.Init(0, arraySize);
   gameDate[0] = days;
   gameDate[1] = months;
   gameDate[2] = years;

   // match initial speed
   timeUnit = 0;
   UpdateGameSpeedDelegate.AddDynamic(this, &ARTSGameState::UpdateGameSpeed);

   // Server should check the vision of all friendlies and replicate the vision data accordintly
   GetWorld()->GetTimerManager().SetTimer(allyVisionUpdateTimerHandle, this, &ARTSGameState::UpdateVisibleEnemies, .1, true);
   GetWorld()->GetTimerManager().SetTimer(enemyVisionUpdateTimerHandle, this, &ARTSGameState::UpdateVisiblePlayerUnits, .1, true);

   // Don't really use this ATM but it's there if we want to play around with it and actually make it a thing we can increase the precision of the model by adding more polygons to the plane
   // and performing more line traces. However it is quite inefficient since we would have to do this for every ally...
   // Implemented via https://www.redblobgames.com/articles/visibility/.
   // ! Not using this currently however...
   FOWplane = GetWorld()->SpawnActor<AFogOfWarPlane>(FOWplaneClass, FVector(0, 0, 0), FRotator());

   URTSVisionComponent::unitNoLongerInVisionSpheres.AddUObject(this, &ARTSGameState::OnEnemyOutsideVisionSpheres);
}

void ARTSGameState::StopUpdate()
{
   GetWorld()->GetTimerManager().PauseTimer(allyVisionUpdateTimerHandle);
   GetWorld()->GetTimerManager().PauseTimer(enemyVisionUpdateTimerHandle);
   visibleEnemies.Empty();
   visiblePlayerUnits.Empty();
}

void ARTSGameState::ResumeUpdate() const
{
   GetWorld()->GetTimerManager().UnPauseTimer(allyVisionUpdateTimerHandle);
   GetWorld()->GetTimerManager().UnPauseTimer(enemyVisionUpdateTimerHandle);
}

void ARTSGameState::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);
   clockwork += deltaSeconds * timeUnit;
   Clock();
}

void ARTSGameState::Clock()
{
   seconds = floor(fmod(clockwork, 60));
   minutes = floor(fmod(clockwork / 60, 60));
   if(UNLIKELY(hours == 24)) {
      hours = floor(fmod(clockwork / 3600, 24)) + 1;
      if(UNLIKELY(hours == 0)) // branching conditional faster than modulo?
      {
         ++days;
         Calendar();
         clockwork = 0;
      }
   }

   gameTime[0] = seconds;
   gameTime[1] = minutes;
   gameTime[2] = hours;

   ingameWidget->SetClock(gameTime);
}

void ARTSGameState::Calendar()
{
   if(UNLIKELY(days > UKismetMathLibrary::DaysInMonth(years, months))) {
      days = 1;
      ++months;
      if(months > 12) {
         months = 1;
         ++years;
      }
      gameDate[0] = days;
      gameDate[1] = months;
      gameDate[2] = years;
   }
   gameDate[0] = days;
   gameDate[1] = months;
   gameDate[2] = years;

   ingameWidget->SetDate(gameDate);
}

void ARTSGameState::UpdateGameSpeed(float speedMultiplier)
{
   speedModifier = speedMultiplier;
   timeUnit      = speedModifier * defaultGameTimeSpeed;
}

void ARTSGameState::UpdateGameTime(int secs, int mins, int hrss)
{
   clockwork = clockwork + seconds + minutes * 60 + hours * 3600;
}

void ARTSGameState::UpdateGameDay(int day, int month, int year)
{
   gameDate[0] = days;
   gameDate[1] = months;
   gameDate[2] = years;
}

void ARTSGameState::AddGameTime(FDateTime timeToAdd)
{
}

void ARTSGameState::UpdateVisibleEnemies()
{
   /**Uses 3 optimzations:
    *Keeps track of what possible enemies allies know about with overlap events so we don't have to trace between every ally - enemy pair
    *Keeps track of what enemies passed the test so we don't have to trace if a pair already exists for an enemy
    *Loops through only the visible enemies at the beginning of the test (before we add more) to see which ones are not visible anymore (removed cause we use sets)
    */

   SCOPE_CYCLE_COUNTER(STAT_UnitVision)
   {
      TSet<AUnit*> lastVisibleEnemies;

      // By reference capture according to this https://en.cppreference.com/w/cpp/language/lambda in Lambda Capture section
      // A thread is launched for each number from {0:Num)
      // Each thread tries to get a pointer to an allied unit inside allyList (accessing the set by order using FSetElementId::FromInteger
      // If a unit dies while we are trying to access it, the unit won't be instantly GC'd so the pointer should be ok
      // But it will update the allyList so that we don't iterate it again
      // Then after some kind of delay (hopefully the delay is long enough) the unit will actually be GC'd. But the thread referencing it should be long gone
      // This won't work if the thread referencing the unit is still running after it is GC'd. This means we have way too many enemies to loop through
      // Or too many ally threads were spawned

      ParallelForWithPreWork(
          allyList.Num(),
          [this](int32 curIndx) {
             AAlly* ally = allyList[FSetElementId::FromInteger(curIndx)];
             ally->visionMutex.ReadLock();
             for(AUnit* enemy : ally->possibleEnemiesInRadius) {
                enemy->visionMutex.ReadLock();
                // If enemy hasn't been checked yet so we don't do it twice.  Also make sure we weren't waiting for enemy destruction with the IsValid check.
                // Once the writer mutex has been freed, the enemy should no longer be valid
                visibleMutex.ReadLock();
                if(IsValid(enemy) && !visibleEnemies.Contains(enemy)) {
                   visibleMutex.ReadUnlock();
                   // If we can trace a line and hit the enemy without hitting a wall (unitVisionTrace channel)
                   if(!GetWorld()->LineTraceSingleByChannel(visionHitResult, ally->GetActorLocation(), enemy->GetActorLocation(), UNIT_VISION_CHANNEL)) {
                      // If the enemy isn't invisible
                      if(UNLIKELY(!enemy->IsInvisible())) {
                         // Make it visible

                         visibleMutex.WriteLock();
                         visibleEnemies.Add(enemy);
                         visibleMutex.WriteUnlock();
                      }
                   }
                } else
                   visibleMutex.ReadUnlock();
                enemy->visionMutex.ReadUnlock();
             }
             ally->visionMutex.ReadUnlock();
          },
          [this, &lastVisibleEnemies]() {
             // Code ran before the threads are created
             visibleMutex.WriteLock();
             // Save the enemies we saw last and then we'll update what we see
             Swap(lastVisibleEnemies, visibleEnemies);
             visibleMutex.WriteUnlock();
          });

      /// --Non multi-threaded code below runs after parallel work--

      for(auto enemy : visibleEnemies) {
         if(!enemy->GetCapsuleComponent()->IsVisible()) {
            // Can't change flags like visibility in threads
            enemy->GetCapsuleComponent()->SetVisibility(true, true);
            enemy->GetCapsuleComponent()->SetCollisionResponseToChannel(SELECTABLE_BY_CLICK_CHANNEL, ECollisionResponse::ECR_Block); // not selectable by clicks
         }
      }

      // If there's an ally that is not visible this time, but was last time, make sure he's now invisible
      // Even if due to timing issues visibleEnemies doesn't have the right units inside it, it doesn't matter because the timing is so narrow and this will be rerun
      for(AUnit* visibleEnemy : lastVisibleEnemies) {
         if(!visibleEnemies.Contains(visibleEnemy)) {
            visibleEnemy->GetCapsuleComponent()->SetVisibility(false, true);
            visibleEnemy->GetCapsuleComponent()->SetCollisionResponseToChannel(SELECTABLE_BY_CLICK_CHANNEL, ECollisionResponse::ECR_Ignore);
         }
      }
   }
}

void ARTSGameState::UpdateVisiblePlayerUnits()
{
   visiblePlayerUnits.Empty(visiblePlayerUnits.Num());

   ParallelFor(enemyList.Num(), [this](int32 curIndx) {
      AEnemy* enemy = enemyList[FSetElementId::FromInteger(curIndx)];
      if(IsValid(enemy)) {
         enemy->visionMutex.ReadLock();

         for(AUnit* ally : enemy->possibleEnemiesInRadius) {
            ally->visionMutex.ReadLock();
            // if ally hasn't been checked yet so we don't do it twice
            visiblePlayersMutex.ReadLock();
            if(!visiblePlayerUnits.Contains(ally)) {
               visiblePlayersMutex.ReadUnlock();
               // If we can trace a line and hit the ally without hitting a wall
               if(!GetWorld()->LineTraceSingleByChannel(visionHitResult, enemy->GetActorLocation(), ally->GetActorLocation(), UNIT_VISION_CHANNEL)) {
                  if(UNLIKELY(!ally->IsInvisible())) {
                     // make it visible
                     visiblePlayersMutex.WriteLock();
                     visiblePlayerUnits.Add(ally);
                     visiblePlayersMutex.WriteUnlock();
                  }
               }
            } else
               visiblePlayersMutex.ReadUnlock();
            ally->visionMutex.ReadUnlock();
         }
         enemy->visionMutex.ReadUnlock();
      }
   });
}

void ARTSGameState::OnEnemyOutsideVisionSpheres(AUnit* enemy)
{
   visibleMutex.WriteLock();
   visibleEnemies.Remove(enemy);
   visibleMutex.WriteUnlock();
}
