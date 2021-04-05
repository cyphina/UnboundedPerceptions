// Created 1/15/21 5:15 PM

#pragma once
#include "Actionbar_UnitInterface.h"
#include "Actionbar_AllyInterface.generated.h"

class AAlly;
class AUnit;
class UButton;

/** Showcases information about an ally including its hitpoints, mana, special skills, etc.
 * TODO: Setup Context for AIMode communication with gameplay systems
 * TODO: Create a button group widget for this kinda shit
 */

UCLASS()
class MYPROJECT_API UActionbar_AllyInterface : public UActionbar_UnitInterface
{
   GENERATED_BODY()

public:
   void OnWidgetShown(AUnit* focusedUnit) override;

protected:
   void NativeOnInitialized() override;

   UPROPERTY(Meta=(BindWidget))
   UButton* Btn_Passive;

   UPROPERTY(Meta=(BindWidget))
   UButton* Btn_Defensive;

   UPROPERTY(Meta=(BindWidget))
   UButton* Btn_Neutral;

   UPROPERTY(Meta=(BindWidget))
   UButton* Btn_Offensive;

   UPROPERTY(Meta=(BindWidget))
   UButton* Btn_Aggressive;

private:
   void SelectNewAIMode(int modeIndex);

   UFUNCTION()
   void SelectPassiveMode();
   
   UFUNCTION()
   void SelectDefensiveMode();
   
   UFUNCTION()
   void SelectNeutralMode();
   
   UFUNCTION()
   void SelectOffensiveMode();
   
   UFUNCTION()
   void SelectAggressiveMode();

   TStaticArray<UButton*, 5> AIOptionButtons;

   int currentlySelectedButtonIndex = 2;

   UPROPERTY(BlueprintReadOnly, Meta=(AllowPrivateAccess))
   AAlly* focusedAlly;
};
