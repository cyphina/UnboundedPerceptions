// Created 5/13/21 5:42 AM

#include "MyProject.h"
#include "PannableContentPanelSlot.h"
#include "PannableContentPanel.h"
#include "SPannableContentPanel.h"
#include "Widget.h"

void UPannableContentPanelSlot::ReleaseSlateResources(bool bReleaseChildren)
{
   Super::ReleaseSlateResources(bReleaseChildren);
   PannableContentPanel.Reset();
}

void UPannableContentPanelSlot::SetPadding(FMargin InPadding)
{
   CastChecked<UPannableContentPanel>(Parent)->SetPadding(InPadding);
}

void UPannableContentPanelSlot::BuildSlot(TSharedRef<SPannableContentPanel> InContentPanel)
{
   PannableContentPanel = InContentPanel;
   PannableContentPanel.Pin()->SetPadding(Padding);
   PannableContentPanel.Pin()->SetContent(Content ? Content->TakeWidget() : SNullWidget::NullWidget);
}

void UPannableContentPanelSlot::SynchronizeProperties()
{
   if(PannableContentPanel.IsValid()) {
      SetPadding(Padding);
   }
}
