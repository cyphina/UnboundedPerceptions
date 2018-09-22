// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UnitQueryGenerator.h"
#include "RTSGameState.h"
#include "WorldObjects/Unit.h"
#include "WorldObjects/Ally.h"
#include "Worldobjects/Enemies/Enemy.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

UUnitQueryGenerator::UUnitQueryGenerator()
{
	//unitClass = AUnit::StaticClass();
}

void UUnitQueryGenerator::GenerateItems(FEnvQueryInstance& queryInstance) const
{
	ARTSGameState* gameStateRef = Cast<ARTSGameState>(queryInstance.Owner.Get()->GetWorld()->GetGameState());

	TArray<AActor*> matchingActors;

	for(AAlly* unit : gameStateRef->allyList)
	{
		matchingActors.Add(unit);
	}

	for(AEnemy* unit : gameStateRef->enemyList)
	{
		matchingActors.Add(unit);
	}

	queryInstance.AddItemData<UEnvQueryItemType_Actor>(matchingActors);
}
