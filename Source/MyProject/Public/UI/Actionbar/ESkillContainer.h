#pragma once

#include "SlotContainer.h"
#include "ESkillContainer.generated.h"

class USkillSlot;
class UMySpell;
class AAlly;
class AUnit;
class URTSAbilitySystemComponent;

/**
 * @brief Widget to display a skill ring when a unit is selected. Players can select skills from the ring ot cast, and swap positions on the ring
 * to control what skill binds to what hotkey.
 * Component currently resides inside the "Single Unit Views".
 */
UCLASS()
class MYPROJECT_API UESkillContainer : public USlotContainer
{
   GENERATED_BODY()

public:
   void OnWidgetShown(URTSAbilitySystemComponent* focusedUnitAbilityComponent);

   int GetNumSlots() const override { return skillSlots.Num(); }

protected:
   void NativePreConstruct() override;
   void NativeOnInitialized() override;

   UPROPERTY(Meta = (BindWidget))
   USkillSlot* skillSlot1;

   UPROPERTY(Meta = (BindWidget))
   USkillSlot* skillSlot2;

   UPROPERTY(Meta = (BindWidget))
   USkillSlot* skillSlot3;

   UPROPERTY(Meta = (BindWidget))
   USkillSlot* skillSlot4;

   UPROPERTY(Meta = (BindWidget))
   USkillSlot* skillSlot5;

   UPROPERTY(Meta = (BindWidget))
   USkillSlot* skillSlot6;

   UPROPERTY(Transient, Meta = (BindWidgetAnim))
   UWidgetAnimation* flowOutAnim;

   UPROPERTY(Transient, Meta = (BindWidgetAnim))
   UWidgetAnimation* flowInAnim;

private:
   UFUNCTION()
   void OnKeybindsChanged(FInputActionKeyMapping newKeyMap);

   UFUNCTION()
   void OnSkillSlotDropped(int dragSlotIndex, int dropSlotIndex);

   void OnFocusedUnitSpellCasted(AUnit* focusedUnit, int spellIndex);
   void OnSpellSlotReplaced(AUnit* affectedUnit, TSubclassOf<UMySpell> replacingSpellClass, int slotIndex);

   URTSAbilitySystemComponent* focusedUnitAbilityComp;
   TArray<USkillSlot*>         skillSlots;
};
