// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "FindVisibleEnemiesCenterContext.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "WorldObjects/Enemies/Enemy.h"
#include "RTSGameState.h"

void UFindVisibleEnemiesCenterContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
   Super::ProvideContext(QueryInstance, ContextData);

   FVector centerPointOfVisibleEnemies = FVector::ZeroVector;
   const auto& visibleEnemies = GetVisibleEnemies(QueryInstance);

   for(AUnit* enemy : visibleEnemies) {
      centerPointOfVisibleEnemies += enemy->GetActorLocation();
   }

   if(visibleEnemies.Num() != 0) centerPointOfVisibleEnemies /= visibleEnemies.Num();

   UEnvQueryItemType_Point::SetContextHelper(ContextData, centerPointOfVisibleEnemies);
}

const TSet<AUnit*>&  UFindVisibleEnemiesCenterContext::GetVisibleEnemies(const FEnvQueryInstance& queryInstance) const
{
   const auto& visionContext = Cast<IVisionContext>(queryInstance.Owner.Get()->GetWorld()->GetGameState());
   return visionContext->GetVisibleEnemies();
}

