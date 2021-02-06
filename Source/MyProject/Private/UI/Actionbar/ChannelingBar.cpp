// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "ChannelingBar.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "Unit.h"
#include "UnitController.h"
#include "SpellSystem/MySpell.h"
#include "SpellCastComponent.h"

void UChannelingBar::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   controllerRef = GetOwningPlayer<AUserInput>();
}

FText UChannelingBar::GetChannelingName()
{
   AUnit* channelingUnit = controllerRef->GetBasePlayer()->GetFocusedUnit();
   if(IsValid(channelingUnit))
   {
      if(USpellCastComponent* spellCastComp = channelingUnit->GetUnitController()->FindComponentByClass<USpellCastComponent>())
      {
         const TSubclassOf<UMySpell> channeledSpellClass = spellCastComp->GetCurrentSpell();
         if(IsValid(channeledSpellClass))
         {
            return channeledSpellClass.GetDefaultObject()->GetSpellName();
         }
      }
   }
   return FText();
}

float UChannelingBar::GetSpellChannelProgress()
{
   // TODO: Update this via timer
   // The unit could die while we have the channelingbar visible so we need a null check
   AUnit* channelingUnit = controllerRef->GetBasePlayer()->GetFocusedUnit();
   if(IsValid(channelingUnit))
   {
      if(USpellCastComponent* spellCastComp = channelingUnit->GetUnitController()->FindComponentByClass<USpellCastComponent>())
      {
         if(spellCastComp->GetCurrentIncantationTime() > 0)
         {
            const float spellCastTime = spellCastComp->GetCurrentSpell().GetDefaultObject()->GetCastTime(channelingUnit->GetAbilitySystemComponent());

            if(spellCastTime > 0)
            {
               return spellCastComp->GetCurrentIncantationTime() / spellCastTime;
            }
         }

         if(spellCastComp->GetCurrentChannelingTime() > 0)
         {
            const float spellCastTime = spellCastComp->GetCurrentSpell().GetDefaultObject()->GetSecondaryTime(channelingUnit->GetAbilitySystemComponent());

            if(spellCastTime > 0)
            {
               return spellCastComp->GetCurrentChannelingTime() / spellCastTime;
            }
         }
      }
   }
   return 0;
}

ESlateVisibility UChannelingBar::IsFocusedUnitChanneling()
{
   AUnit* channelingUnit = controllerRef->GetBasePlayer()->GetFocusedUnit();

   if(IsValid(channelingUnit))
   {
      if(USpellCastComponent* channelingComp = channelingUnit->GetUnitController()->FindComponentByClass<USpellCastComponent>())
      {
         if(channelingComp->GetCurrentChannelingTime() > 0 || channelingComp->GetCurrentIncantationTime() > 0)
         {
            return ESlateVisibility::SelfHitTestInvisible;
         }
      }
   }
   return ESlateVisibility::Hidden;
}
