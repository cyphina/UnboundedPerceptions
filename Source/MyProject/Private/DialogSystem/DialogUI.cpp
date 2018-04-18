// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "DialogUI.h"
#include "UserInput.h"
#include "UI/HUDManager.h"

void UDialogUI::SetMainView()
{
	if (bConvView)
		SetConversationView();
	else
		SetIntimateView();
}

bool UDialogUI::OnWidgetAddToViewport_Implementation()
{
	//change view to whatever it is set to prior
	SetMainView(); 
	return true;
}

