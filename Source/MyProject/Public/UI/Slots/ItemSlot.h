#pragma once

#include "CoreMinimal.h"
#include "UI/Slots/ActionSlot.h"
#include "ItemSlot.generated.h"

/**
 * Item slot for inventory (can also be used in storage inventory as well)
 */
UCLASS()
class MYPROJECT_API UItemSlot : public UActionSlot
{
   GENERATED_BODY()

protected:
   void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
   bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
   void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
   class UInventory* inventoryRef;

   void ShowDesc(UToolTipWidget* tooltip) override;
};
