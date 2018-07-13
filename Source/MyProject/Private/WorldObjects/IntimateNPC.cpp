// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "IntimateNPC.h"
#include "RTSGameMode.h"
#include "UserInput.h"
#include "UI/HUDManager.h"
#include "DialogSystem/DialogUI.h"

AIntimateNPC::AIntimateNPC() : ANPC()
{
	
}

void AIntimateNPC::BeginPlay()
{
	Super::BeginPlay();
	gameModeRef = Cast<ARTSGameMode>(GetGameInstance());
}

void AIntimateNPC::SetRelationshipPoints(int pointsToAdd)
{
	relationshipPoints += pointsToAdd;
	if (relationshipPoints > relationshipEventPointValues[currentRelationshipEventIndex])
	{
		gameModeRef->GetTriggerManager()->ActivateTrigger(relationshipTriggers[currentRelationshipEventIndex]);
	}
}

void AIntimateNPC::Interact_Implementation(ABaseHero* hero)
{
	Super::Interact_Implementation(hero);
}

void AIntimateNPC::SetupAppropriateView()
{
	controllerRef->GetHUDManager()->GetSocialWindow()->SetIntimateView();
}

