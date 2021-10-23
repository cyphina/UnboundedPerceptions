// Created 5/13/21 5:42 AM

#pragma once
#include "PanelSlot.h"
#include "PannableContentPanelSlot.generated.h"

class SPannableContentPanel;
class UPannableContentPanel;

UCLASS()
class MYPROJECT_API UPannableContentPanelSlot : public UPanelSlot
{
   GENERATED_BODY()

 public:
   UFUNCTION(BlueprintCallable, Category = "Appearance")
   void SetPadding(FMargin InPadding);

   void         BuildSlot(TSharedRef<SPannableContentPanel> InContentPanel);
   virtual void SynchronizeProperties() override;
   virtual void ReleaseSlateResources(bool bReleaseChildren) override;

 protected:
   /** The padding area between the slot and the content it contains. */
   UPROPERTY(EditAnywhere, Category = "Layout|Border Slot")
   FMargin Padding;

 private:
   TWeakPtr<SPannableContentPanel> PannableContentPanel;
};
