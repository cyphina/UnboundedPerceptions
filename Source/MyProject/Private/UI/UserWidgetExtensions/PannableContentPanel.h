#pragma once
#include "Components/ContentWidget.h"
#include "PannableContentPanel.generated.h"

class SPannableContentPanel;

/**
 * @brief A panel that holds a widget that is larger than the space specifiied by this panel.
 * We can then drag around the area defined by this widget to translate the child.
 */
UCLASS()
class MYPROJECT_API UPannableContentPanel : public UContentWidget
{
   GENERATED_BODY()

 public:
   UPannableContentPanel();

   UFUNCTION(BlueprintCallable, Category = "Appearance")
   void SetPadding(FMargin InPadding);

   /** The padding area between the slot and the content it contains. */
   UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Content")
   FMargin Padding;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
   float ScrollSpeed = 0.2f;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
   float PanSpeed = 1.f;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
   float MaxPanLeftRight = 1000.f;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
   float MaxPanTopBottom = 1000.f;

 protected:
   virtual UClass* GetSlotClass() const override;
   virtual void    OnSlotAdded(UPanelSlot* InSlot) override;
   virtual void    OnSlotRemoved(UPanelSlot* InSlot) override;

   virtual TSharedRef<SWidget> RebuildWidget() override;
   virtual void                ReleaseSlateResources(bool bReleaseChildren) override;

   TSharedPtr<SPannableContentPanel> MyContentPanel;

 private:
   virtual const FText GetPaletteCategory() override;
};
