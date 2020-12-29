// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "ESkillContainer.h"
#include "Slots/ActionSlot.h"
#include "WorldObjects/Ally.h"
#include "SpellSystem/SpellDelegateStore.h"
#include "Slots/SkillSlot.h"

void UESkillContainer::NativeConstruct()
{
   Super::NativeConstruct();
   SpellHUDEvents::OnFocusedUnitSpellCastedEvent.AddUObject(this, &UESkillContainer::OnFocusedUnitSpellCasted);
}

USkillSlot* UESkillContainer::GetSkillSlot(int index) const
{
   if(index >= 0 && index < allyRef->abilities.Num()) { return skillSlots[index]; }
   return nullptr;
}

void UESkillContainer::OnFocusedUnitSpellCasted(AUnit* focusedUnit, int spellIndex)
{
   if(GetSkillSlot(spellIndex)) { skillSlots[spellIndex]->ShowCooldown(); }
}

void UESkillContainer::UseSkill(int skillToUseIndex)
{
   if(skillToUseIndex >= 0 && skillToUseIndex < allyRef->abilities.Num()) { GetSkillSlot(skillToUseIndex)->PerformAction(); }
}
