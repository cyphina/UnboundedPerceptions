// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "MyUserWidget.h"
#include "UserInput.h"

void UMyUserWidget::NativeConstruct()
{
   CPC = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
   Super::NativeConstruct();
}

bool UMyUserWidget::OnWidgetAddToViewport_Implementation()
{
   return true;
}
