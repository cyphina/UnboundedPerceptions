#pragma once

#include "CoreMinimal.h"
#include "UI/Slots/ActionSlot.h"
#include "ShopSlot.generated.h"

/**
 * Slot we press to buy and sell items!
 */

struct FItemPrice;

UCLASS()
class MYPROJECT_API UShopSlot : public UActionSlot
{
   GENERATED_BODY()

   void OnBtnClick() override;
   void ShowDesc(UToolTipWidget* tooltip) override;

   FText MakeItemPriceText(FItemPrice& price) const;
};
