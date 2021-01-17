#include "ESkillContainer.h"
#include "ActionbarInterface.h"
#include "BasePlayer.h"
#include "PartyDelegateContext.h"
#include "Unit.h"
#include "UserInput.h"
#include "Primitives/RTSWidgetSwitcher.h"
#include "Actionbar_MultiUnitView.h"
#include "Actionbar_SingleUnitView.h"

void UActionbarInterface::NativeOnInitialized()
{
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateContext>()->OnFocusedUnitChanged().AddUObject(this,
      &UActionbarInterface::OnFocusedUnitChanged);
}

void UActionbarInterface::OnAllySelected()
{
   if(CPC->GetBasePlayer()->selectedAllies.Num() > 1)
   {
      WS_UnitTypeView->SetActiveWidget(multiUnitView);
   }
}

void UActionbarInterface::OnFocusedUnitChanged(AUnit* newFocusedUnit)
{
   if(newFocusedUnit)
   {
      WS_UnitTypeView->SetActiveWidget(singleUnitView);
   }
}

void UActionbarInterface::OnAllyDeselected()
{
   if(CPC->GetBasePlayer()->selectedAllies.Num() == 1)
   {
      WS_UnitTypeView->SetActiveWidget(singleUnitView);
   }
}

void UActionbarInterface::OnGroundSelected()
{
   SetVisibility(ESlateVisibility::Collapsed);
}
