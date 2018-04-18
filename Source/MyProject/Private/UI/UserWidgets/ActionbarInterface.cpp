// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"

#include "../ESkillContainer.h"
#include "ActionbarInterface.h"

#include "UserInput.h"
#include "BasePlayer.h"
#include "WorldObjects/Unit.h"

#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "../Slots/SkillSlot.h"
#include "ResourceManager.h"

void UActionbarInterface::UseSkill(int skillIndex)
{
	skillContainerRef->UseSkill(skillIndex);
}

void UActionbarInterface::ShowSkillVisualCD(int skillIndex)
{
	if(skillContainerRef->GetSkill(skillIndex))
		skillContainerRef->GetSkill(skillIndex)->PlayTimeline(0);
}

