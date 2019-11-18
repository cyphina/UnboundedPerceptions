// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "../ESkillContainer.h"
#include "ActionbarInterface.h"
#include "../Slots/SkillSlot.h"
#include "MySpell.h"

#include "Ally.h"

void UActionbarInterface::UseSkill(int skillIndex)
{
   if (this->IsVisible()) skillContainerRef->UseSkill(skillIndex);
}

void UActionbarInterface::ShowSkillVisualCD(int skillIndex)
{
   //Make sure we choose an active skill slot and make sure the spell has a cooldown to show
   if (skillContainerRef->GetSkillSlot(skillIndex) && skillContainerRef->GetAllyRef()->abilities[skillIndex].GetDefaultObject()->
      GetCDDuration(skillContainerRef->GetAllyRef()->GetAbilitySystemComponent()) > 0) 
      skillContainerRef->GetSkillSlot(skillIndex)->PlayTimeline(0);
}
