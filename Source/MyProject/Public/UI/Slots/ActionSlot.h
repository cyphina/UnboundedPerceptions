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

   static TSubclassOf<UToolTipWidget> toolTipWidgetClass;

 protected:
   UPROPERTY(BlueprintReadWrite, Category = "Action", Meta = (BindWidget))
   UButton* btnAction;

   UPROPERTY(BlueprintReadWrite, Category = "Action", Meta = (BindWidget))
   UImage* actionImage;

   UPROPERTY(BlueprintReadWrite, Category = "Action", Meta = (BindWidget))
   UTextBlock* infoText;

   UPROPERTY(BlueprintReadOnly)
   AUserInput* CPCRef;

   UFUNCTION() void NativeConstruct() override;

   UFUNCTION()
   virtual void OnBtnClick() PURE_VIRTUAL(UActionSlot::OnBtnClick, );

   /** Sets up the text to be displayed for a tooltip*/
   UFUNCTION()
   void OnBtnHover();

   /** Setup information on the tooltip widget*/
   UFUNCTION()
   virtual void ShowDesc(UToolTipWidget* tooltip) PURE_VIRTUAL(UActionSlot::ShowDesc, );

 public:
   UActionSlot(const FObjectInitializer& o);

   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action", Meta = (ExposeOnSpawn = true))
   int slotIndex;

   UFUNCTION(BlueprintCallable, Category = "Action")
   virtual void SetImage(UTexture2D* image);

   UFUNCTION(BlueprintCallable, Category = "Action")
   void SetInfo(FText newInfo);

   UFUNCTION(BlueprintCallable, Category = "Action")
   void SetImageFromMaterial(UMaterialInstanceDynamic* image);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Action")
   UTexture2D* GetImage() const;
};
