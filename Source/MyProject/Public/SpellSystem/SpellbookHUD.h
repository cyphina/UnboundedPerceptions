#pragma once

#include "CoreMinimal.h"

#include "MySpell.h"
#include "SlotContainer.h"
#include "SpellbookHUD.generated.h"

class ABaseHero;
class UButton;
class UBorder;
class UTextBlock;
class USpellbookSlot;

/**
 * HUD for leveling up and setting spells
 */
UCLASS()
class MYPROJECT_API USpellbookHUD : public USlotContainer
{
   GENERATED_BODY()

public:
   ABaseHero* GetHeroRef() const { return heroWithOpenSpellbookRef; }
   
   UPROPERTY()
   class AHUDManager* hudManagerRef;

   UPROPERTY(BlueprintReadOnly, Meta=(AllowPrivateAccess=true))
   ABaseHero* heroWithOpenSpellbookRef;
   
   bool bLevelingUp = false;

   /** When we click on the button to level up our skills, the background changes to let the user know that clicking on their skills will add a point to it*/
   UFUNCTION()
   void ChangeBackgroundColorWhenLeveling();

protected:
   FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
   void NativeOnInitialized() override;
   bool OnWidgetAddToViewport_Implementation() override;
   
   UPROPERTY(BlueprintReadWrite, Category = "SpellbookData")
   TArray<USpellbookSlot*> spellbookSlots;

   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action", Meta = (BindWidget))
   UButton* btnSpendSkillPoints;

   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action", Meta = (BindWidget))
   UButton* btnClose;

   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action", Meta = (BindWidget))
   UBorder* hudBackground;

   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action", Meta = (BindWidget))
   UTextBlock* txtPoints;

private:
   UFUNCTION()
   void CloseWidget();

   /** Updates colors of all the skill slots. Used when reopening this HUD or after we learn a new spell since we may unlock others in the chain*/
   void ResetHUDForNewHero(); 
   void UpdateSpellSlotImageAndLevelText();
   void ColorLearnedSpellIndices();
   void ColorLearnableSpellSlots();
   void ColorUnknownSpellSlots();

   void OnSpellLearned(TSubclassOf<UMySpell> spellClass);
   void OnSpellUpgraded(TSubclassOf<UMySpell> spellClass);

   const FLinearColor canLearnSpellColor    = FLinearColor(0.62, 0.61, 0, 1.0);
   const FLinearColor tooHighLevelSpellColor = FLinearColor(0.6, 0, 0.02, 1.0);
};
