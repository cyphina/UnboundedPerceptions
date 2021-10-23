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
   AUnit* ChannelingUnit = controllerRef->GetBasePlayer()->GetFocusedUnit();
   if(IsValid(ChannelingUnit))
   {
      if(AUnitController* ChannelingUnitController = ChannelingUnit->GetUnitController())
      {
         if(USpellCastComponent* spellCastComp = ChannelingUnitController->FindComponentByClass<USpellCastComponent>())
         {
            const TSubclassOf<UMySpell> ChanneledSpellClass = spellCastComp->GetCurrentSpell();
            if(IsValid(ChanneledSpellClass))
            {
               return ChanneledSpellClass.GetDefaultObject()->GetSpellName();
            }
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
   if(ABasePlayer* BasePlayer = controllerRef->GetBasePlayer())
   {
      AUnit* ChannelingUnit = BasePlayer->GetFocusedUnit();

      if(IsValid(ChannelingUnit))
      {
         if(AUnitController* UnitController = ChannelingUnit->GetUnitController())
         {
            if(USpellCastComponent* channelingComp = UnitController->FindComponentByClass<USpellCastComponent>())
            {
               if(channelingComp->GetCurrentChannelingTime() > 0 || channelingComp->GetCurrentIncantationTime() > 0)
               {
                  return ESlateVisibility::SelfHitTestInvisible;
               }
            }
         }
      }
   }
   return ESlateVisibility::Hidden;
}
