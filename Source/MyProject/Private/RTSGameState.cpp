// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSGameState.h"
#include "UserInput.h"
#include "MainWidget.h"
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
   GetWorld()->GetTimerManager().SetTimer(enemyVisionUpdateTimerHandle, this, &ARTSGameState::UpdateVisibleAllies, .1, true);

   FOWplane = GetWorld()->SpawnActor<AFogOfWarPlane>(FOWplaneClass, FVector(0, 0, 0), FRotator());
}

void ARTSGameState::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);
   clockwork += deltaSeconds * timeUnit;
   Clock();

   // if(floor(fmod(clockwork,SECONDS_IN_DAY) < SMALL_NUMBER))
   //{
   //	++days;
   //	Calendar();
   //}
}

void ARTSGameState::Clock()
{
   seconds = floor(fmod(clockwork, 60));
   minutes = floor(fmod(clockwork / 60, 60));
   if (hours == 24) {
      hours = floor(fmod(clockwork / 3600, 24)) + 1;
      if (hours == 0) // branching conditional faster than modulo?
      {
         ++days;
         Calendar();
         clockwork = 0;
      }
   }

   gameTime[0] = seconds;
   gameTime[1] = minutes;
   gameTime[2] = hours;

   mainWidgetRef->SetClock(gameTime);
}

void ARTSGameState::Calendar()
{
   if (days > UKismetMathLibrary::DaysInMonth(years, months)) {
      days = 1;
      ++months;
      if (months > 12) {
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

   mainWidgetRef->SetDate(gameDate);
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
      TSet<AEnemy*> lastVisibleEnemies;
      Swap(lastVisibleEnemies, visibleEnemies);

      for (AAlly* ally : allyList) {
         for (AEnemy* enemy : ally->possibleEnemiesInRadius) {
            // if enemy hasn't been checked yet so we don't do it twice
            if (!visibleEnemies.Contains(enemy)) {
               // If we can trace a line and hit the enemy without hitting a wall
               if (!GetWorld()->LineTraceSingleByObjectType(visionHitResult, ally->GetActorLocation(), enemy->GetActorLocation(), queryParamVision)) {
                  //If the enemy isn't invisible
                  if (!enemy->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Invisibility")) ||
                     ally->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.TrueSight"))) {
                     //Make it visible
                     if (!enemy->GetCapsuleComponent()->bVisible) { enemy->GetCapsuleComponent()->SetVisibility(true, true); }
                     visibleEnemies.Add(enemy);
                  }
               }
            }
         }
      }

      //If there's an ally that is not visible this time, but was last time, make sure he's now invisible
      for (AEnemy* visibleEnemy : lastVisibleEnemies) {
         if (!visibleEnemies.Contains(visibleEnemy)) { visibleEnemy->GetCapsuleComponent()->SetVisibility(false, true); }
      }
   }
}

void ARTSGameState::UpdateVisibleAllies()
{
   visibleAllies.Empty(visibleAllies.Num());

   for (AEnemy* enemy : enemyList) {
      for (AAlly* ally : enemy->possibleEnemiesInRadius) {
         // if ally hasn't been checked yet so we don't do it twice
         if (!visibleAllies.Contains(ally)) {
            // If we can trace a line and hit the ally without hitting a wall
            if (!GetWorld()->LineTraceSingleByObjectType(visionHitResult, enemy->GetActorLocation(), ally->GetActorLocation(), queryParamVision)) {
               if (!ally->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Invisibility")) ||
                  enemy->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.TrueSight"))) {
                  // make it visible
                  visibleAllies.Add(ally);
               }
            }
         }
      }
   }
}
