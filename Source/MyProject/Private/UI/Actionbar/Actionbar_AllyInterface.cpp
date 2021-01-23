// Created 1/15/21 9:05 PM

#include "MyProject.h"
#include "Actionbar_AllyInterface.h"

#include "Ally.h"
#include "AllyAIController.h"
#include "Button.h"

void UActionbar_AllyInterface::NativeOnInitialized()
{
   AIOptionButtons[0] = Btn_Passive;
   AIOptionButtons[1] = Btn_Defensive;
   AIOptionButtons[2] = Btn_Neutral;
   AIOptionButtons[3] = Btn_Offensive;
   AIOptionButtons[4] = Btn_Aggressive;

   AIOptionButtons[0]->OnClicked.AddDynamic(this, &UActionbar_AllyInterface::SelectPassiveMode);
   AIOptionButtons[0]->OnClicked.AddDynamic(this, &UActionbar_AllyInterface::SelectDefensiveMode);
   AIOptionButtons[0]->OnClicked.AddDynamic(this, &UActionbar_AllyInterface::SelectNeutralMode);
   AIOptionButtons[0]->OnClicked.AddDynamic(this, &UActionbar_AllyInterface::SelectOffensiveMode);
   AIOptionButtons[0]->OnClicked.AddDynamic(this, &UActionbar_AllyInterface::SelectAggressiveMode);
}

void UActionbar_AllyInterface::OnWidgetShown(AUnit* focusedUnit)
{
   focusedAlly = Cast<AAlly>(focusedUnit);
   if(focusedAlly)
   {
      AIOptionButtons[currentlySelectedButtonIndex]->SetIsEnabled(true);
      AIOptionButtons[static_cast<int>(focusedAlly->GetAllyAIController()->GetAllyBehaviorMode())]->SetIsEnabled(false);
   }
}

void UActionbar_AllyInterface::SelectNewAIMode(int modeIndex)
{
   AIOptionButtons[currentlySelectedButtonIndex]->SetIsEnabled(true);
   AIOptionButtons[modeIndex]->SetIsEnabled(false);
   currentlySelectedButtonIndex = modeIndex;
   focusedAlly->allyController->SwitchAIModes(static_cast<EAllyBehavioralMode>(modeIndex));
}

void UActionbar_AllyInterface::SelectPassiveMode()
{
   SelectNewAIMode(0);
}

void UActionbar_AllyInterface::SelectDefensiveMode()
{
   SelectNewAIMode(1);
}

void UActionbar_AllyInterface::SelectNeutralMode()
{
   SelectNewAIMode(2);
}

void UActionbar_AllyInterface::SelectOffensiveMode()
{
   SelectNewAIMode(3);
}

void UActionbar_AllyInterface::SelectAggressiveMode()
{
   SelectNewAIMode(4);
}
