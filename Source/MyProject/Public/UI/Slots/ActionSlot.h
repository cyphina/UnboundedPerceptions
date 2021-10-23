#pragma once

#include "DraggedActionWidget.h"
#include "Blueprint/UserWidget.h"
#include "ActionSlot.generated.h"

class USlotContainer;
class UBorder;
class AUserInput;
class UButton;
class UTextBlock;
class UImage;
class UToolTipWidget;
class UActionSlotStyle;

/**
 * Deriving from this allows us leverage several features (you don't have to use all of them). Don't think of inheritance as having to be perfect, think of mutated babies huh?\n
 * Provides an interface to allow parents to send message to the gameplay system if a slot is clicked (so we don't have to handle gameplay logic in our UI)\n
 * -- When doing this, handle the logic within the slot container class (or whatever class you use to hold the slots) \n
 * -- If not using a slot container parent, you'll have to override the mouse click functionality \n
 * Has support for drag and drop\n
 * Has styling options\n
 * Has tooltip support\n
 */

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

   /**
    * @brief Normally we don't have to change the index of any slots but if we have several slots that are dynamically generated then we need to change them with this function instead of in the designer
    * @param NewSlotIndex
    */
   UFUNCTION(BlueprintCallable, Category = "Action")
   void SetSlotIndex(int NewSlotIndex) { slotIndex = NewSlotIndex; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Action")
   int GetSlotIndex() const { return slotIndex; }

   void SetIsEnabled(bool bInIsEnabled) override;

   void RemakeDescription();

 protected:
   /** Setup information on the tooltip widget before it is displayed*/
   UFUNCTION()
   virtual void ShowDesc(UToolTipWidget* tooltip) PURE_VIRTUAL(UActionSlot::ShowDesc, );

   FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
   FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
   FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

   void NativePreConstruct() override;
   void NativeOnInitialized() override;
   void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
   void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

   /**
    * @brief Only works if we're using a SlotContainer to hold the action slot. If we're aggregating these slots in some other kind of class, will return nullptr.
    * @return Returns SlotContainer holder this action slot.
    */
   USlotContainer* GetParentContainer() const;

   UActionSlotStyle* GetStyleCDO() const;

   UDraggedActionWidget* CreateDragIndicator();

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
   // This has to be set to enable drag and drop.
   UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
   TSubclassOf<UDraggedActionWidget> draggedActionWidgetClass;
};
