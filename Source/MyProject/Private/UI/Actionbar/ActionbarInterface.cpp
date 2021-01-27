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
      partyContext->OnAllySelectedDelegate.AddDynamic(this, &UActionbarInterface::OnAllySelected);
      partyContext->OnAllyDeselectedDelegate.AddDynamic(this, &UActionbarInterface::OnAllyDeselected);
      partyContext->OnGroundSelectedDelegate.AddDynamic(this, &UActionbarInterface::OnGroundSelected);
   }
}

FOnSlotSelected& UActionbarInterface::OnSlotSelected()
{
   return singleUnitView->OnSlotSelected();
}

void UActionbarInterface::OnAllySelected(bool bToggled)
{
   if(CPC->GetBasePlayer()->selectedAllies.Num() > 1)
   {
      WS_UnitTypeView->SetActiveWidget(multiUnitView);
      multiUnitView->OnWidgetShown();
   }
   else
   {
      WS_UnitTypeView->SetActiveWidget(singleUnitView);
      singleUnitView->OnWidgetShown(CPC->GetBasePlayer()->selectedAllies[0]);
   }
   SetVisibility(ESlateVisibility::SelfHitTestInvisible);
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

void UActionbarInterface::OnAllyDeselected(AAlly* deselectedAlly)
{
   if(CPC->GetBasePlayer()->selectedAllies.Num() == 1)
   {
      WS_UnitTypeView->SetActiveWidget(singleUnitView);
      singleUnitView->OnWidgetShown(CPC->GetBasePlayer()->selectedAllies[0]);
      SetVisibility(ESlateVisibility::SelfHitTestInvisible);
   }
   else if(CPC->GetBasePlayer()->selectedAllies.Num() == 0)
   {
      SetVisibility(ESlateVisibility::Collapsed);
   }
}

void UActionbarInterface::OnGroundSelected()
{
   SetVisibility(ESlateVisibility::Collapsed);
}
