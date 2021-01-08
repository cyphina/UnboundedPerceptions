// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "VisibleEnemiesQueryGenerator.h"
#include "RTSGameState.h"
#include "WorldObjects/Enemies/Enemy.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

UVisibleEnemiesQueryGenerator::UVisibleEnemiesQueryGenerator()
{
   ItemType = UEnvQueryItemType_Actor::StaticClass();
}

void UVisibleEnemiesQueryGenerator::GenerateItems(FEnvQueryInstance& queryInstance) const
{
   ARTSGameState*  gameStateRef = Cast<ARTSGameState>(GEngine->GetWorldFromContextObject(queryInstance.Owner.Get(), EGetWorldErrorMode::LogAndReturnNull)->GetGameState());
   TArray<AActor*> matchingActors;

   for (AUnit* enemy : gameStateRef->GetVisibleEnemies()) {
      matchingActors.Add(enemy);
   }

   queryInstance.AddItemData<UEnvQueryItemType_Actor>(matchingActors);
}

FText UVisibleEnemiesQueryGenerator::GetDescriptionTitle() const
{
   return NSLOCTEXT("EnvQueryGenerator", "VisibleEnemiesGenerator", "Generates enemies in ally vision");
}
