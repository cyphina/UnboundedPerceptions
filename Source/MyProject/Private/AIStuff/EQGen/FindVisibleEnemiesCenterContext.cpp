// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "FindVisibleEnemiesCenterContext.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "WorldObjects/Enemies/Enemy.h"
#include "UserInput.h"


void UFindVisibleEnemiesCenterContext::ProvideContext(FEnvQueryInstance& QueryInstance,
	FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);
	AUserInput* CPCRef = Cast<AUserInput>(QueryInstance.Owner.Get()->GetWorld()->GetFirstPlayerController());
	if(CPCRef)
	{
		
		FVector centerPointOfVisibleEnemies = FVector::ZeroVector;
		for(AActor* enemy : CPCRef->visibleEnemies)
		{
			centerPointOfVisibleEnemies += enemy->GetActorLocation();
		}

		if(CPCRef->visibleEnemies.Num() != 0)
			centerPointOfVisibleEnemies /= CPCRef->visibleEnemies.Num();

		UEnvQueryItemType_Point::SetContextHelper(ContextData, centerPointOfVisibleEnemies);
	}
}
