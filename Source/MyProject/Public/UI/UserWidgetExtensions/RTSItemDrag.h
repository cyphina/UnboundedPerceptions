#pragma once

#include "MyUserWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "RTSItemDrag.generated.h"

class UBackpack;

/**
 * Used when dragging item slots from one inventory to another (can't drag store inventories though)
 */
UCLASS()
class MYPROJECT_API URTSItemDrag : public UDragDropOperation
{
   GENERATED_BODY()

public:
   /** Describes the index of the slog dragged in terms of the backpack's indices not the index in the inventory view */
   UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
   int slotToBackpackIndex;

   UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
   UTexture2D* itemImage;

   UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
   UBackpack* backpack;

   /** Widget containing the slots including the slot this drag operation was created off of */
   UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
   UMyUserWidget* dragWidget;
};
