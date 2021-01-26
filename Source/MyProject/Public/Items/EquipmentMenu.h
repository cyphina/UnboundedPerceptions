#pragma once

#include "EquipmentSlot.h"
#include "UI/UserWidgetExtensions/MyDraggableWidget.h"
#include "EquipmentMenu.generated.h"

/**
 * Menu for equipment
 */

struct FBackpackUpdateResult;
class UEquip;
class UWeapon;
class UTextBlock;
class ABaseHero;
struct FMyItem;

UCLASS()
class MYPROJECT_API UEquipmentMenu : public UMyDraggableWidget
{
   GENERATED_BODY()

 public:
   UFUNCTION(BlueprintCallable)
   ABaseHero* GetEquippedHero() const { return heroRef; }

 protected:
   void NativeOnInitialized() override;
   bool OnWidgetAddToViewport_Implementation() override;
   
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

   UPROPERTY(meta=(BindWidget))
   UTextBlock* Text_MenuTitle;
   
 private:
   void SetupEquipImages();
   void OnEquipmentChanged(const ABaseHero* heroThatChanged, const FBackpackUpdateResult& equipAddedToInventory);

   TStaticArray<UEquipmentSlot*, 8> equipSlots;
};
