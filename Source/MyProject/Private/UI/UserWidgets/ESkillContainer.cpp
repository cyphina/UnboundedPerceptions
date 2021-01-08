// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "ESkillContainer.h"

#include "BasePlayer.h"
#include "ManualSpellComponent.h"
#include "UserInput.h"
#include "Slots/ActionSlot.h"
#include "WorldObjects/Ally.h"
#include "SpellSystem/SpellDelegateStore.h"
#include "Slots/SkillSlot.h"

void UESkillContainer::NativeConstruct()
{
   Super::NativeConstruct();
   SpellHUDEvents::OnFocusedUnitSpellCastedEvent.AddUObject(this, &UESkillContainer::OnFocusedUnitSpellCasted);
   if(ABasePlayer* basePlayer = Cast<AUserInput>(GetWorld()->GetFirstPlayerController())->GetBasePlayer()) { focusedUnitRef = basePlayer->GetFocusedUnit(); }
}

USkillSlot* UESkillContainer::GetSkillSlot(int index) const
{
   if(index >= 0 && index < focusedUnitRef->GetAbilitySystemComponent()->GetAbilities().Num()) { return skillSlots[index]; }
   return nullptr;
}

void UESkillContainer::OnFocusedUnitSpellCasted(AUnit* focusedUnit, int spellIndex)
{
   if(USkillSlot* skillSlot = GetSkillSlot(spellIndex)) { skillSlot->ShowCooldown(); }
}

void UESkillContainer::UseSkill(int skillToUseIndex)
{
   if(skillToUseIndex >= 0 && skillToUseIndex < focusedUnitRef->GetAbilitySystemComponent()->GetAbilities().Num()) {
      if(UManualSpellComponent* manualSpellComp = focusedUnitRef->FindComponentByClass<UManualSpellComponent>()) { manualSpellComp->PressedCastSpell(skillToUseIndex); }
   }
}
