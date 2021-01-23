// Created 1/15/21 09:08 PM

#pragma once
#include "UserWidget.h"
#include "UnitSelectionSlot.generated.h"

class UButton;
class UImage;
class AUnit;

UCLASS()
class MYPROJECT_API UUnitSelectionSlot : public UUserWidget
{
   GENERATED_BODY()

public:
   void SetUnitInformation(AUnit* unit);

protected:
   void NativeOnInitialized() override;
   
   UPROPERTY(Meta=(BindWidget))
   UButton* Btn_SelectUnit;

   UPROPERTY(Meta=(BindWidget))
   UImage* Img_UnitIcon;

private:
   UFUNCTION()
   void OnBtnSelectUnitClicked();
   
   AUnit* unitRef;
};
