// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "IntimateNPC.h"
#include "MyGameInstance.h"
#include "UserInput.h"
#include "UI/HUDManager.h"
#include "DialogSystem/DialogUI.h"

AIntimateNPC::AIntimateNPC() : ANPC()
{
	
}

void AIntimateNPC::BeginPlay()
{
	gameInstanceRef = Cast<UMyGameInstance>(GetGameInstance());
}

void AIntimateNPC::SetRelationshipPoints(int pointsToAdd)
{
	relationshipPoints += pointsToAdd;
	if (relationshipPoints > relationshipEventPointValues[currentRelationshipEventIndex])
	{
		gameInstanceRef->GetTriggerManager()->ActivateTrigger(relationshipTriggers[currentRelationshipEventIndex]);
	}
}

void AIntimateNPC::Interact_Implementation(ABaseHero* hero)
{
	if (GetWantsToConverse())
	{
		controllerRef->GetHUDManager()->GetSocialWindow()->SetNPC(this);
		controllerRef->GetHUDManager()->GetSocialWindow()->SetIntimateView();
		controllerRef->GetHUDManager()->AddHUDDialog(GetStartingConversationName(), GetOnDialogFinishedTrigger());
	}
	else
		controllerRef->GetHUDManager()->AddHUDDialog(GetStartingConversationName(), FTriggerData::defaultTrigger);
}

