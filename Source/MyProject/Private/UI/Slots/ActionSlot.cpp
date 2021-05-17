#include "MyProject.h"
#include "ActionSlot.h"

#include "ActionSlotStyle.h"
#include "Border.h"
#include "SlotContainer.h"
#include "TextBlock.h"
#include "UserInput.h"
#include "UpWidgetHelperLibrary.h"
#include "WidgetBlueprintLibrary.h"
#include "UI/HUDManager.h"

#include "UI/UserWidgets/ToolTipWidget.h"
#include "UMG/Public/Components/Image.h"

UActionSlot::UActionSlot(const FObjectInitializer& o) : UUserWidget(o)
{
}

void UActionSlot::SetSlotImage(UTexture2D* image)
{
   if(image)
   {
      actionImage->SetBrushFromTexture(image, false);
   }
   else
   {
      if(GetStyleCDO())
      {
         actionImage->SetBrushFromTexture(GetStyleCDO()->GetDefaultSlotImage(), false);
      }
   }
}

void UActionSlot::SetSlotStyle(TSubclassOf<UActionSlotStyle> newStyle)
{
   if(newStyle)
   {
      style = newStyle;
      actionImage->SetBrushFromTexture(GetStyleCDO()->GetDefaultSlotImage(), false);
      actionBorder->SetBrush(GetStyleCDO()->GetDefaultBrush());
   }
}

void UActionSlot::NativePreConstruct()
{
   Super::NativePreConstruct();
   SetSlotStyle(style);
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
}

void UActionSlot::SetIsEnabled(bool bInIsEnabled)
{
   Super::SetIsEnabled(bInIsEnabled);
   if(bIsEnabled)
   {
      if(GetStyleCDO())
      {
         actionBorder->SetBrush(GetStyleCDO()->GetDisabledBrush());
      }
   }
   else
   {
      actionBorder->SetBrush(GetStyleCDO()->GetDefaultBrush());
   }
}

UActionSlotStyle* UActionSlot::GetStyleCDO() const
{
   return style ? style.GetDefaultObject() : nullptr;
}

UDraggedActionWidget* UActionSlot::CreateDragIndicator()
{
   if(draggedActionWidgetClass)
   {
      return CreateWidget<UDraggedActionWidget>(this, draggedActionWidgetClass);
   }
   return nullptr;
}

FReply UActionSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
   if(GetStyleCDO())
   {
      actionBorder->SetBrush(GetStyleCDO()->GetPressedBrush());
   }

   if(InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
   {
      if(draggedActionWidgetClass)
      {
         return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
      }
   }

   return FReply::Handled();
}

FReply UActionSlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
   if(USlotContainer* slotContainer = GetParentContainer())
   {
      slotContainer->SetSelectedSlotIndex(slotIndex);
   }

   if(GetStyleCDO())
   {
      actionBorder->SetBrush(GetStyleCDO()->GetDefaultBrush());
   }
   return FReply::Unhandled();
}

FReply UActionSlot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
   return FReply::Handled();
}

void UActionSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
   if(AUserInput* CPCRef = Cast<AUserInput>(GetOwningPlayer<AUserInput>()))
   {
      UToolTipWidget* ToolTipWidget = CreateWidget<UToolTipWidget>(CPCRef, CPCRef->GetHUDManager()->toolTipWidgetClass);

      if(ToolTipWidget)
      {
         ShowDesc(ToolTipWidget);
         SetToolTip(ToolTipWidget);
      }
   }
}

void UActionSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
   if(GetStyleCDO())
   {
      actionBorder->SetBrush(GetStyleCDO()->GetDefaultBrush());
   }

   SetToolTip(nullptr);
}

USlotContainer* UActionSlot::GetParentContainer() const
{
   return UUpWidgetHelperLibrary::GetUserWidgetParent<USlotContainer>(this);
}
