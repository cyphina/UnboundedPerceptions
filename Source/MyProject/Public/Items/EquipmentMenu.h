#pragma once

#include "CoreMinimal.h"

#include "EquipmentSlot.h"
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
   ABaseHero* GetEquippedHero() const { return heroRef; }

   /** Call to redraw what is in equipment */
   UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Equipment")
   void Update();

   void NativeOnInitialized() override;

   void Update_Implementation();

 protected:
   UPROPERTY(BlueprintReadWrite, Category = "References")
   ABaseHero* heroRef;

   UPROPERTY(meta=(BindWidget))
   UEquipmentSlot* helmetSlot;

   UPROPERTY(meta=(BindWidget))
   UEquipmentSlot* bodySlot;

   UPROPERTY(meta=(BindWidget))
   UEquipmentSlot* gloveSlot;

   UPROPERTY(meta=(BindWidget))
   UEquipmentSlot* footSlot;

   UPROPERTY(meta=(BindWidget))
   UEquipmentSlot* accessorySlot;

   UPROPERTY(meta=(BindWidget))
   UEquipmentSlot* primaryWeaponSlot;

   UPROPERTY(meta=(BindWidget))
   UEquipmentSlot* offHandSlot;
   
 private:
   void SetupEquipImages();
   void OnEquipmentChanged(const ABaseHero* heroThatChanged, const FMyItem& changedEquip);

   TStaticArray<UEquipmentSlot*, 8> equipSlots;
};
