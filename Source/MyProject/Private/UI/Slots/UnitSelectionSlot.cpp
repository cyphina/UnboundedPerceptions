// Created 1/15/21 09:08 PM

#include "UnitSelectionSlot.h"
#include "Button.h"
#include "Image.h"
#include "UIDelegateContext.h"
#include "Unit.h"

void UUnitSelectionSlot::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   Btn_SelectUnit->OnClicked.AddDynamic(this, &UUnitSelectionSlot::OnBtnSelectUnitClicked);
}

void UUnitSelectionSlot::SetUnitInformation(AUnit* unit)
{
   unitRef = unit;
   Img_UnitIcon->SetBrushFromTexture(unit->GetImage());
}

void UUnitSelectionSlot::OnBtnSelectUnitClicked()
{
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UUIDelegateContext>()->OnUnitSlotSelected().Broadcast(unitRef);
}
