#pragma once

#include "CoreMinimal.h"
#include "UI/Slots/ActionSlot.h"
#include "ShopSlot.generated.h"

struct FItemPrice;

/**
* Slot we press to buy and sell items!
*/
UCLASS()
class MYPROJECT_API UShopSlot : public UActionSlot
{
   GENERATED_BODY()
   
   void ShowDesc(UToolTipWidget* tooltip) override;

   FText MakeItemPriceText(const FItemPrice& price) const;
};
