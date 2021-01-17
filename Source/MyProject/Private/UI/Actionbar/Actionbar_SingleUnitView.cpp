#include "MyProject.h"
#include "Actionbar_SingleUnitView.h"
#include "Actionbar_AllyInterface.h"
#include "Actionbar_DeadInterface.h"
#include "Actionbar_EnemyInterface.h"
#include "Actionbar_FocusedUnitPortrait.h"
#include "BasePlayer.h"
#include "GameplayDelegateContext.h"
#include "PartyDelegateContext.h"
#include "Primitives/RTSWidgetSwitcher.h"
#include "Unit.h"
#include "UserInput.h"

void UActionbar_SingleUnitView::NativeOnInitialized()
{
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateContext>()->OnUnitSlotSelected().AddUObject(this, &UActionbar_SingleUnitView::OnUnitSlotSelected);
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UGameplayDelegateContext>()->OnUnitDieGlobal().AddUObject(this, &UActionbar_SingleUnitView::OnUnitDie);
}

void UActionbar_SingleUnitView::OnWidgetShown(AUnit* focusedUnit)
{
   if(focusedUnit->GetIsEnemy())
   {
      WS_UnitTypeView->SetActiveWidget(enemyInterface);
   }
   else
   {
      WS_UnitTypeView->SetActiveWidget(allyInterface);
   }
   
   Cast<UActionbar_UnitInterface>(WS_UnitTypeView->GetActiveWidget())->OnWidgetShown(focusedUnit);
   focusedUnitPortrait->OnWidgetShown(focusedUnit);
}

void UActionbar_SingleUnitView::OnUnitDie(AUnit* deadUnit)
{
   if(deadUnit == GetOwningPlayer<AUserInput>()->GetBasePlayer()->GetFocusedUnit())
   {
      WS_UnitTypeView->SetActiveWidget(deadInterface);
   }
}

void UActionbar_SingleUnitView::OnUnitSlotSelected(AUnit* selectedUnit)
{
   if(!IsValid(selectedUnit))
   {
      WS_UnitTypeView->SetActiveWidget(deadInterface);
   }
}
