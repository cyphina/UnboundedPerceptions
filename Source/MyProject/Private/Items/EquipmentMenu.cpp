// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "EquipmentMenu.h"
#include "Items/ItemDelegateStore.h"
#include "UI/HUDManager.h"

void UEquipmentMenu::NativeOnInitialized()
{
   ItemChangeEvents::OnEquipmentChangedEvent.AddUObject(this, &UEquipmentMenu::OnEquipmentChanged);
}

void UEquipmentMenu::Update_Implementation()
{
}

void UEquipmentMenu::OnEquipmentChanged(const ABaseHero* heroThatChanged, const FMyItem& changedEquip)
{
   if(GetVisibility() != ESlateVisibility::Collapsed) {
      if(GetEquippedHero() == heroThatChanged) Update();
   }
}
