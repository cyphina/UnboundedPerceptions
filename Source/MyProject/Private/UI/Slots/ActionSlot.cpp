#include "MyProject.h"
#include "ActionSlot.h"

#include "SlotContainer.h"
#include "TextBlock.h"
#include "UserInput.h"
#include "UWidgetHelperLibrary.h"
#include "WidgetBlueprintLibrary.h"
#include "UI/HUDManager.h"

#include "UI/UserWidgets/ToolTipWidget.h"
#include "UMG/Public/Components/Image.h"

UActionSlot::UActionSlot(const FObjectInitializer& o) : UUserWidget(o)
{
}

void UActionSlot::SetSlotImage(UTexture2D* image)
{
   actionImage->SetBrushFromTexture(image, false);
}

void UActionSlot::SetInfo(FText newInfo)
{
   infoText->SetText(newInfo);
}

void UActionSlot::SetImageFromMaterial(UMaterialInstanceDynamic* image)
{
   actionImage->SetBrushFromMaterial(image);
}

UTexture2D* UActionSlot::GetImage() const
{
   UTexture* slotTex = Cast<UTexture>(actionImage->Brush.GetResourceObject());
   if(!slotTex)
   {
      UMaterialInterface* slotMaterial = Cast<UMaterialInterface>(actionImage->Brush.GetResourceObject());
      slotMaterial->GetTextureParameterValue(TEXT("RadialTexture"), slotTex);
   }

   return Cast<UTexture2D>(slotTex);
}

void UActionSlot::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   CPCRef = Cast<AUserInput>(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController());
}

void UActionSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
   UToolTipWidget* ttWidget = CreateWidget<UToolTipWidget>(CPCRef, CPCRef->GetHUDManager()->toolTipWidgetClass);

   if(ttWidget)
   {
      ShowDesc(ttWidget);
      SetToolTip(ttWidget);
   }
}

FReply UActionSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
   if(InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
   {
      if(USlotContainer* slotContainer = UUWidgetHelperLibrary::GetUserWidgetParent<USlotContainer>(this))
      {
         slotContainer->SetSelectedSlotIndex(slotIndex);
      }
      return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
   }
   return FReply::Unhandled();
}

void UActionSlot::OnHover()
{
}
