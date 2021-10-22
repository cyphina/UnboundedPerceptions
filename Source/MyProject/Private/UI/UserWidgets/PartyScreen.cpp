// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UI/UserWidgets/PartyScreen.h"

#include "BasePlayer.h"
#include "UserInput.h"

bool UPartyScreen::OnWidgetAddToViewport_Implementation()
{
	CPC->GetBasePlayer()->ClearSelectedUnits();
	
	return Super::OnWidgetAddToViewport_Implementation();
}
