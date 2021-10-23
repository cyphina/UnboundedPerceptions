#pragma once

#include "CoreMinimal.h"
#include "UI/Slots/ActionSlot.h"
#include "EquipmentSlot.generated.h"

/**
 * Slots in the equipment menu
 */
UCLASS()
class MYPROJECT_API UEquipmentSlot : public UActionSlot
{
   GENERATED_BODY()

   void NativeConstruct() override;
   void ShowDesc(UToolTipWidget* tooltip) override;
};
