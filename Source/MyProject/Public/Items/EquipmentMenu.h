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
struct FMyItem;

UCLASS()
class MYPROJECT_API UEquipmentMenu : public UMyDraggableWidget
{
   GENERATED_BODY()

 public:
   UFUNCTION(BlueprintCallable)
   ABaseHero* GetEquippedHero() const { return hero; }

   void NativeOnInitialized() override;

   /** Call to redraw what is in equipment*/
   UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Equipment")
   void Update();
   void Update_Implementation();

 protected:
   UPROPERTY(BlueprintReadWrite, Category = "References")
   ABaseHero* hero;

 private:
   void OnEquipmentChanged((const ABaseHero* heroThatChanged, const FMyItem& changedEquip);
};
