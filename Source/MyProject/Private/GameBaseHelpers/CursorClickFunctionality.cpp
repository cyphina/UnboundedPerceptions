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

DEFINE_LOG_CATEGORY(Up_Log_PlayerActions);

void CursorClickFunctionalityBase::IssueMoveToSelectedUnits(FVector moveLocation)
{
   for(AUnit* unit : controllerRef->GetBasePlayer()->GetSelectedUnits())
   {
      if(unit->GetUnitController()->GetState() != EUnitState::STATE_CASTING && unit->GetUnitController()->GetState() != EUnitState::STATE_CHANNELING &&
         unit->GetUnitController()->GetState() != EUnitState::STATE_INCANTATION)
      {
         unit->GetUnitController()->Move(moveLocation);
      }
   }
}

void CursorClickFunctionalityBase::IssueMoveToFocusedUnit(FVector moveLocation)
{
   if(AUnit* focusedUnit = controllerRef->GetBasePlayer()->GetFocusedUnit())
   {
      if(focusedUnit->GetUnitController()->GetState() != EUnitState::STATE_CASTING && focusedUnit->GetUnitController()->GetState() != EUnitState::STATE_CHANNELING &&
         focusedUnit->GetUnitController()->GetState() != EUnitState::STATE_INCANTATION)
      {
         focusedUnit->GetUnitController()->Move(moveLocation);
      }
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

void CursorClickFunctionalityBase::IssueAttackToFocusedUnit(AUnit* attackTarget)
{
   if(AUnit* focusedUnit = controllerRef->GetBasePlayer()->GetFocusedUnit())
   {
      if(focusedUnit->GetIsEnemy() != attackTarget->GetIsEnemy())
      {
         if(UTargetedAttackComponent* attackComp = focusedUnit->GetUnitController()->FindComponentByClass<UTargetedAttackComponent>())
         {
            attackComp->BeginAttack(attackTarget);
            focusedUnit->GetUnitController()->StopAutomation();
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
   UE_LOG(Up_Log_PlayerActions, Log, TEXT("Player issued attack move towards %s for selected units."), *attackLocation.ToString());
}

void CursorClickFunctionalityBase::IssueAttackMoveToFocusedUnit(FVector attackLocation)
{
   if(AUnit* focusedUnit = controllerRef->GetBasePlayer()->GetFocusedUnit())
   {
      focusedUnit->GetUnitController()->FindComponentByClass<UTargetedAttackComponent>()->BeginAttackMove(attackLocation);
      UE_LOG(Up_Log_PlayerActions, Log, TEXT("Played issued attack move command to unit %s with destination %s"), *focusedUnit->GetGameName().ToString(),
             *attackLocation.ToString());
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

void CursorClickFunctionalityBase::SelectionRectSetup() const
{
   float floatX, floatY;
   controllerRef->GetMousePosition(floatX, floatY);
   pawnRef->startMousePos = FVector2D{floatX, floatY};
}

void CursorClickFunctionalityBase::QueueActionToSelectedUnits(const TFunction<void(AUnit*)>& queuedAction)
{
   for(AUnit* unit : controllerRef->GetBasePlayer()->GetSelectedUnits())
   {
      unit->GetUnitController()->QueueAction(
          [unit, queuedAction]()
          {
             queuedAction(unit);
          });
   }
}

void CursorClickFunctionalityBase::IssueInteractCommandToSelectedHeroes()
{
   if(AInteractableBase* InteractableObject = Cast<AInteractableBase>(pawnRef->GetHitActorClick(clickHitResult)))
   {
      for(ABaseHero* Hero : controllerRef->GetBasePlayer()->GetSelectedHeroes())
      {
         Hero->GetHeroController()->BeginInteract(InteractableObject);
      }
      UE_LOG(Up_Log_PlayerActions, Log, TEXT("Player selected interactable %s"), *InteractableObject->GetGameName().ToString());
   }
}

void CursorClickFunctionalityBase::IssueTalkComandToSelectedHeroes()
{
   if(ANPC* NPC = Cast<ANPC>(pawnRef->GetHitActorClick(clickHitResult)))
   {
      for(ABaseHero* hero : controllerRef->GetBasePlayer()->GetSelectedHeroes())
      {
         hero->GetHeroController()->BeginInteract(NPC);
      }
      UE_LOG(Up_Log_PlayerActions, Log, TEXT("Player selected NPC %s"), *NPC->GetGameName().ToString());
   }
}

void CursorClickFunctionalityBase::IssueItemUseCommandToHeroWithInventory()
{
   pawnRef->GetHitResultClick(clickHitResult);
   if(clickHitResult.IsValidBlockingHit())
   {
      ABaseHero* HeroUsingInventory = GetHeroUsingInventory();

      const auto HeroController = HeroUsingInventory->GetHeroController();

      const TSubclassOf<UMySpell> SpellToCast = HeroController->GetManualSpellComponent()->GetCurrentlySelectedSpell();
      if(HeroController->GetManualSpellComponent()->OnSpellConfirmInput(clickHitResult, SpellToCast))
      {
         HeroController->HaltUnit();
         HeroController->GetManualSpellComponent()->StartSpellCastAction(clickHitResult, SpellToCast);
         pawnRef->SetSecondaryCursor();
         UE_LOG(Up_Log_PlayerActions, Log, TEXT("Hero %s using item %d on selected target %s"), *HeroUsingInventory->GetGameName().ToString(),
                HeroUsingInventory->GetCurrentItemId(), *clickHitResult.GetActor()->GetName());
      }
   }
}

void CursorClickFunctionalityBase::ResetSecondaryCursorState()
{
   pawnRef->SetSecondaryCursor();
   pawnRef->HideSpellCircle();
}

ABaseHero* CursorClickFunctionalityBase::GetHeroUsingInventory() const
{
   UHeroInventory* heroInventoryRef = controllerRef->GetWidgetProvider()->GetIngameHUD()->GetInventoryHUD();

   const int hIndex = heroInventoryRef->GetHeroIndex();
   check(static_cast<unsigned>(hIndex) < static_cast<unsigned>(controllerRef->GetBasePlayer()->GetHeroes().Num()));

   return controllerRef->GetBasePlayer()->GetHeroes()[hIndex];
}
