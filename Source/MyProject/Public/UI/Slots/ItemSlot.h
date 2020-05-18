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

   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
   class UInventory* inventoryRef;

   void OnBtnClick() override;
   void ShowDesc(UToolTipWidget* tooltip) override;
};
