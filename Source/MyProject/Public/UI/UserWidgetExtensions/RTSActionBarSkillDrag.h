#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "RTSActionBarSkillDrag.generated.h"

/**
 * Used when we drag off a spell on our action bar to rearrange its location or to remove it
 */
UCLASS()
class MYPROJECT_API URTSActionBarSkillDrag : public UDragDropOperation
{
   GENERATED_BODY()

public:
   UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
   int slotIndex;
};
