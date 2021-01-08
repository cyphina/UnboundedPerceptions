// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "MyUserWidget.h"
#include "UserInput.h"

void UMyUserWidget::OnWidgetRemovedFromViewport_Implementation()
{
}

void UMyUserWidget::NativeConstruct()
{
   CPC = Cast<AUserInput>(GetOwningPlayer());
   Super::NativeConstruct();
}

bool UMyUserWidget::OnWidgetAddToViewport_Implementation()
{
   return true;
}
