// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "WidgetDrag.generated.h"

/**
 * Drag and drop information for widget drag and dropping
 */
UCLASS()
class MYPROJECT_API UWidgetDrag : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		UUserWidget* widgetToDrag; //reference to widget we need to drag so when we get the operation again, we can grab this information	

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		FVector2D size; //Keep this at 0 if autosize
};
