// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyUserWidget.h"
#include "MyDraggableWidget.generated.h"

/**
 * Widget with callback when first shown and draggable
 */
UCLASS()
class MYPROJECT_API UMyDraggableWidget : public UMyUserWidget
{
   GENERATED_BODY()

 public:
   FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
   void   NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
};