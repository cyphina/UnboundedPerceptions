// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "ActionSlot.h"
#include "UserInput.h"
#include "WidgetBlueprintLibrary.h"

void UActionSlot::NativeConstruct()
{
   Super::NativeConstruct();
   CPCRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
}
