// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

 protected:
   UFUNCTION()
   virtual void OnBtnClick() PURE_VIRTUAL(UActionSlot::OnBtnClick, );

   /** Sets up the text to be displayed for a tooltip*/
   UFUNCTION()
   void OnBtnHover();

   /** Setup information on the tooltip widget*/
   UFUNCTION()
   virtual void ShowDesc(UToolTipWidget* tooltip) PURE_VIRTUAL(UActionSlot::ShowDesc, );

   void NativeOnInitialized() override;

   // A button we can press to trigger the slot
   UPROPERTY(Meta = (BindWidget))
   UButton* btnAction;

   // An image covering the slot (used for items, spells, etc.)
   UPROPERTY(Meta = (BindWidget))
   UImage* actionImage;

   // Text that shows up in corner of the slot
   UPROPERTY(Meta = (BindWidget))
   UTextBlock* infoText;

   UPROPERTY(BlueprintReadOnly)
   AUserInput* CPCRef;

   UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Action", Meta = (ExposeOnSpawn = true))
   int slotIndex;

 private:
   static TSubclassOf<UToolTipWidget> toolTipWidgetClass;
};
