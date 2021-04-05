// Created 1/17/21 01:13 AM

#pragma once

#include "LocalPlayerSubsystem.h"
#include "StatEnums.h"
#include "UIDelegateContext.generated.h"

class AUnit;
class ABaseHero;
class UBackpack;
class UUnitSelectionSlot;
class UActionbarInterface;
class USettingsMenu;
class UActionbar_EnemyInterface;

// Hero whose stat got upgraded, Attribute that got upgraded, and was T/F = upgraded/downgraded
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAttributePointAllocated, ABaseHero*, EAttributes, bool);

DECLARE_EVENT_OneParam(UUnitSelectionSlot, FOnUnitSlotSelected, AUnit*); // Pass in unit corresponding to unit lost

DECLARE_EVENT(UActionbarInterface, FOnSelectionLockToggled);

DECLARE_EVENT_OneParam(UActionbar_EnemyInterface, FOnEnemySkillSlotClicked, int);

DECLARE_EVENT(USettingsMenu, FOnQuickCastSettingToggled);
DECLARE_EVENT(USettingsMenu, FOnStaticFormationSettingToggled);
DECLARE_EVENT(USettingsMenu, FOnAutoClickSettingToggled);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillSlotDropped, int, dragSlotIndex, int, dropSlotIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillSlotDroppedSB, int, dragSlotIndex, int, dropSlotIndex); // Used when we drop from the spell book
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnItemSlotDroppedFromInventory, int, dragSlotIndex, int, dropSlotIndex, UBackpack*, dragPack, UBackpack*, dropPack);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnItemSlotDroppedFromStorage, int, dragSlotIndex, int, dropSlotIndex, UBackpack*, dragPack, UBackpack*, dropPack);


UCLASS()
class MYPROJECT_API UUIDelegateContext : public ULocalPlayerSubsystem
{
   GENERATED_BODY()
   
public:
   FOnAttributePointAllocated& OnAttributePointAllocated() const { return OnAttributePointAllocatedEvent; }

   FOnUnitSlotSelected& OnUnitSlotSelected() const { return OnUnitSlotSelectedEvent; }

   FOnSelectionLockToggled& OnSelectionLockToggled() const { return OnSelectionLockToggledEvent; }

   FOnQuickCastSettingToggled& OnQuickCastSettingToggled() const { return OnQuickCastSettingToggledEvent; }

   FOnStaticFormationSettingToggled& OnStaticFormationToggled() const { return OnStaticFormationToggledEvent; }

   FOnAutoClickSettingToggled& OnAutoclickToggled() const { return OnAutoclickToggledEvent; }

   FOnEnemySkillSlotClicked& OnEnemySkillSlotClicked() const { return OnEnemySkillSlotClickedEvent; }
   
   UPROPERTY(BlueprintAssignable, BlueprintCallable)
   FOnSkillSlotDropped OnSkillSlotDroppedEvent;

   UPROPERTY(BlueprintAssignable, BlueprintCallable)
   FOnSkillSlotDroppedSB OnSkillSlotDroppedSBEvent;

   UPROPERTY(BlueprintAssignable, BlueprintCallable)
   FOnItemSlotDroppedFromInventory OnItemSlotDroppedInventoryEvent;

   UPROPERTY(BlueprintAssignable, BlueprintCallable)
   FOnItemSlotDroppedFromStorage OnItemSlotDroppedStorageEvent;

private:
   mutable FOnUnitSlotSelected OnUnitSlotSelectedEvent;
   mutable FOnSelectionLockToggled OnSelectionLockToggledEvent;
   mutable FOnAttributePointAllocated OnAttributePointAllocatedEvent;
   mutable FOnQuickCastSettingToggled OnQuickCastSettingToggledEvent;
   mutable FOnStaticFormationSettingToggled OnStaticFormationToggledEvent;
   mutable FOnAutoClickSettingToggled OnAutoclickToggledEvent;
   mutable FOnEnemySkillSlotClicked OnEnemySkillSlotClickedEvent;
};
