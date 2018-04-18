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
	UFUNCTION(BlueprintNativeEvent, BlueprintCosmetic, Category = "Mouse")
		FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
	virtual FEventReply OnMouseButtonDown_Implementation(FGeometry MyGeometry, const FPointerEvent& MouseEvent); //Setup offset and detect drag

	UFUNCTION(BlueprintNativeEvent, BlueprintCosmetic, Category = "Drag and Drop")
		void OnDragDetected(FGeometry MyGeometry, const FPointerEvent& PointerEvent, UDragDropOperation*& Operation);
	virtual void OnDragDetected_Implementation(FGeometry MyGeometry, const FPointerEvent& PointerEvent, UDragDropOperation*& Operation);
};
