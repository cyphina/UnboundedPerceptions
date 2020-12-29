// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UnitQueryGenerator.h"
#include "RTSGameState.h"
#include "WorldObjects/Unit.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

UUnitQueryGenerator::UUnitQueryGenerator()
{
   // unitClass = AUnit::StaticClass();
}

void UUnitQueryGenerator::GenerateItems(FEnvQueryInstance& queryInstance) const
{
   IAllUnitsContext* allUnitsContext = Cast<IAllUnitsContext>(queryInstance.Owner.Get()->GetWorld()->GetGameState());

   TArray<AActor*> matchingActors;

   for(AUnit* unit : allUnitsContext->GetAllFriendlyUnits()) {
      matchingActors.Add(unit);
   }

   for(AUnit* unit : allUnitsContext->GetAllEnemyUnits()) {
      matchingActors.Add(unit);
   }

   queryInstance.AddItemData<UEnvQueryItemType_Actor>(matchingActors);
}
