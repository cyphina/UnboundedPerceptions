// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "MyDraggableWidget.h"
#include "WidgetBlueprintLibrary.h"
#include "UserInput.h"
#include "WidgetDrag.h"

FEventReply UMyDraggableWidget::OnMouseButtonDown_Implementation(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	if (CPC)
	{
#if UE_EDITOR
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("On Mouse Button Down!"));
#endif
		CPC->offset = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition()); //set the offset so we can add it to the screen properly
		return UWidgetBlueprintLibrary::DetectDragIfPressed(MouseEvent, this, EKeys::LeftMouseButton);
	}
	return FEventReply();
}

void UMyDraggableWidget::OnDragDetected_Implementation(FGeometry MyGeometry, const FPointerEvent& PointerEvent, UDragDropOperation*& Operation)
{
	UWidgetDrag* dragOp = Cast<UWidgetDrag>(UWidgetBlueprintLibrary::CreateDragDropOperation(UWidgetDrag::StaticClass()));
	dragOp->DefaultDragVisual = this;
	dragOp->widgetToDrag = this; //set what widget we're dragging so we can figure out it later to add back to the screen
	dragOp->Pivot = EDragPivot::MouseDown;
	Operation = dragOp;
	RemoveFromParent();
}

