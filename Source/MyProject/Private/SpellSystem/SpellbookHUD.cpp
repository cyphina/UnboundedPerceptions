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
#include "SpellDelegateStore.h"
#include "TextBlock.h"

void USpellbookHUD::NativeConstruct()
{
   btnClose->OnClicked.AddDynamic(this, &USpellbookHUD::CloseWidget);
   btnSpendSkillPoints->OnClicked.AddDynamic(this, &USpellbookHUD::ChangeBackgroundColorWhenLeveling);
   btnSpendSkillPoints->OnClicked.AddDynamic(this, &USpellbookHUD::ChangeBackgroundColorWhenLeveling);
   SpellHUDEvents::OnSpellLearnedEvent.AddUObject(this, &USpellbookHUD::OnSpellLearned);
   SpellHUDEvents::OnSpellUpgradedEvent.AddUObject(this, &USpellbookHUD::OnSpellUpgraded);
   return Super::NativeConstruct();
}

bool USpellbookHUD::OnWidgetAddToViewport_Implementation()
{
   heroWithOpenSpellbookRef = CPC->GetBasePlayer()->selectedHeroes[0];
   if(heroWithOpenSpellbookRef)
   {
      ResetHUDForNewHero();
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
   const auto red      = FLinearColor(0.5, 0.02, 0.05, 0.8);
   const auto paleBlue = FLinearColor(0.78, 0.85, 0.91, 1);
   bLevelingUp         = !bLevelingUp;
   bLevelingUp ? hudBackground->SetBrushColor(red) : hudBackground->SetBrushColor(paleBlue);
}

void USpellbookHUD::ResetHUDForNewHero()
{
   txtPoints->SetText(FText::FromString(FString::FromInt(heroWithOpenSpellbookRef->GetSkillPoints())));
   UpdateSpellSlotImageAndLevelText();
   ColorUnknownSpellSlots();
   ColorLearnableSpellSlots();
   ColorLearnedSpellIndices();
}

void USpellbookHUD::UpdateSpellSlotImageAndLevelText()
{
   const int numAvailableSpellsToUpdate = FMath::Min(heroWithOpenSpellbookRef->GetSpellBook()->GetAvailableSpells().Num(), spellbookSlots.Num());
   for(int i = 0; i < numAvailableSpellsToUpdate; ++i)
   {
      if(TSubclassOf<UMySpell> spell = heroWithOpenSpellbookRef->GetSpellBook()->GetAvailableSpells()[i])
      {
         const auto spellDefaultObj = spell.GetDefaultObject();
         spellbookSlots[i]->SetSlotImage(spellDefaultObj->GetSpellDefaults().image);
         spellbookSlots[i]->UpdateSlotLevelText();
      }
   }
}

void USpellbookHUD::ColorUnknownSpellSlots()
{
   TArray<int> unknownSpellIndices = heroWithOpenSpellbookRef->GetSpellBook()->GetUnknownSpellIndices();

   for(const int index : unknownSpellIndices)
   {
      if(index < spellbookSlots.Num())
      {
         spellbookSlots[index]->SetColorAndOpacity(tooHighLevelSpellColor);
      }
   }
}

void USpellbookHUD::ColorLearnableSpellSlots()
{
   TArray<int> learnableSpellIndices = heroWithOpenSpellbookRef->GetSpellBook()->GetLearnableSpellsIndices();

   for(const int index : learnableSpellIndices)
   {
      if(index < spellbookSlots.Num())
      {
         spellbookSlots[index]->SetColorAndOpacity(canLearnSpellColor);
      }
   }
}

void USpellbookHUD::ColorLearnedSpellIndices()
{
   TArray<int> learnedSpellIndices = heroWithOpenSpellbookRef->GetSpellBook()->GetLearnedSpellIndices();
   for(const int index : learnedSpellIndices)
   {
      if(index < spellbookSlots.Num())
      {
         spellbookSlots[index]->SetColorAndOpacity(FColor::White);
      }
   }
}

void USpellbookHUD::OnSpellLearned(TSubclassOf<UMySpell> spellClass)
{
   USpellBook*  spellBook                 = heroWithOpenSpellbookRef->GetSpellBook();
   const uint32 slotWithLearnedSpellIndex = spellBook->GetAvailableSpells().Find(spellClass);
   spellbookSlots[slotWithLearnedSpellIndex]->SetColorAndOpacity(FColor::White);
   spellbookSlots[slotWithLearnedSpellIndex]->UpdateSlotLevelText();

   for(int i = spellBook->GetLearnableSpellsIndices().Num(); i > 0; ++i)
   {
      if(spellBook->GetSpellFromIndex(i).GetDefaultObject()->GetSpellDefaults().PreReqs.Contains(spellClass))
         spellbookSlots[i]->SetColorAndOpacity(canLearnSpellColor);
   }
}

void USpellbookHUD::OnSpellUpgraded(TSubclassOf<UMySpell> spellClass)
{
   const uint32 slotWithUpgradedSpellIndex = heroWithOpenSpellbookRef->GetSpellBook()->GetAvailableSpells().Find(spellClass);
   spellbookSlots[slotWithUpgradedSpellIndex]->UpdateSlotLevelText();
}

