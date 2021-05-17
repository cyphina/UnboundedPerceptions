// Created 5/12/21 8:42 PM

#include "MyProject.h"
#include "PannableContentPanel.h"

#include "PannableContentPanelSlot.h"
#include "SPannableContentPanel.h"

void UPannableContentPanel::ReleaseSlateResources(bool bReleaseChildren)
{
   Super::ReleaseSlateResources(bReleaseChildren);
   MyContentPanel.Reset();
}

UPannableContentPanel::UPannableContentPanel()
{
   SetClipping(EWidgetClipping::ClipToBounds);
}

void UPannableContentPanel::SetPadding(FMargin InPadding)
{
   Padding = InPadding;
   if(MyContentPanel.IsValid())
   {
      MyContentPanel->SetPadding(InPadding);
   }
}

UClass* UPannableContentPanel::GetSlotClass() const
{
   return UPannableContentPanelSlot::StaticClass();
}

void UPannableContentPanel::OnSlotAdded(UPanelSlot* InSlot)
{
   UPannableContentPanelSlot* ContentPanelSlot = CastChecked<UPannableContentPanelSlot>(InSlot);

   if(MyContentPanel.IsValid())
   {
      ContentPanelSlot->BuildSlot(MyContentPanel.ToSharedRef());
   }
}

void UPannableContentPanel::OnSlotRemoved(UPanelSlot* InSlot)
{
   if(MyContentPanel.IsValid())
   {
      MyContentPanel->SetContent(SNullWidget::NullWidget);
   }
}

TSharedRef<SWidget> UPannableContentPanel::RebuildWidget()
{
   MyContentPanel = SNew(SPannableContentPanel).ScrollZoomMultiplier(ScrollSpeed).PanSpeed(PanSpeed).MaxPanX(MaxPanLeftRight).MaxPanY(MaxPanTopBottom);

   if(GetChildrenCount())
   {
      Cast<UPannableContentPanelSlot>(GetContentSlot())->BuildSlot(MyContentPanel.ToSharedRef());
   }

   return MyContentPanel.ToSharedRef();
}

const FText UPannableContentPanel::GetPaletteCategory()
{
   return NSLOCTEXT("UMG", "Panel", "Panel");
}
