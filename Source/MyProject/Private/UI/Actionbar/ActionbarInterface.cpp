#include "ESkillContainer.h"
#include "ActionbarInterface.h"
#include "BasePlayer.h"
#include "PartyDelegateContext.h"
#include "Unit.h"
#include "UserInput.h"
#include "Primitives/RTSWidgetSwitcher.h"
#include "Actionbar_MultiUnitView.h"
#include "Actionbar_SingleUnitView.h"
#include "Ally.h"
#include "RTSPawn.h"

void UActionbarInterface::NativeOnInitialized()
{
   if(UPartyDelegateContext* partyContext = GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateContext>())
   {
      partyContext->OnFocusedUnitChanged().AddUObject(this,
         &UActionbarInterface::OnFocusedUnitChanged);
      partyContext->OnUnitSelectedDelegate.AddDynamic(this, &UActionbarInterface::OnUnitSelected);
      partyContext->OnUnitDeselectedDelegate.AddDynamic(this, &UActionbarInterface::OnUnitDeselected);
      partyContext->OnSelectionClearedDelegate.AddDynamic(this, &UActionbarInterface::OnAllAlliesCleared);
   }
}

void UActionbarInterface::OnUnitSelected()
{
   if(CPC->GetBasePlayer()->selectedUnits.Num() > 1)
   {
      WS_UnitTypeView->SetActiveWidget(multiUnitView);
      multiUnitView->OnWidgetShown();
   }
   else
   {
      WS_UnitTypeView->SetActiveWidget(singleUnitView);
      singleUnitView->OnWidgetShown(CPC->GetBasePlayer()->selectedUnits[0]);
   }
   SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}


void UActionbarInterface::OnUnitDeselected()
{
   if(CPC->GetBasePlayer()->selectedUnits.Num() == 1)
   {
      WS_UnitTypeView->SetActiveWidget(singleUnitView);
      singleUnitView->OnWidgetShown(CPC->GetBasePlayer()->selectedUnits[0]);
      SetVisibility(ESlateVisibility::SelfHitTestInvisible);
   }
   else if(CPC->GetBasePlayer()->selectedUnits.Num() == 0)
   {
      SetVisibility(ESlateVisibility::Collapsed);
   }
}

void UActionbarInterface::OnFocusedUnitChanged(AUnit* newFocusedUnit)
{
   if(newFocusedUnit)
   {
      WS_UnitTypeView->SetActiveWidget(singleUnitView);
      singleUnitView->OnWidgetShown(newFocusedUnit);
      SetVisibility(ESlateVisibility::SelfHitTestInvisible);
   }
}

void UActionbarInterface::OnAllAlliesCleared()
{
   SetVisibility(ESlateVisibility::Collapsed);
}

FOnSlotSelected& UActionbarInterface::OnSlotSelected()
{
   return singleUnitView->OnSlotSelected();
}