// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DraggedActionWidget.h"
#include "Blueprint/UserWidget.h"
#include "ActionSlot.generated.h"

/**
 * Slot a use can press to trigger some kind of effect
 */

class AUserInput;
class UButton;
class UTextBlock;
class UImage;
class UToolTipWidget;

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
   void SetInfo(FText newInfo);

   UFUNCTION(BlueprintCallable, Category = "Action")
   void SetImageFromMaterial(UMaterialInstanceDynamic* image);

   UFUNCTION(BlueprintCallable, Category = "Action")
   int GetSlotIndex() const { return slotIndex; }

 protected:
   /** Sets up the text to be displayed for a tooltip*/
   UFUNCTION()
   void OnHover();

   /** Setup information on the tooltip widget*/
   UFUNCTION()
   virtual void ShowDesc(UToolTipWidget* tooltip) PURE_VIRTUAL(UActionSlot::ShowDesc, );

   FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

   void NativeOnInitialized() override;
   void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

   // An image covering the slot (used for items, spells, etc.)
   UPROPERTY(Meta = (BindWidget))
   UImage* actionImage;

   // Text that shows up in corner of the slot
   UPROPERTY(Meta = (BindWidget))
   UTextBlock* infoText;

   UPROPERTY(BlueprintReadOnly)
   AUserInput* CPCRef;
	
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
