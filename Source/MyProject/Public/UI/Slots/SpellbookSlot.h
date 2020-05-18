#pragma once

#include "CoreMinimal.h"
#include "UI/Slots/ActionSlot.h"
#include "SpellbookSlot.generated.h"

/**
 * Slot for skills we can learn inside the spellbook
 */
UCLASS()
class MYPROJECT_API USpellbookSlot : public UActionSlot
{
   GENERATED_BODY()

   void OnBtnClick() override;
   void ShowDesc(UToolTipWidget* tooltip) override;

public:

   /** Updates the level indicator text (telling us what level this spell is at)*/
   void UpdateSlotLevelText(int newLevel) const;
};
