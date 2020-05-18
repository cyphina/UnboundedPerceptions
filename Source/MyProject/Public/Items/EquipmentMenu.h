#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyDraggableWidget.h"
#include "EquipmentMenu.generated.h"

/**
 * Menu for equipment
 */

class UEquip;
class UWeapon;
class ABaseHero;

UCLASS()
class MYPROJECT_API UEquipmentMenu : public UMyDraggableWidget
{
   GENERATED_BODY()

 protected:
   UPROPERTY(BlueprintReadWrite, Category = "References")
   ABaseHero* hero;

 public:
   UFUNCTION(BlueprintCallable)
   ABaseHero* GetEquippedHero() const { return hero; }

   bool OnWidgetAddToViewport_Implementation() override;

   /**Call to redraw what is in equipment*/
   UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Equipment")
   void Update();
   void Update_Implementation();
};
