// Created 1/15/21 8:57 PM

#pragma once
#include "UserWidget.h"
#include "Actionbar_MultiUnitPortrait.generated.h"

class UUnitSelectionSlot;
UCLASS()
class MYPROJECT_API UActionbar_MultiUnitPortrait : public UUserWidget
{
   GENERATED_BODY()

public:
   void RefreshDisplayedUnitImages();
   
private:
   TStaticArray<UUnitSelectionSlot*, 16> unitSlots;
};
