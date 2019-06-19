// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "FindVisibleEnemiesCenterContext.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "WorldObjects/Enemies/Enemy.h"
#include "RTSGameState.h"

void UFindVisibleEnemiesCenterContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
   Super::ProvideContext(QueryInstance, ContextData);
   ARTSGameState* gameStateRef = Cast<ARTSGameState>(QueryInstance.Owner.Get()->GetWorld()->GetGameState());
   if (gameStateRef) {

      FVector centerPointOfVisibleEnemies = FVector::ZeroVector;
      for (AUnit* enemy : gameStateRef->visibleEnemies) {
         centerPointOfVisibleEnemies += enemy->GetActorLocation();
      }

      if (gameStateRef->visibleEnemies.Num() != 0) centerPointOfVisibleEnemies /= gameStateRef->visibleEnemies.Num();

      UEnvQueryItemType_Point::SetContextHelper(ContextData, centerPointOfVisibleEnemies);
   }
}
