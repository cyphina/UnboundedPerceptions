// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "ItemExamineWidget.generated.h"

class UButton;

/**
 * UI that opens when we want to examine an item closely. One day I'll add a 3D model we can examine in addition to just textures like Ace Attorney
 */
UCLASS()
class MYPROJECT_API UItemExamineWidget : public UMyUserWidget
{
   GENERATED_BODY()

 public:
   void NativeConstruct() override;

   bool OnWidgetAddToViewport_Implementation() override;

   /**
    * @brief ID of the item who's picture should be examined when the item is used. Only set in the Examine spell's BP.
    */
   UPROPERTY(BlueprintReadWrite)
   int itemToDisplayID = -1;

   /**Maps item id which is specified when this hud is opened to a corresponding picture*/
   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
   TMap<int, TSoftObjectPtr<UTexture>> itemIDToDetailedPicture;

 protected:
   UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
   UButton* btnClose;

 private:
   UFUNCTION()
   void CloseMenu();

   UPROPERTY()
   class AHUDManager* hudManagerRef;
};
