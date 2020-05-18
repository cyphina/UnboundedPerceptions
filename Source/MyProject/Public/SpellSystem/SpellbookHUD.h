// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyDraggableWidget.h"
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
class MYPROJECT_API USpellbookHUD : public UMyDraggableWidget
{
   GENERATED_BODY()

   UPROPERTY()
   class AHUDManager* hudManagerRef;

   UPROPERTY(BlueprintReadOnly, Meta=(AllowPrivateAccess=true))
   ABaseHero* heroRef;

   void NativeConstruct() override;

   bool OnWidgetAddToViewport_Implementation() override;

   UFUNCTION()
   void CloseWidget();

   /** When we click on the button to level up our skills, the background changes to let the user know that clicking on their skills will add a point to it*/
   UFUNCTION()
   void ChangeBackgroundColorWhenLeveling();

 public:
   ABaseHero* GetHeroRef() const { return heroRef; }
   bool       bLevelingUp = false;

   /** Updates colors of all the skill slots. Used after we learn a new spell since we may unlock others in the chain*/
   void Update();

 protected:
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
};
