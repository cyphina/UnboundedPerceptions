// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UserInput.h"
#include "SpellbookHUD.h"

#include "BaseHero.h"
#include "BasePlayer.h"
#include "Button.h"
#include "Border.h"
#include "HUDManager.h"
#include "MySpell.h"
#include "SpellBook.h"
#include "SpellbookSlot.h"
#include "TextBlock.h"

void USpellbookHUD::NativeConstruct()
{
   btnClose->OnClicked.AddDynamic(this, &USpellbookHUD::CloseWidget);
   btnSpendSkillPoints->OnClicked.AddDynamic(this, &USpellbookHUD::ChangeBackgroundColorWhenLeveling);
   return Super::NativeConstruct();
}

bool USpellbookHUD::OnWidgetAddToViewport_Implementation()
{
   if(heroRef = CPC->GetBasePlayer()->selectedHeroes[0]; heroRef) {
      Update();
      return true;
   }
   return false;
}

void USpellbookHUD::CloseWidget()
{
   hudManagerRef->AddHUD(static_cast<uint8>(EHUDs::HS_Spellbook));
}

void USpellbookHUD::ChangeBackgroundColorWhenLeveling()
{
   auto red         = FLinearColor(0.5, 0.02, 0.05, 0.8);
   auto paleBlue = FLinearColor(0.78, 0.85, 0.91, 1);
   bLevelingUp = !bLevelingUp;
   bLevelingUp ? hudBackground->SetBrushColor(red) : hudBackground->SetBrushColor(paleBlue);
}

void USpellbookHUD::Update()
{
   txtPoints->SetText(FText::FromString(FString::FromInt(heroRef->GetSkillPoints())));
   // This shouldn't happen but lets check just in case
   int numSpellsToUpdate = FMath::Min(heroRef->GetSpellBook()->availableSpells.Num(), spellbookSlots.Num());

   // Setup the images and display some text telling us what level we have to be below the icon
   for(int i = 0; i < numSpellsToUpdate; ++i) {
      if(TSubclassOf<UMySpell> spell = heroRef->GetSpellBook()->availableSpells[i]; IsValid(spell)) {
         const auto spellDefaultObj = spell.GetDefaultObject();
         spellbookSlots[i]->SetSlotImage(spellDefaultObj->spellDefaults.image);
         const int spellLevel = spellDefaultObj->GetLevel(heroRef->GetAbilitySystemComponent());
         if(spellLevel != 0)
            spellbookSlots[i]->UpdateSlotLevelText(spellLevel);
      }
   }

   // Change all the spell slots for spells that we cannot learn because they are too high level to be red color
   numSpellsToUpdate                                = FMath::Min(heroRef->GetSpellBook()->GetUnknownSpells().Num(), spellbookSlots.Num());
   static const FLinearColor tooHighLevelSpellColor = FLinearColor(0.6, 0, 0.02, 1.0);

   for(int i = 0; i < numSpellsToUpdate; ++i) {
      spellbookSlots[heroRef->GetSpellBook()->GetUnknownSpells()[i]]->SetColorAndOpacity(tooHighLevelSpellColor);
   }

   // Change all spells that we can learn to yellow
   numSpellsToUpdate                            = FMath::Min(heroRef->GetSpellBook()->GetLearnableSpells().Num(), spellbookSlots.Num());
   static const FLinearColor canLearnSpellColor = FLinearColor(0.62, 0.61, 0, 1.0);

   for(int i = 0; i < numSpellsToUpdate; ++i) {
      spellbookSlots[heroRef->GetSpellBook()->GetLearnableSpells()[i]]->SetColorAndOpacity(canLearnSpellColor);
   }

   // Change all spells we learned to white (since we reuse the hud for different heros so some slots may be colors we do not want from other heros opening the menu)
   numSpellsToUpdate = FMath::Min(heroRef->GetSpellBook()->GetLearnedSpells().Num(), spellbookSlots.Num());

   for(int i = 0; i < numSpellsToUpdate; ++i) {
      spellbookSlots[heroRef->GetSpellBook()->GetLearnedSpells()[i]]->SetColorAndOpacity(FLinearColor::White);
   }
}
