#pragma once

#include "CoreMinimal.h"
#include "Items/Inventory.h"
#include "StorageInventory.generated.h"

DECLARE_EVENT(UStorageInventory, FOnStorageInventoryClosed);

/**
 * Native base class for widget that stores items temporarily in some kind of interactable (storage box) type object.
 */
UCLASS()
class MYPROJECT_API UStorageInventory : public UInventory
{
   GENERATED_BODY()

public:
	FOnStorageInventoryClosed& OnStorageInventoryClosed() { return OnStorageInventoryClosedEvent; }
	
protected:
	bool OnWidgetAddToViewport_Implementation() override;
	void OnWidgetRemovedFromViewport_Implementation() override;

private:
	FOnStorageInventoryClosed OnStorageInventoryClosedEvent;
};
