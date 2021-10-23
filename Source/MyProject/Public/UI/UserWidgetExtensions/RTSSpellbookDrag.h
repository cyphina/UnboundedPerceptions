#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "RTSSpellbookDrag.generated.h"

/**
 * Used when we drag off a spell in our spell book (to equip into our action bar)
 */
UCLASS()
class MYPROJECT_API URTSSpellbookDrag : public UDragDropOperation
{
   GENERATED_BODY()

public:
   UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
   int slotIndex;
};
