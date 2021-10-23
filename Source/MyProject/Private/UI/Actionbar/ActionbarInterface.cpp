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
#include "CheckBox.h"
#include "RTSPawn.h"
#include "UIDelegateContext.h"

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

   Chk_Lock->OnCheckStateChanged.AddDynamic(this, &UActionbarInterface::OnSelectLockToggled);
}

void UActionbarInterface::OnUnitSelected()
{
   if(CPC->GetBasePlayer()->GetSelectedUnits().Num() > 1)
   {
      WS_UnitTypeView->SetActiveWidget(multiUnitView);
      multiUnitView->OnWidgetShown();
   }
   else
   {
      WS_UnitTypeView->SetActiveWidget(singleUnitView);
      singleUnitView->OnWidgetShown(CPC->GetBasePlayer()->GetSelectedUnits()[0]);
   }
   SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}


void UActionbarInterface::OnUnitDeselected(AUnit* DeselectedUnit)
{
   if(CPC->GetBasePlayer()->GetSelectedUnits().Num() == 1)
   {
      WS_UnitTypeView->SetActiveWidget(singleUnitView);
      singleUnitView->OnWidgetShown(CPC->GetBasePlayer()->GetSelectedUnits()[0]);
      SetVisibility(ESlateVisibility::SelfHitTestInvisible);
   }
   else if(CPC->GetBasePlayer()->GetSelectedUnits().Num() == 0)
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

void UActionbarInterface::OnSelectLockToggled(bool bToggleOn)
{
   GetOwningLocalPlayer()->GetSubsystem<UUIDelegateContext>()->OnSelectionLockToggled().Broadcast();
}

FOnSlotSelected& UActionbarInterface::OnSkillSlotSelected()
{
   return singleUnitView->OnSkillSlotSelected();
}

FOnSlotSelected& UActionbarInterface::OnEffectSlotSelected()
{
   return singleUnitView->OnEffectSlotSelected();
}
