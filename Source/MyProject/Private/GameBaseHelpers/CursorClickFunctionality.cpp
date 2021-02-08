#include "GameBaseHelpers/CursorClickFunctionality.h"

#include "BaseHero.h"
#include "BasePlayer.h"
#include "HeroAIController.h"
#include "HeroInventory.h"
#include "HUDProvider.h"
#include "ItemFunctionLibrary.h"
#include "ManualSpellComponent.h"
#include "NPC.h"
#include "RTSIngameWidget.h"
#include "RTSPawn.h"
#include "TargetedAttackComponent.h"
#include "Unit.h"
#include "UnitController.h"
#include "UserInput.h"

void CursorClickFunctionalityBase::IssueMoveToSelectedUnits(FVector moveLocation)
{
   for(AUnit* unit : controllerRef->GetBasePlayer()->GetSelectedUnits())
   {
      unit->GetUnitController()->Move(moveLocation);
      unit->GetUnitController()->StopAutomation();
   }
}

void CursorClickFunctionalityBase::IssueAttackToSelectedUnits(AUnit* attackTarget)
{
   for(AUnit* unit : controllerRef->GetBasePlayer()->GetSelectedUnits())
   {
      if(unit->GetIsEnemy() != attackTarget->GetIsEnemy())
      {
         if(UTargetedAttackComponent* attackComp = unit->GetUnitController()->FindComponentByClass<UTargetedAttackComponent>())
         {
            attackComp->BeginAttack(attackTarget);
            unit->GetUnitController()->StopAutomation();
         }
      }
   }
}

void CursorClickFunctionalityBase::IssueAttackMoveToSelectedUnits(FVector attackLocation)
{
   for(AUnit* unit : controllerRef->GetBasePlayer()->GetSelectedUnits())
   {
      unit->GetUnitController()->FindComponentByClass<UTargetedAttackComponent>()->BeginAttackMove(attackLocation);
   }
}

void CursorClickFunctionalityBase::CancelSelectedUnitsSelectedSpell()
{
   for(AUnit* unit : controllerRef->GetBasePlayer()->GetSelectedUnits())
   {
      if(UManualSpellComponent* manualSpellComp = unit->GetUnitController()->FindComponentByClass<UManualSpellComponent>())
      {
         manualSpellComp->PressedCastSpell(nullptr);
      }
   }
}

void CursorClickFunctionalityBase::SelectionRectSetup()
{
   float floatX, floatY;
   controllerRef->GetMousePosition(floatX, floatY);
   pawnRef->startMousePos = FVector2D{floatX, floatY};
}

void CursorClickFunctionalityBase::QueueActionToSelectedUnits(const TFunction<void(AUnit*)>& queuedAction)
{
   for(AUnit* unit : controllerRef->GetBasePlayer()->GetSelectedUnits())
   {
      unit->GetUnitController()->QueueAction([unit, queuedAction]() { queuedAction(unit); });
   }
}

void CursorClickFunctionalityBase::IssueInteractCommandToSelectedHeroes()
{
   if(AInteractableBase* interactableObject = Cast<AInteractableBase>(pawnRef->GetHitActorClick(clickHitResult)))
   {
      for(ABaseHero* hero : controllerRef->GetBasePlayer()->GetSelectedHeroes())
      {
         hero->GetHeroController()->BeginInteract(interactableObject);
      }
   }
}

void CursorClickFunctionalityBase::IssueTalkComandToSelectedHeroes()
{
   if(ANPC* interactableObject = Cast<ANPC>(pawnRef->GetHitActorClick(clickHitResult)))
   {
      for(ABaseHero* hero : controllerRef->GetBasePlayer()->GetSelectedHeroes())
      {
         hero->GetHeroController()->BeginInteract(interactableObject);
      }
   }
}

void CursorClickFunctionalityBase::IssueItemUseCommandToHeroWithInventory()
{
   pawnRef->GetHitResultClick(clickHitResult);
   if(clickHitResult.IsValidBlockingHit())
   {
      ABaseHero* heroUsingInventory = GetHeroUsingInventory();

      const auto heroController = heroUsingInventory->GetHeroController();
      if(heroController->GetManualSpellComponent()->OnSpellConfirmInput(clickHitResult))
      {
         heroController->StopAutomation();
         pawnRef->SetSecondaryCursor(ECursorStateEnum::Select);
      }
   }
}

ABaseHero* CursorClickFunctionalityBase::GetHeroUsingInventory() const
{
   UHeroInventory* heroInventoryRef = controllerRef->GetWidgetProvider()->GetIngameHUD()->GetInventoryHUD();

   const int hIndex = heroInventoryRef->GetHeroIndex();
   check(static_cast<unsigned>(hIndex) < static_cast<unsigned>(controllerRef->GetBasePlayer()->GetHeroes().Num()));

   return controllerRef->GetBasePlayer()->GetHeroes()[hIndex];
}
