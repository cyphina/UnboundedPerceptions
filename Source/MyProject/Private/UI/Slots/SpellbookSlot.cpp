#include "MyProject.h"
#include "SpellbookSlot.h"
#include "RTSAbilitySystemComponent.h"
#include "DraggedActionWidget.h"
#include "UserInput.h"
#include "HUDManager.h"
#include "SpellbookHUD.h"
#include "RTSIngameWidget.h"
#include "RTSSpellbookDrag.h"
#include "SpellBook.h"
#include "SpellFunctionLibrary.h"
#include "ToolTipWidget.h"
#include "SpellSystem/MySpell.h"
#include "WorldObjects/BaseHero.h"
#include "UMG/Public/Components/TextBlock.h"

#define LOCTEXT_NAMESPACE "SpellbookSlot"

const FLinearColor USpellbookSlot::tooHighLevelSpellColor = FLinearColor(0.6, 0, 0.02, 1.0);
const FLinearColor USpellbookSlot::canLearnSpellColor     = FLinearColor(0.62, 0.61, 0, 1.0);
const FLinearColor USpellbookSlot::learnedSpellColor      = FLinearColor::White;

void USpellbookSlot::UpdateSlotColor()
{
   const ABaseHero* heroRef   = GetHeroRef();
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
   if(const ABaseHero* heroRef = GetHeroRef())
   {
      if(USpellBook* spellBook = heroRef->GetSpellBook())
      {
         if(spellBook->HasLearnedSpell(slotIndex))
         {
            const int newLevel = spellBook->GetSpellFromIndex(slotIndex).GetDefaultObject()->GetLevel(heroRef->GetAbilitySystemComponent());
            infoText->SetText(FText::FromString(FString::FromInt(newLevel)));
         }
         else
         {
            infoText->SetText(FText::GetEmpty());
         }
      }
   }
}

void USpellbookSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
   if(const ABaseHero* heroRef = GetHeroRef())
   {
      if(USpellBook* spellBook = heroRef->GetSpellBook())
      {
         if(const TSubclassOf<UMySpell> spellClass = spellBook->GetSpellFromIndex(slotIndex))
         {
            if(spellBook->HasLearnedSpell(slotIndex))
            {
               UDraggedActionWidget* dragVisual = CreateDragIndicator();
               dragVisual->SetDraggedImage(spellClass.GetDefaultObject()->GetSpellDefaults().image);

               URTSSpellbookDrag* dragOp = NewObject<URTSSpellbookDrag>(this);
               dragOp->Pivot             = EDragPivot::CenterCenter;
               dragOp->DefaultDragVisual = dragVisual;
               dragOp->slotIndex         = slotIndex;
               OutOperation              = MoveTemp(dragOp);
            }
         }
      }
   }
}

void USpellbookSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
   const ABaseHero* heroRef = GetHeroRef();
   if(static_cast<unsigned>(slotIndex) >= static_cast<unsigned>(heroRef->GetSpellBook()->GetAvailableSpells().Num()))
   {
      return;
   }

   const auto spellClassRef = heroRef->GetSpellBook()->GetAvailableSpells()[slotIndex];
   if(!spellClassRef)
   {
      return;
   }

   Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void USpellbookSlot::ShowDesc(UToolTipWidget* tooltip)
{
   const ABaseHero*                  heroRef              = GetHeroRef();
   const URTSAbilitySystemComponent* abilitySystemCompRef = heroRef->GetAbilitySystemComponent();

   if(static_cast<unsigned>(slotIndex) >= static_cast<unsigned>(heroRef->GetSpellBook()->GetAvailableSpells().Num()))
   {
      return;
   }

   const auto spellClassRef = heroRef->GetSpellBook()->GetAvailableSpells()[slotIndex];
   if(!spellClassRef)
   {
      return;
   }

   const auto  spellObj           = spellClassRef.GetDefaultObject();
   const int   adjustedSpellLevel = heroRef->GetSpellBook()->HasLearnedSpell(slotIndex) ? spellObj->GetLevel(abilitySystemCompRef) : 0;
   const FText titleAndLevelCap =
       FText::Format(LOCTEXT("SpellSlotTitleAndLevelCap", "{0} ({1}/{2})"), spellObj->GetSpellName(), adjustedSpellLevel, spellObj->GetSpellDefaults().MaxLevel);
   const FText costAndManaDesc = FText::Format(LOCTEXT("SpellSlotCostAndMana", "Costs {0} mana \r\n{1} second cooldown \r\n{2} Element"),
                                               spellObj->GetCost(abilitySystemCompRef), spellObj->GetCDDuration(abilitySystemCompRef), spellObj->GetElem());
   FText       preReqNamesDesc =
       spellObj->GetPreReqNames().Num() > 0 ? LOCTEXT("LearnedSpellReqsStart", "Must have learned ") : LOCTEXT("NoSpellReqs", "No spell requirements");
   const FText levelRequirementString = FText::Format(LOCTEXT("LevelReq", "Requires Level: {0}"), spellObj->GetReqLevel(abilitySystemCompRef));

   for(auto& preReqName : spellObj->GetPreReqNames())
   {
      preReqNamesDesc = FText::Format(LOCTEXT("LearnedSpellReqs", "{0} - {1}"), preReqNamesDesc, preReqName);
   }

   if(IsValid(spellObj))
   {
      if(AUserInput* CPCRef = Cast<AUserInput>(GetOwningPlayer<AUserInput>()))
      {
         tooltip->SetupTTBoxText(titleAndLevelCap, levelRequirementString, preReqNamesDesc,
                                 USpellFunctionLibrary::ParseDesc(spellObj->GetDescription(), abilitySystemCompRef, spellObj), costAndManaDesc);
      }
   }
}

ABaseHero* USpellbookSlot::GetHeroRef() const
{
   if(AUserInput* CPCRef = Cast<AUserInput>(GetOwningPlayer<AUserInput>()))
   {
      return CPCRef->GetWidgetProvider()->GetIngameHUD()->GetSpellBookMenu()->GetHeroFromSpellbook();
   }
   return nullptr;
}

#undef LOCTEXT_NAMESPACE
