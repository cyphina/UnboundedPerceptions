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
      case ECursorStateEnum::Attack: SelectEnemy();
         break;
      case ECursorStateEnum::Select:
      case ECursorStateEnum::Moving:
      case ECursorStateEnum::PanUp:
      case ECursorStateEnum::PanDown:
      case ECursorStateEnum::PanLeft:
      case ECursorStateEnum::PanRight: SelectSingleUnitUnderClick();
         break;
      case ECursorStateEnum::Interact: ClickInteract();
         break;
      case ECursorStateEnum::Item: ClickUseItem();
         break;
      case ECursorStateEnum::Magic: ClickCastSpell();
         break;
      case ECursorStateEnum::Talking: ClickTalk();
         break;
      case ECursorStateEnum::AttackMove: ClickAttackMove();
         break;
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
         // Edge case where we select our unit using a selection rect but don't hover outside it meaning the cursor won't change
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
      pawnRef->SetSecondaryCursor();
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
               pawnRef->HaltSelectedExceptMovement();

               if(!pawnRef->GetStaticFormationEnabled())
               {
                  IssueMoveToSelectedUnits(location);
               }
               else
               {
                  MoveInFormation(location);
               }
            }
            break;
            case ENEMY_CHANNEL:
            {
               if(AEnemy* enemy = Cast<AEnemy>(clickHitResult.GetActor()))
               {
                  pawnRef->HaltSelectedExceptMovement();
                  if(!enemy->GetIsUnitHidden())
                  {
                     IssueAttackToSelectedUnits(enemy);
                  } else
                  {
                     const FVector location = enemy->GetActorLocation();
                     pawnRef->CreateClickVisual(location);
                     IssueMoveToSelectedUnits(location);
                  }
               }
            }
            break;
            default: break;
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
      case ECursorStateEnum::PanRight: ToggleSingleAllySelection();
         break;
      case ECursorStateEnum::AttackMove: AttackMoveQueue();
         break;
      default: break;
   }
   pawnRef->clickedOnBrowserHud = false;
}

void DefaultCursorClickFunctionality::HandleShiftRightClick()
{
   if(GetCursorState() == ECursorStateEnum::Magic)
   {
      CancelSelectedUnitsSelectedSpell();
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

            QueueActionToSelectedUnits([location](AUnit* unit) { unit->GetUnitController()->Move(location); });
         }
         break;
         case ENEMY_CHANNEL:
         {
            AEnemy* enemy = Cast<AEnemy>(clickHitResult.GetActor());

            QueueActionToSelectedUnits([enemy](AUnit* unit)
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
            } else
            {
               allyRef->SetUnitSelected(true);
            }
         }
      }
   }
}

void DefaultCursorClickFunctionality::AttackMoveQueue()
{
   pawnRef->GetHitResultClick(clickHitResult);
   if(clickHitResult.IsValidBlockingHit())
   {
      const FVector                                    moveLocation = clickHitResult.Location;
      QueueActionToSelectedUnits([moveLocation](AUnit* unit)
      {
         if(UTargetedAttackComponent* targetedAttackComp = unit->GetUnitController()->FindComponentByClass<UTargetedAttackComponent>())
         {
            targetedAttackComp->BeginAttackMove(moveLocation);
         }
      });

      pawnRef->CreateClickVisual(moveLocation);
      pawnRef->SetSecondaryCursor();
   }
}

void DefaultCursorClickFunctionality::SpellCastQueue()
{
   pawnRef->GetHitResultClick(clickHitResult);
   if(clickHitResult.IsValidBlockingHit())
   {
      const FVector                            moveLocation = clickHitResult.Location;
      QueueActionToSelectedUnits([this](AUnit* unit)
      {
         if(UManualSpellComponent* manSpellComp = unit->GetUnitController()->FindComponentByClass<UManualSpellComponent>())
         {
            manSpellComp->OnSpellConfirmInput(clickHitResult);
         }
      });
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

      const auto itemAbility    = UItemFunctionLibrary::GetConsumableInfo(heroUsingInventory->GetCurrentItem().GetValue()).abilityClass;
      const auto heroController = heroUsingInventory->GetHeroController();

      heroController->QueueAction([heroController, this]()
      {
         if(heroController->GetManualSpellComponent()->OnSpellConfirmInput(clickHitResult))
         {
            heroController->StopAutomation();
            pawnRef->SetSecondaryCursor(ECursorStateEnum::Select);
         }
      });
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
               controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnEnemySelectedWithouDebugging().Execute(selectedUnit);
            }
         }
         selectedUnit->SetUnitSelected(true);
      } else
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
               controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnEnemySelectedWithouDebugging().Execute(selectedUnit);
            } else
            {
               selectedUnit->SetUnitSelected(true);
            }
         } else
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
      TArray<AUnit*> validSpellCastingAllies = controllerRef->GetBasePlayer()->GetSelectedUnits().FilterByPredicate([this](const AUnit* const unit)
      {
         if(UManualSpellComponent* manSpellCastComp = unit->GetUnitController()->FindComponentByClass<UManualSpellComponent>())
         {
            return CheckAllyWantToCast(manSpellCastComp->GetSpellCastComp()) && AttemptAllyCastOnTarget(manSpellCastComp);
         }
         return false;
      });
      Algo::ForEach(validSpellCastingAllies, [](const AUnit* spellCastingAlly) { spellCastingAlly->GetUnitController()->StopAutomation(); });
   }
}

bool DefaultCursorClickFunctionality::CheckAllyWantToCast(USpellCastComponent* spellCastComp)
{
   return !(spellCastComp->GetCurrentChannelingTime() > 0 || spellCastComp->GetCurrentIncantationTime() > 0);
}

bool DefaultCursorClickFunctionality::AttemptAllyCastOnTarget(UManualSpellComponent* manSpellCastComp)
{
   return manSpellCastComp->OnSpellConfirmInput(clickHitResult);
}

void DefaultCursorClickFunctionality::ClickAttackMove()
{
   pawnRef->GetHitResultClick(clickHitResult);
   if(clickHitResult.IsValidBlockingHit())
   {
      const FVector moveLocation = clickHitResult.Location;

      IssueAttackMoveToSelectedUnits(moveLocation);

      pawnRef->CreateClickVisual(moveLocation);
      pawnRef->SetSecondaryCursor(ECursorStateEnum::Select);
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
