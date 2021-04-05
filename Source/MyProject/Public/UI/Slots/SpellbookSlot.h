#pragma once

#include "CoreMinimal.h"
#include "UI/Slots/ActionSlot.h"
#include "SpellbookSlot.generated.h"

class ABaseHero;
/**
 * Slot for skills we can learn inside the spellbook
 */
UCLASS()
class MYPROJECT_API USpellbookSlot : public UActionSlot
{
   GENERATED_BODY()
   
public:
   /** Updates the color of this slot to indicate whether we can learn the spell in this slot or we cannot or we have already learned it */
   void UpdateSlotColor();

   /** Updates the level indicator text (telling us what level this spell is at)*/
   void UpdateSlotLevelText();

protected:
   void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
   void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
   void ShowDesc(UToolTipWidget* tooltip) override;
   ABaseHero* GetHeroRef() const;

   static const inline FLinearColor tooHighLevelSpellColor = FLinearColor(0.6, 0, 0.02, 1.0);
   static const inline FLinearColor canLearnSpellColor    = FLinearColor(0.62, 0.61, 0, 1.0);
   static const inline FLinearColor learnedSpellColor = FLinearColor::White;
   
};
