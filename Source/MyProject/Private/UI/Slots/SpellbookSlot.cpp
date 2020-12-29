// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "SpellbookSlot.h"
#include "UserInput.h"
#include "HUDManager.h"
#include "SpellbookHUD.h"
#include "RTSIngameWidget.h"
#include "SpellBook.h"
#include "ToolTipWidget.h"
#include "SpellSystem/MySpell.h"
#include "WorldObjects/BaseHero.h"
#include "UMG/Public/Components/TextBlock.h"

#define LOCTEXT_NAMESPACE "SpellbookSlot"

void USpellbookSlot::OnBtnClick()
{
   if(CPCRef->GetWidgetProvider()->GetIngameHUD()->GetSpellBookMenu()->bLevelingUp) {
      CPCRef->GetWidgetProvider()->GetIngameHUD()->GetSpellBookMenu()->GetHeroRef()->GetSpellBook()->LearnSpell(slotIndex);
      CPCRef->GetWidgetProvider()->GetIngameHUD()->GetSpellBookMenu()->Update();
   } else {
      CPCRef->GetWidgetProvider()->GetIngameHUD()->DisplayHelpText(LOCTEXT("PressUpgradeButtonSpellLevelup", "Press the upgrade button before levling up a spell!"));
   }
}

void USpellbookSlot::ShowDesc(UToolTipWidget* tooltip)
{
   const auto heroRef              = CPCRef->GetWidgetProvider()->GetIngameHUD()->GetSpellBookMenu()->GetHeroRef();
   const auto abilitySystemCompRef = heroRef->GetAbilitySystemComponent();
   if(unsigned(slotIndex) >= unsigned(heroRef->GetSpellBook()->availableSpells.Num()))
      return; // If the person hovers over an empty slot
   const auto spellClassRef = heroRef->GetSpellBook()->availableSpells[slotIndex];
   const auto spellObj      = spellClassRef.GetDefaultObject();

   const FText costAndManaDesc = FText::Format(LOCTEXT("SpellSlotCostAndMana", "Costs {0} mana \r\n{1} second cooldown \r\n{2} Element"),
                                               spellObj->GetCost(abilitySystemCompRef), spellObj->GetCDDuration(abilitySystemCompRef), spellObj->GetElem());
   FText       preReqNamesDesc =
       spellObj->GetPreReqNames().Num() > 0 ? LOCTEXT("LearnedSpellReqsStart", "Must have learned - ") : LOCTEXT("NoSpellReqs", "No spell requirements");
   const FText levelRequirementString = FText::Format(LOCTEXT("LevelReq", "Requires Level: {0}"), spellObj->GetReqLevel(abilitySystemCompRef));

   // Get names of all the prereq spells and format them into text
   for(auto& preReqName : spellObj->GetPreReqNames()) {
      preReqNamesDesc = FText::Format(LOCTEXT("LearnedSpellReqs", "{0} - {1}"), preReqNamesDesc, preReqName);
   }

   if(IsValid(spellObj)) {
      tooltip->SetupTTBoxText(spellObj->GetName(), levelRequirementString, preReqNamesDesc, spellObj->GetDescription(), costAndManaDesc);
   }
}

void USpellbookSlot::UpdateSlotLevelText(int newLevel) const
{
   infoText->SetText(FText::FromString(FString::FromInt(newLevel)));
}

#undef LOCTEXT_NAMESPACE