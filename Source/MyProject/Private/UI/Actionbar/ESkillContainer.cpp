#include "MyProject.h"
#include "ESkillContainer.h"

#include "BasePlayer.h"
#include "ManualSpellComponent.h"
#include "RTSCheatManager.h"
#include "SettingsContext.h"
#include "SpellDelegateStore.h"
#include "UserInput.h"
#include "Slots/ActionSlot.h"
#include "WorldObjects/Ally.h"
#include "Slots/SkillSlot.h"

void UESkillContainer::NativePreConstruct()
{
   Super::NativeConstruct();
}

void UESkillContainer::NativeOnInitialized()
{
   SpellHUDEvents::OnFocusedUnitSpellCastedEvent.AddUObject(this, &UESkillContainer::OnFocusedUnitSpellCasted);
   SpellHUDEvents::OnSpellSlotReplacedEvent.AddUObject(this, &UESkillContainer::OnSpellSlotReplaced);
   GetWorld()->GetGameInstance()->GetSubsystem<USettingsContext>()->OnKeybindsChanged().AddDynamic(this, &UESkillContainer::OnKeybindsChanged);
   
   skillSlots.Add(skillSlot1);
   skillSlots.Add(skillSlot2);
   skillSlots.Add(skillSlot3);
   skillSlots.Add(skillSlot4);
   skillSlots.Add(skillSlot5);
   skillSlots.Add(skillSlot6);
}

void UESkillContainer::OnWidgetShown(URTSAbilitySystemComponent* focusedUnitAbilityComp)
{
   int index = 0;
   for(USkillSlot* skillSlot : skillSlots)
   {
      skillSlot->UpdateSkillSlot(focusedUnitAbilityComp->GetSpellAtSlot(index));
      ++index;
   }
   PlayAnimation(flowOutAnim);
}

void UESkillContainer::OnFocusedUnitSpellCasted(AUnit* focusedUnit, int spellIndex)
{
   if(0 <= spellIndex && spellIndex < skillSlots.Num())
   {
      skillSlots[spellIndex]->ShowCooldown();
   }
}

void UESkillContainer::OnSpellSlotReplaced(AUnit* affectedUnit, TSubclassOf<UMySpell> replacingSpellClass, int slotIndex)
{
   AUnit* focusedUnit = GetOwningPlayer<AUserInput>()->GetBasePlayer()->GetFocusedUnit();
   if(focusedUnit == affectedUnit)
   {
      skillSlots[slotIndex]->UpdateSkillSlot(replacingSpellClass);
   }
}

void UESkillContainer::OnKeybindsChanged(FInputActionKeyMapping newKeyMap)
{
   TArray<FName>         abilityActionNames;
   const UInputSettings* inputSettings = UInputSettings::GetInputSettings();

   inputSettings->GetActionNames(abilityActionNames);
   abilityActionNames.FilterByPredicate([](const FName actionName) { return actionName.ToString().StartsWith("UseAbility"); });
   abilityActionNames.Sort([](const FName&             a, const FName& b) { return a.FastLess(b); });

   int                            index = 0;
   TArray<FInputActionKeyMapping> outNewActionKeyMappings;
   for(int i = 0; i < abilityActionNames.Num() && i < skillSlots.Num(); ++i)
   {
      const FName& actionName = abilityActionNames[i];
      inputSettings->GetActionMappingByName(actionName, outNewActionKeyMappings);
      skillSlots[index]->SetInfo(FText::FromName(outNewActionKeyMappings[0].ActionName));
      ++index;
   }
}
