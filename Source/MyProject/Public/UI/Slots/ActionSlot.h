#pragma once

#include "CoreMinimal.h"

#include "DraggedActionWidget.h"
#include "Blueprint/UserWidget.h"
#include "ActionSlot.generated.h"

/**
 * Slot a use can press to trigger some kind of effect
 */

class USlotContainer;
class UBorder;
class AUserInput;
class UButton;
class UTextBlock;
class UImage;
class UToolTipWidget;
class UActionSlotStyle;

UCLASS(Abstract)
class MYPROJECT_API UActionSlot : public UUserWidget
{
   GENERATED_BODY()

 public:
   UActionSlot(const FObjectInitializer& o);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Action")
   UTexture2D* GetImage() const;

   UFUNCTION(BlueprintCallable, Category = "Action")
   virtual void SetSlotImage(UTexture2D* image);

   UFUNCTION(BlueprintCallable, Category = "Action")
   void SetSlotStyle(TSubclassOf<UActionSlotStyle> newStyle);

   UFUNCTION(BlueprintCallable, Category = "Action")
   void SetInfo(FText newInfo);

   UFUNCTION(BlueprintCallable, Category = "Action")
   void SetImageFromMaterial(UMaterialInstanceDynamic* image);

   UFUNCTION(BlueprintCallable, Category = "Action")
   int GetSlotIndex() const { return slotIndex; }

   void SetIsEnabled(bool bInIsEnabled) override;

 protected:
   /** Setup information on the tooltip widget*/
   UFUNCTION()
   virtual void ShowDesc(UToolTipWidget* tooltip) PURE_VIRTUAL(UActionSlot::ShowDesc, );

   FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
   FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

   void NativePreConstruct() override;
   void NativeOnInitialized() override;
   void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
   void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

   USlotContainer* GetParentContainer() const;
	
   UActionSlotStyle* GetStyleCDO() const;

   UPROPERTY(Meta = (BindWidget))
   UBorder* actionBorder;

   // An image covering the slot (used for items, spells, etc.)
   UPROPERTY(Meta = (BindWidget))
   UImage* actionImage;

   // Text that shows up in corner of the slot
   UPROPERTY(Meta = (BindWidget))
   UTextBlock* infoText;

   UPROPERTY(EditANywhere, BlueprintReadOnly)
   TSubclassOf<UActionSlotStyle> style;

   /**
    * Can only be edited in the designer or in the "CreateWidget" node since it shouldn't be modified anywhere else but there.
    * When using loops to create many slots use the setter in the "CreateWidget" node and when there's a few set number of slots
    * (e.g. the ActionBar skill ring) then set the slot index manually in the designer.
    */
   UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Action", Meta = (ExposeOnSpawn = true))
   int slotIndex;

   // TODO: Maybe make this a simple slate widget?
   UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
   TSubclassOf<UDraggedActionWidget> draggedActionWidgetClass;
};
