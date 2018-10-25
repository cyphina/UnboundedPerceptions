// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "../ESkillContainer.h"
#include "ActionbarInterface.h"
#include "../Slots/SkillSlot.h"

void UActionbarInterface::UseSkill(int skillIndex)
{
   if (this->IsVisible()) skillContainerRef->UseSkill(skillIndex);
}

void UActionbarInterface::ShowSkillVisualCD(int skillIndex)
{
   if (skillContainerRef->GetSkillSlot(skillIndex)) skillContainerRef->GetSkillSlot(skillIndex)->PlayTimeline(0);
}
