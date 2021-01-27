#include "MyProject.h"
#include "SpellbookSlot.h"

#include "DraggedActionWidget.h"
#include "UserInput.h"
#include "HUDManager.h"
#include "SpellbookHUD.h"
#include "RTSIngameWidget.h"
#include "RTSSpellbookDrag.h"
#include "SpellBook.h"
#include "ToolTipWidget.h"
#include "SpellSystem/MySpell.h"
#include "WorldObjects/BaseHero.h"
#include "UMG/Public/Components/TextBlock.h"

#define LOCTEXT_NAMESPACE "SpellbookSlot"

void USpellbookSlot::UpdateSlotColor()
{
   const ABaseHero* heroRef   = CPCRef->GetWidgetProvider()->GetIngameHUD()->GetSpellBookMenu()->GetHeroRef();
   USpellBook*      spellBook = heroRef->GetSpellBook();

   if(spellBook->GetLearnedSpellIndices().Contains(slotIndex))
   {
      SetColorAndOpacity(learnedSpellColor);
   }
   else if(spellBook->GetLearnableSpellsIndices().Contains(slotIndex))
   {
      SetColorAndOpacity(canLearnSpellColor);
   }
   else if(spellBook->GetUnknownSpellIndices().Contains(slotIndex))
   {
      SetColorAndOpacity(tooHighLevelSpellColor);
   }
}

void USpellbookSlot::UpdateSlotLevelText()
{
   if(const ABaseHero* heroRef = CPCRef->GetWidgetProvider()->GetIngameHUD()->GetSpellBookMenu()->GetHeroRef())
   {
      if(USpellBook* spellBook = heroRef->GetSpellBook())
      {
         const int newLevel = spellBook->GetSpellFromIndex(slotIndex).GetDefaultObject()->GetLevel(heroRef->GetAbilitySystemComponent());
         infoText->SetText(FText::FromString(FString::FromInt(newLevel + 1)));
      }
   }
}

void USpellbookSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
   if(const ABaseHero* heroRef = CPCRef->GetWidgetProvider()->GetIngameHUD()->GetSpellBookMenu()->GetHeroRef())
   {
      if(USpellBook* spellBook = heroRef->GetSpellBook())
      {
         if(const TSubclassOf<UMySpell> spellClass = spellBook->GetSpellFromIndex(slotIndex))
         {
            if(spellBook->HasLearnedSpell(slotIndex))
            {
               UDraggedActionWidget* dragVisual = CreateWidget<UDraggedActionWidget>(this, draggedActionWidgetClass);
               dragVisual->SetDraggedImage(spellClass.GetDefaultObject()->GetSpellDefaults().image);

               URTSSpellbookDrag* dragOp = NewObject<URTSSpellbookDrag>(this);
               dragOp->Pivot = EDragPivot::CenterCenter;
               dragOp->DefaultDragVisual = dragVisual;
               dragOp->slotIndex = slotIndex;
               OutOperation = MoveTemp(dragOp);
            }
         }
      }
   }

}

void USpellbookSlot::ShowDesc(UToolTipWidget* tooltip)
{
   const auto heroRef              = CPCRef->GetWidgetProvider()->GetIngameHUD()->GetSpellBookMenu()->GetHeroRef();
   const auto abilitySystemCompRef = heroRef->GetAbilitySystemComponent();

   if(static_cast<unsigned>(slotIndex) >= static_cast<unsigned>(heroRef->GetSpellBook()->GetAvailableSpells().Num()))
   {
      return;
   }

   const auto spellClassRef = heroRef->GetSpellBook()->GetAvailableSpells()[slotIndex];
   if(!spellClassRef)
   {
      return;
   }

   const auto  spellObj        = spellClassRef.GetDefaultObject();
   const FText costAndManaDesc = FText::Format(LOCTEXT("SpellSlotCostAndMana", "Costs {0} mana \r\n{1} second cooldown \r\n{2} Element"),
                                               spellObj->GetCost(abilitySystemCompRef), spellObj->GetCDDuration(abilitySystemCompRef), spellObj->GetElem());
   FText preReqNamesDesc =
   spellObj->GetPreReqNames().Num() > 0 ? LOCTEXT("LearnedSpellReqsStart", "Must have learned - ") : LOCTEXT("NoSpellReqs", "No spell requirements");
   const FText levelRequirementString = FText::Format(LOCTEXT("LevelReq", "Requires Level: {0}"), spellObj->GetReqLevel(abilitySystemCompRef));

   for(auto& preReqName : spellObj->GetPreReqNames())
   {
      preReqNamesDesc = FText::Format(LOCTEXT("LearnedSpellReqs", "{0} - {1}"), preReqNamesDesc, preReqName);
   }

   if(IsValid(spellObj))
   {
      tooltip->SetupTTBoxText(spellObj->GetSpellName(), levelRequirementString, preReqNamesDesc, spellObj->GetDescription(), costAndManaDesc);
   }
}
#undef LOCTEXT_NAMESPACE
