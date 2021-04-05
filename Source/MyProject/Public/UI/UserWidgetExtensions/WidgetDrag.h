#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "WidgetDrag.generated.h"

/**
 * Drag and drop information for entire widget drag and dropping
 */
UCLASS()
class MYPROJECT_API UWidgetDrag : public UDragDropOperation
{
   GENERATED_BODY()

 public:
   // Reference to widget we need to drag so when we get the operation again, we can grab this information
   UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
   UUserWidget* widgetToDrag; 

   // Keep this at 0 if autosize
   UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
   FVector2D size;
};
