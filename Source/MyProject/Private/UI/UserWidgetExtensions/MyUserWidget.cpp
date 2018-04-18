// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "MyUserWidget.h"
#include "UserInput.h"

void UMyUserWidget::Construct_Implementation()
{
	CPC = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()); 
}

bool UMyUserWidget::OnWidgetAddToViewport_Implementation()
{	
	return true;
}

