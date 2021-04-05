#pragma once

#include "EquipmentSlot.h"
#include "SlotContainer.h"
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
class MYPROJECT_API UEquipmentMenu : public USlotContainer
{
   GENERATED_BODY()

<<<<<<< HEAD
 public:
=======
public:
>>>>>>> componentrefactor
   UFUNCTION(BlueprintCallable)
   ABaseHero* GetEquippedHero() const { return heroRef; }

<<<<<<< HEAD
   /** Call to redraw what is in equipment */
   UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Equipment")
   void Update();

   void NativeOnInitialized() override;

   void Update_Implementation();

 protected:
   UPROPERTY(BlueprintReadWrite, Category = "References")
   ABaseHero* hero;

 private:
   void OnEquipmentChanged(const ABaseHero* heroThatChanged, const FMyItem& changedEquip);
=======
   int GetNumValidItems() const override { return equipSlots.Num(); }

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
   void OnEquipmentChanged(const ABaseHero* heroThatChanged, TArray<int> updatedInventorySlots);

   TStaticArray<UEquipmentSlot*, 8> equipSlots;
>>>>>>> componentrefactor
};
