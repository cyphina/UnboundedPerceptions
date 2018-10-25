// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "VisibleAlliesQueryGenerator.h"
#include "RTSGameState.h"
#include "WorldObjects/Ally.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

UVisibleAlliesQueryGenerator::UVisibleAlliesQueryGenerator()
{
   ItemType = UEnvQueryItemType_Actor::StaticClass();
}

void UVisibleAlliesQueryGenerator::GenerateItems(FEnvQueryInstance& queryInstance) const
{
   ARTSGameState*  gameStateRef = Cast<ARTSGameState>(GEngine->GetWorldFromContextObject(queryInstance.Owner.Get(), EGetWorldErrorMode::LogAndReturnNull)->GetGameState());
   TArray<AActor*> matchingActors;

   for (AAlly* ally : gameStateRef->visibleAllies) {
      matchingActors.Add(ally);
   }

   queryInstance.AddItemData<UEnvQueryItemType_Actor>(matchingActors);
}

FText UVisibleAlliesQueryGenerator::GetDescriptionTitle() const
{
   return NSLOCTEXT("EnvQueryGenerator", "VisibleAlliesGenerator", "Generates allies in enemy vision");
}
