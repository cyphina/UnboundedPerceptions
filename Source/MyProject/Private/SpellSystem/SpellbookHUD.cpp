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
#include "PartyDelegateContext.h"
#include "RTSIngameWidget.h"
#include "SpellBook.h"
#include "SpellbookSlot.h"
#include "SpellDelegateStore.h"
#include "TextBlock.h"
#include "UpStatComponent.h"

void USpellbookHUD::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   btnClose->OnClicked.AddDynamic(this, &USpellbookHUD::CloseWidget);
   btnSpendSkillPoints->OnClicked.AddDynamic(this, &USpellbookHUD::ChangeBackgroundColorWhenLeveling);
   SpellGameContext::OnSpellLearnedEvent.AddUObject(this, &USpellbookHUD::OnSpellLearned);
   SpellGameContext::OnSpellUpgradedEvent.AddUObject(this, &USpellbookHUD::OnSpellUpgraded);
   GetOwningLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnHeroLevelUp().AddUObject(this, &USpellbookHUD::OnHeroLeveledUp);
}

bool USpellbookHUD::OnWidgetAddToViewport_Implementation()
{
   heroWithOpenSpellbookRef = CPC->GetBasePlayer()->GetSelectedHeroes()[0];
   if(heroWithOpenSpellbookRef && heroWithOpenSpellbookRef->GetSpellBook())
   {
      ResetHUDForNewHero();
      return true;
   }
   return false;
}

void USpellbookHUD::OnWidgetRemovedFromViewport_Implementation()
{
   heroWithOpenSpellbookRef->OnLevelUp().RemoveAll(this);
   if(bLevelingUp)
   {
      ChangeBackgroundColorWhenLeveling();
   }
}

void USpellbookHUD::CloseWidget()
{
   hudManagerRef->HideHUD(EHUDs::HS_Spellbook);
}

void USpellbookHUD::ChangeBackgroundColorWhenLeveling()
{
   const auto red      = FLinearColor(0.5, 0.02, 0.05, 0.8);
   const auto paleBlue = FLinearColor(0.78, 0.85, 0.91, 1);
   bLevelingUp         = !bLevelingUp;
   bLevelingUp ? hudBackground->SetBrushColor(red) : hudBackground->SetBrushColor(paleBlue);
}

int USpellbookHUD::GetNumValidItems() const
{
   return heroWithOpenSpellbookRef->GetSpellBook()->GetAvailableSpells().Num();
}

FReply USpellbookHUD::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
   if(GetSelectedSlotIndex() != INDEX_NONE)
   {
      if(bLevelingUp)
      {
         Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
      } else
      {
         URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("SpellbookHUD", "PressUpgradeButtonSpellLevelup",
                                                                       "Press the upgrade button before levling up a spell!"));
         SetSelectedSlotIndex(INDEX_NONE);
      }
      return FReply::Handled();
   }
   return FReply::Unhandled();
}

void USpellbookHUD::ResetHUDForNewHero()
{
   txtPoints->SetText(FText::AsNumber(heroWithOpenSpellbookRef->GetSkillPoints()));
   txtLevel->SetText(FText::AsNumber(heroWithOpenSpellbookRef->GetStatComponent()->GetUnitLevel()));
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
   
   for(int i = numAvailableSpellsToUpdate; i < spellbookSlots.Num(); ++i)
   {
      spellbookSlots[i]->SetSlotImage(nullptr);
      spellbookSlots[i]->UpdateSlotLevelText();
      spellbookSlots[i]->SetColorAndOpacity(FColor::White);
   }
}

void USpellbookHUD::ColorUnknownSpellSlots()
{
   TArray<int> unknownSpellIndices = heroWithOpenSpellbookRef->GetSpellBook()->GetUnknownSpellIndices();

   for(const int index : unknownSpellIndices)
   {
      if(index < spellbookSlots.Num()) { spellbookSlots[index]->SetColorAndOpacity(tooHighLevelSpellColor); }
   }
}

void USpellbookHUD::ColorLearnableSpellSlots()
{
   TArray<int> learnableSpellIndices = heroWithOpenSpellbookRef->GetSpellBook()->GetLearnableSpellsIndices();

   for(const int index : learnableSpellIndices)
   {
      if(index < spellbookSlots.Num()) { spellbookSlots[index]->SetColorAndOpacity(canLearnSpellColor); }
   }
}

void USpellbookHUD::ColorLearnedSpellIndices()
{
   TArray<int> learnedSpellIndices = heroWithOpenSpellbookRef->GetSpellBook()->GetLearnedSpellIndices();
   for(const int index : learnedSpellIndices)
   {
      if(index < spellbookSlots.Num()) { spellbookSlots[index]->SetColorAndOpacity(FColor::White); }
   }
}

void USpellbookHUD::OnSpellLearned(const ABaseHero& heroThatLearnedSpell, TSubclassOf<UMySpell> spellClass)
{
   if(heroWithOpenSpellbookRef && heroWithOpenSpellbookRef == &heroThatLearnedSpell)
   {
      USpellBook*  spellBook                 = heroWithOpenSpellbookRef->GetSpellBook();
      const uint32 slotWithLearnedSpellIndex = spellBook->GetAvailableSpells().Find(spellClass);
      spellbookSlots[slotWithLearnedSpellIndex]->SetColorAndOpacity(FColor::White);
      spellbookSlots[slotWithLearnedSpellIndex]->UpdateSlotLevelText();

      for(int i = 0; i < spellBook->GetLearnableSpellsIndices().Num(); ++i)
      {
         if(spellBook->GetSpellFromIndex(i).GetDefaultObject()->GetSpellDefaults().PreReqs.Contains(spellClass))
            spellbookSlots[i]->SetColorAndOpacity(canLearnSpellColor);
      }

      GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
      {
         txtPoints->SetText(FText::AsNumber(heroWithOpenSpellbookRef->GetSkillPoints()));
      });
   }
}

void USpellbookHUD::OnSpellUpgraded(const ABaseHero& heroThatLearnedSpell, TSubclassOf<UMySpell> spellClass)
{
   if(heroWithOpenSpellbookRef && heroWithOpenSpellbookRef == &heroThatLearnedSpell)
   {
      const uint32 slotWithUpgradedSpellIndex = heroWithOpenSpellbookRef->GetSpellBook()->GetAvailableSpells().Find(spellClass);
      spellbookSlots[slotWithUpgradedSpellIndex]->UpdateSlotLevelText();
   }

   GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
   {
      txtPoints->SetText(FText::AsNumber(heroWithOpenSpellbookRef->GetSkillPoints()));
   });
}

void USpellbookHUD::OnHeroLeveledUp(ABaseHero* heroThatLeveledUp)
{
   if(heroThatLeveledUp && GetHeroRef())
   {
      if(heroThatLeveledUp == GetHeroRef())
      {
         GetWorld()->GetTimerManager().SetTimerForNextTick(
         [this]()
         {
            txtPoints->SetText(FText::AsNumber(heroWithOpenSpellbookRef->GetSkillPoints()));
         });
      }
   }
}
