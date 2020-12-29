// Fill out your copyright notice in the Description page of Project Settings.

#include "../ESkillContainer.h"
#include "ActionbarInterface.h"

#include "BasePlayer.h"
#include "../Slots/SkillSlot.h"
#include "PartyDelegateStore.h"
#include "Unit.h"
#include "UserInput.h"

void UActionbarInterface::UseSkill(int skillIndex)
{
   if(this->IsVisible()) skillContainerRef->UseSkill(skillIndex);
}

void UActionbarInterface::NativeOnInitialized()
{
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateStore>()->OnFocusedUnitChanged().AddUObject(this,
                                                                                                                           &UActionbarInterface::OnFocusedUnitChanged);
}

void UActionbarInterface::OnFocusedUnitChanged(AUnit* newFocusedUnit)
{
   if(newFocusedUnit->GetIsEnemy()) {
      // TODO: Set enemy view
      return;   
   }

   if(CPC->GetBasePlayer()->selectedAllies.Num() > 1) {
      // TODO: Set multi ally view
   }
   else {
      // TODO: Set single ally view
   }
}
