// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "DialogUI.h"
#include "UserInput.h"
#include "UI/HUDManager.h"

void UDialogUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDialogUI::SetMainView()
{
	switch(socialHUDState)
	{
		case ESocialHUDState::Conversation: SetConversationView(); break;
		case ESocialHUDState::Intimate: SetIntimateView(); break;
		case ESocialHUDState::Shop: SetShopView(); break;
	}
}

bool UDialogUI::OnWidgetAddToViewport_Implementation()
{
	//change view to whatever it is set to prior
	//SetMainView(); 
	return true;
}

