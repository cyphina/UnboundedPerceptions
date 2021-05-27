#include "Algo/ForEach.h"
#include "DefaultCursorClickFunctionality.h"

#include "BaseHero.h"
#include "WorldObjects/Ally.h"
#include "WorldObjects/Enemies/Enemy.h"
#include "BasePlayer.h"
#include "HeroAIController.h"
#include "HeroInventory.h"
#include "HUDManager.h"
#include "ItemFunctionLibrary.h"
#include "ManualSpellComponent.h"
#include "MyProject.h"
#include "PartyDelegateContext.h"
#include "RTSGlobalCVars.h"
#include "RTSIngameWidget.h"
#include "RTSPawn.h"
#include "TargetedAttackComponent.h"
#include "UnitController.h"
#include "UserInput.h"
#include "SpellSystem/MySpell.h"
#include "GameBaseHelpers/ECursorStates.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "SpellCastComponent.h"
#include "UpAIHelperLibrary.h"

DefaultCursorClickFunctionality::DefaultCursorClickFunctionality(ARTSPawn* pawnRef, AUserInput* controllerRef)
{
   this->pawnRef       = pawnRef;
   this->controllerRef = controllerRef;
}

void DefaultCursorClickFunctionality::HandleLeftClick()
{
   SelectionRectSetup();

   switch(pawnRef->GetCursorState())
   {
      case ECursorStateEnum::Attack: SelectEnemy(); break;
      case ECursorStateEnum::Select:
      case ECursorStateEnum::Moving:
      case ECursorStateEnum::PanUp:
      case ECursorStateEnum::PanDown:
      case ECursorStateEnum::PanLeft:
      case ECursorStateEnum::PanRight: SelectSingleUnitUnderClick(); break;
      case ECursorStateEnum::Interact: ClickInteract(); break;
      case ECursorStateEnum::Item: ClickUseItem(); break;
      case ECursorStateEnum::Magic: ClickCastSpell(); break;
      case ECursorStateEnum::Talking: ClickTalk(); break;
      case ECursorStateEnum::AttackMove: ClickAttackMove(); break;
      default: break;
   }

   pawnRef->clickedOnBrowserHud = false;
}

void DefaultCursorClickFunctionality::HandleLeftClickRelease()
{
   if(controllerRef->GetBasePlayer()->GetSelectedUnits().Num() > 0)
   {
      if(!controllerRef->GetCameraPawn()->HasSecondaryCursor())
      {
         // Edge case where we select our unit using a selection rect but don't hover outside it (our rect is really small) meaning the cursor won't change
         pawnRef->ChangeCursor(ECursorStateEnum::Moving);
      }
      if(pawnRef->IsSelectionRectActive())
      {
         pawnRef->startMousePos = FVector2D::ZeroVector;
         pawnRef->endMousePos   = FVector2D::ZeroVector;
      }
   }
}

void DefaultCursorClickFunctionality::HandleRightClick()
{
   if(GetCursorState() == ECursorStateEnum::Magic)
   {
      CancelSelectedUnitsSelectedSpell();
      ResetSecondaryCursorState();
      return;
   }

   if(GetCursorState() != ECursorStateEnum::UI)
   {
      pawnRef->GetHitResultRightClick(clickHitResult);
      if(clickHitResult.bBlockingHit)
      {
         switch(clickHitResult.GetComponent()->GetCollisionObjectType())
         {
            case ECC_WorldStatic:
            {
               const FVector location = clickHitResult.Location;
               pawnRef->CreateClickVisual(location);
               // TODO: Find a better way to check this
               pawnRef->CancelSelectedUnitsActionBeforePlayerCommand(EUnitState::STATE_MOVING,
                                                                     {EUnitState::STATE_CASTING, EUnitState::STATE_CHANNELING, EUnitState::STATE_INCANTATION});

               if(!pawnRef->GetStaticFormationEnabled())
               {
                  if(!controllerRef->IsInputKeyDown(FKey("SpaceBar")))
                  {
                     IssueMoveToSelectedUnits(location);
                  }
                  else
                  {
                     IssueMoveToFocusedUnit(location);
                  }
               }
               else
               {
                  MoveInFormation(location);
               }
            }
            break;
            case ENEMY_OBJECT_CHANNEL:
            case ALLY_OBJECT_CHANNEL:
            {
               if(AUnit* unit = Cast<AUnit>(clickHitResult.GetActor()))
               {
                  pawnRef->CancelSelectedUnitsActionBeforePlayerCommand(EUnitState::STATE_ATTACKING);
                  if(!unit->GetIsUnitHidden())
                  {
                     if(!controllerRef->IsInputKeyDown(FKey("SpaceBar")))
                     {
                        IssueAttackToSelectedUnits(unit);
                     }
                     else
                     {
                        IssueAttackToFocusedUnit(unit);
                     }
                  }
                  else
                  {
                     const FVector location = unit->GetActorLocation();
                     pawnRef->CreateClickVisual(location);
                     IssueMoveToSelectedUnits(location);
                     if(!controllerRef->IsInputKeyDown(FKey("SpaceBar")))
                     {
                        IssueMoveToSelectedUnits(location);
                     }
                     else
                     {
                        IssueMoveToFocusedUnit(location);
                     }
                  }
               }
               break;
               default: break;
            }
         }
      }
   }
}

void DefaultCursorClickFunctionality::HandleShiftLeftClick()
{
   SelectionRectSetup();
   //TODO: Handle queueing of other actions
   switch(pawnRef->GetCursorState())
   {
      case ECursorStateEnum::Select:
      case ECursorStateEnum::Moving:
      case ECursorStateEnum::Attack:
      case ECursorStateEnum::PanUp:
      case ECursorStateEnum::PanDown:
      case ECursorStateEnum::PanLeft:
      case ECursorStateEnum::PanRight: ToggleSingleAllySelection(); break;
      case ECursorStateEnum::AttackMove: AttackMoveQueue(); break;
      case ECursorStateEnum::Magic: SpellCastQueue(); break;
      case ECursorStateEnum::Item: ItemUsageQueue(); break;
      default: break;
   }
   pawnRef->clickedOnBrowserHud = false;
}

void DefaultCursorClickFunctionality::HandleShiftRightClick()
{
   if(GetCursorState() == ECursorStateEnum::Magic)
   {
      CancelSelectedUnitsSelectedSpell();
      ResetSecondaryCursorState();
      return;
   }

   if(GetCursorState() != ECursorStateEnum::UI)
   {
      pawnRef->GetHitResultClick(clickHitResult);

      switch(clickHitResult.GetComponent()->GetCollisionObjectType())
      {
         case ECC_WorldStatic:
         {
            const FVector location = clickHitResult.Location;
            pawnRef->CreateClickVisual(location);

            QueueActionToSelectedUnits(
                [location](AUnit* unit)
                {
                   unit->GetUnitController()->Move(location);
                });
         }
         break;
         case ENEMY_OBJECT_CHANNEL:
         {
            AEnemy* enemy = Cast<AEnemy>(clickHitResult.GetActor());

            QueueActionToSelectedUnits(
                [enemy](AUnit* unit)
                {
                   if(UTargetedAttackComponent* targetedAttackComp = unit->GetUnitController()->FindComponentByClass<UTargetedAttackComponent>())
                   {
                      targetedAttackComp->BeginAttack(enemy);
                   }
                });
         }
         break;
         default: break;
      }
   }
}

ECursorStateEnum DefaultCursorClickFunctionality::GetCursorState() const
{
   return pawnRef->GetCursorState();
}

void DefaultCursorClickFunctionality::ToggleSingleAllySelection()
{
   if(!pawnRef->GetIsSelectionLockActive())
   {
      AActor* hitActor = pawnRef->GetHitActorClick(clickHitResult);
      if(IsValid(hitActor))
      {
         if(AAlly* allyRef = Cast<AAlly>(hitActor))
         {
            if(allyRef->GetUnitSelected())
            {
               allyRef->SetUnitSelected(false);
            }
            else
            {
               allyRef->SetUnitSelected(true);
            }
            controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnAllySelectionToggled.Broadcast(allyRef);
         }
      }
   }
}

void DefaultCursorClickFunctionality::AttackMoveQueue()
{
   pawnRef->GetHitResultClick(clickHitResult);
   if(clickHitResult.IsValidBlockingHit())
   {
      const FVector moveLocation = clickHitResult.Location;
      QueueActionToSelectedUnits(
          [moveLocation](AUnit* unit)
          {
             if(UTargetedAttackComponent* targetedAttackComp = unit->GetUnitController()->FindComponentByClass<UTargetedAttackComponent>())
             {
                targetedAttackComp->BeginAttackMove(moveLocation);
             }
          });

      pawnRef->CreateClickVisual(moveLocation);
      ResetSecondaryCursorState();
   }
}

void DefaultCursorClickFunctionality::SpellCastQueue()
{
   pawnRef->GetHitResultClick(clickHitResult);
   if(clickHitResult.IsValidBlockingHit())
   {
      if(AUnit* focusedUnit = controllerRef->GetBasePlayer()->GetFocusedUnit())
      {
         if(UManualSpellComponent* manSpellComp = focusedUnit->GetUnitController()->FindComponentByClass<UManualSpellComponent>())
         {
            const TSubclassOf<UMySpell> selectedSpell = manSpellComp->GetCurrentlySelectedSpell();
            if(manSpellComp->OnSpellConfirmInput(clickHitResult, selectedSpell))
            {
               focusedUnit->GetUnitController()->QueueAction(
                   [this, currentHitResult = this->clickHitResult, manSpellComp, selectedSpell]()
                   {
                      manSpellComp->StartSpellCastAction(currentHitResult, selectedSpell);
                   });
               ResetSecondaryCursorState();
            }
         }
      }
      for(AUnit* selectedUnit : controllerRef->GetBasePlayer()->GetSelectedUnits())
      {
         if(UManualSpellComponent* manSpellComp = selectedUnit->GetUnitController()->FindComponentByClass<UManualSpellComponent>())
         {
            const TSubclassOf<UMySpell> selectedSpell = manSpellComp->GetCurrentlySelectedSpell();
            if(manSpellComp->OnSpellConfirmInput(clickHitResult, selectedSpell))
            {
               selectedUnit->GetUnitController()->QueueAction(
                   [this, currentHitResult = this->clickHitResult, manSpellComp, selectedSpell]()
                   {
                      manSpellComp->StartSpellCastAction(currentHitResult, selectedSpell);
                   });
               ResetSecondaryCursorState();
            }
         }
      }
   }
}

void DefaultCursorClickFunctionality::ItemUsageQueue()
{
   pawnRef->GetHitResultClick(clickHitResult);
   if(clickHitResult.IsValidBlockingHit())
   {
      UHeroInventory* heroInventoryRef = controllerRef->GetWidgetProvider()->GetIngameHUD()->GetInventoryHUD();

      const int hIndex = heroInventoryRef->GetHeroIndex();
      check(static_cast<unsigned>(hIndex) < static_cast<unsigned>(controllerRef->GetBasePlayer()->GetHeroes().Num()));

      ABaseHero* heroUsingInventory = controllerRef->GetBasePlayer()->GetHeroes()[hIndex];

      const TSubclassOf<UMySpell> itemAbility    = UItemFunctionLibrary::GetConsumableInfo(heroUsingInventory->GetCurrentItem().GetValue()).abilityClass;
      const auto                  heroController = heroUsingInventory->GetHeroController();

      heroController->QueueAction(
          [heroController, currentHitResult = this->clickHitResult, itemAbility]()
          {
             if(heroController->GetManualSpellComponent()->OnSpellConfirmInput(currentHitResult, itemAbility))
             {
                heroController->GetManualSpellComponent()->StartSpellCastAction(currentHitResult, itemAbility);
             }
          });

      ResetSecondaryCursorState();
   }
}

void DefaultCursorClickFunctionality::SelectSingleUnitUnderClick()
{
   if(!pawnRef->clickedOnBrowserHud && !pawnRef->GetIsSelectionLockActive())
   {
      controllerRef->GetBasePlayer()->ClearSelectedUnits();
      if(AUnit* selectedUnit = Cast<AUnit>(pawnRef->GetHitActorClick(clickHitResult)))
      {
         if(!GameplayModifierCVars::bEnableEnemyControl)
         {
            if(selectedUnit->GetIsEnemy())
            {
               controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnEnemySelectedWithoutDebugging().Execute(selectedUnit);
            }
         }
         selectedUnit->SetUnitSelected(true);
      }
      else
      {
         pawnRef->ChangeCursor(ECursorStateEnum::Select);
         if(!pawnRef->GetIsSelectionLockActive())
         {
            controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnSelectionClearedDelegate.Broadcast();
         }
      }
      // TODO: Handle Enemy Case
   }
}

void DefaultCursorClickFunctionality::SelectEnemy()
{
   if(!pawnRef->GetIsSelectionLockActive())
   {
      AActor* hitActor = pawnRef->GetHitActorClick(clickHitResult);
      if(AUnit* selectedUnit = Cast<AUnit>(hitActor))
      {
         controllerRef->GetBasePlayer()->ClearSelectedUnits();

         if(selectedUnit->GetIsEnemy())
         {
            if(!GameplayModifierCVars::bEnableEnemyControl)
            {
               controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnEnemySelectedWithoutDebugging().Execute(selectedUnit);
            }
            else
            {
               selectedUnit->SetUnitSelected(true);
            }
         }
         else
         {
            selectedUnit->SetUnitSelected(true);
         }
      }
   }
}

void DefaultCursorClickFunctionality::ClickInteract()
{
   IssueInteractCommandToSelectedHeroes();
}

void DefaultCursorClickFunctionality::ClickTalk()
{
   IssueTalkComandToSelectedHeroes();
}

void DefaultCursorClickFunctionality::ClickUseItem()
{
   IssueItemUseCommandToHeroWithInventory();
}

void DefaultCursorClickFunctionality::ClickCastSpell()
{
   pawnRef->GetHitResultClick(clickHitResult);
   if(clickHitResult.IsValidBlockingHit())
   {
      if(AUnit* focusedUnit = controllerRef->GetBasePlayer()->GetFocusedUnit())
      {
         if(UManualSpellComponent* manSpellCastComp = focusedUnit->GetUnitController()->FindComponentByClass<UManualSpellComponent>())
         {
            if(CheckWantToCast(manSpellCastComp->GetSpellCastComp()))
            {
               const TSubclassOf<UMySpell> currentSpell = manSpellCastComp->GetCurrentlySelectedSpell();
               if(manSpellCastComp->OnSpellConfirmInput(clickHitResult, currentSpell))
               {
                  focusedUnit->GetUnitController()->HaltUnit();
                  manSpellCastComp->StartSpellCastAction(clickHitResult, currentSpell);
               }
            }
         }
      }
      else
      {
         for(AUnit* unit : controllerRef->GetBasePlayer()->GetSelectedUnits())
         {
            if(UManualSpellComponent* manSpellCastComp = unit->GetUnitController()->FindComponentByClass<UManualSpellComponent>())
            {
               if(CheckWantToCast(manSpellCastComp->GetSpellCastComp()))
               {
                  const TSubclassOf<UMySpell> currentSpell = manSpellCastComp->GetCurrentlySelectedSpell();
                  if(manSpellCastComp->OnSpellConfirmInput(clickHitResult, currentSpell))
                  {
                     unit->GetUnitController()->HaltUnit();
                     manSpellCastComp->StartSpellCastAction(clickHitResult, currentSpell);
                  }
               }
            }
         }
      }
   }
}

bool DefaultCursorClickFunctionality::CheckWantToCast(USpellCastComponent* spellCastComp)
{
   return !(spellCastComp->GetCurrentChannelingTime() > 0 || spellCastComp->GetCurrentIncantationTime() > 0);
}

void DefaultCursorClickFunctionality::ClickAttackMove()
{
   pawnRef->GetHitResultClick(clickHitResult);
   if(clickHitResult.IsValidBlockingHit())
   {
      const FVector moveLocation = clickHitResult.Location;

      if(!controllerRef->IsInputKeyDown(FKey("SpaceBar")))
      {
         IssueAttackMoveToSelectedUnits(moveLocation);
      }
      else
      {
         IssueAttackMoveToFocusedUnit(moveLocation);
      }

      pawnRef->CreateClickVisual(moveLocation);
      ResetSecondaryCursorState();
   }
}

void DefaultCursorClickFunctionality::MoveInFormation(FVector newLocation)
{
   const TArray<AUnit*>& selectedUnits = controllerRef->GetBasePlayer()->GetSelectedUnits();
   if(AUnit* closestUnitToNewLocation = UUpAIHelperLibrary::FindClosestUnit(newLocation, TSet<AUnit*>(selectedUnits)))
   {
      for(const AUnit* selectedUnit : selectedUnits)
      {
         const FVector offset = closestUnitToNewLocation->GetActorLocation() - selectedUnit->GetActorLocation();
         selectedUnit->GetUnitController()->Move(newLocation - offset);
      }
   }
}
