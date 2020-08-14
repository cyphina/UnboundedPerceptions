// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "ChannelingBar.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "Unit.h"
#include "SpellSystem/MySpell.h"

FText UChannelingBar::GetChannelingName()
{
   AUnit* channelingUnit = controllerRef->GetBasePlayer()->focusedUnit;
   if (IsValid(channelingUnit)) {
      TSubclassOf<UMySpell> channeledSpellClass = channelingUnit->GetCurrentSpell();
      if (IsValid(channeledSpellClass)) { return channeledSpellClass.GetDefaultObject()->GetName(); }
   }
   return FText();
}

float UChannelingBar::GetSpellChannelProgress()
{
   // The unit could die while we have the channelbar visible so we need a nullcheck
   AUnit* channelingUnit = controllerRef->GetBasePlayer()->focusedUnit;
   if (IsValid(channelingUnit)) {
      // TODO: Update this via delegate I suppose
      return channelingUnit->GetCurrentChannelTime() / channelingUnit->GetChannelTime();
   }
   SetVisibility(ESlateVisibility::Hidden);
   return 0;
}

ESlateVisibility UChannelingBar::IsFocusedUnitChanneling()
{
   AUnit* channelingUnit = controllerRef->GetBasePlayer()->focusedUnit;
   if (IsValid(channelingUnit) && (channelingUnit->GetState() == EUnitState::STATE_CHANNELING || channelingUnit->GetState() == EUnitState::STATE_INCANTATION)) 
      return ESlateVisibility::SelfHitTestInvisible;

   return ESlateVisibility::Hidden;
}
