#include "DefaultCursorClickFunctionality.h"

#include "BaseHero.h"
#include "WorldObjects/Ally.h"
#include "WorldObjects/Enemies/Enemy.h"
#include "BasePlayer.h"
#include "HeroAIController.h"
#include "HeroInventory.h"
#include "HUDManager.h"
#include "ItemFunctionLibrary.h"
#include "MyProject.h"
#include "RTSPawn.h"
#include "UnitController.h"
#include "UserInput.h"
#include "SpellSystem/MySpell.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UDefaultCursorClickFunctionality::UDefaultCursorClickFunctionality(ARTSPawn* pawnRef, AUserInput* controllerRef) : pawnRef{pawnRef}, controllerRef{controllerRef}
{
}

void UDefaultCursorClickFunctionality::HandleLeftClick()
{
   // Set start click for selection rectangle
   float floatX, floatY;
   controllerRef->GetMousePosition(floatX, floatY);
   pawnRef->startMousePos = FVector2D{floatX, floatY};

   switch(pawnRef->GetCursorState()) {
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

   // Code for mouse release not click
   //
   //if(controllerRef->GetBasePlayer()->selectedAllies.Num() > 0) {
   //   if(!pawnRef->hasSecondaryCursor)
   //      pawnRef->ChangeCursor(ECursorStateEnum::Moving);
   //   if(!pawnRef->isSelectionRectActive()) {
   //      pawnRef->startMousePos = FVector2D::ZeroVector;
   //      pawnRef->endMousePos = FVector2D::ZeroVector;
   //   }
   //}
}

void UDefaultCursorClickFunctionality::HandleRightClick()
{
   if(GetCursorState() == ECursorStateEnum::Magic) {
      pawnRef->StopSelectedAllyCommands();
      return;
   }

   if(GetCursorState() != ECursorStateEnum::UI) {
      pawnRef->GetHitResultClick(clickHitResult);

      switch(clickHitResult.GetComponent()->GetCollisionObjectType()) {
         case ECC_WorldStatic: {
            // Last time when I didn't make this temporary, we tried making our lambda automatically capture location and it failed miserably
            FVector location = clickHitResult.Location;

            // Create a little decal where we clicked
            pawnRef->CreateClickVisual(location);

            // Stop the unit and move
            pawnRef->StopSelectedAllyCommands();
            for(AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies) {
               ally->GetUnitController()->Move(FVector(location));
            }
         } break;
         case ENEMY_CHANNEL: {
            AEnemy* enemy = Cast<AEnemy>(clickHitResult.GetActor());

            pawnRef->StopSelectedAllyCommands();
            for(AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies) {
               ally->GetUnitController()->BeginAttack(enemy);
            }
         } break;
         default: break;
      }
   }
}

void UDefaultCursorClickFunctionality::HandleShiftLeftClick()
{
   // Set start click for selection rectangle
   float floatX, floatY;
   controllerRef->GetMousePosition(floatX, floatY);
   pawnRef->startMousePos = FVector2D{floatX, floatY};

   //TODO: Handle queueing of other actions
   switch(pawnRef->GetCursorState()) {
      case ECursorStateEnum::Select:
      case ECursorStateEnum::Moving:
      case ECursorStateEnum::Attack:
      case ECursorStateEnum::PanUp:
      case ECursorStateEnum::PanDown:
      case ECursorStateEnum::PanLeft:
      case ECursorStateEnum::PanRight: ToggleSingleAllySelection(); break;
      case ECursorStateEnum::AttackMove: AttackMoveQueue(); break;
      default: break;
   }
   pawnRef->clickedOnBrowserHud = false;
}

void UDefaultCursorClickFunctionality::HandleShiftRightClick()
{
   if(GetCursorState() == ECursorStateEnum::Magic) {
      pawnRef->StopSelectedAllyCommands();
      return;
   }

   if(GetCursorState() != ECursorStateEnum::UI) {
      // Trace ground if we're right clicking on something that's not an enemy
      pawnRef->GetHitResultClick(clickHitResult);

      switch(clickHitResult.GetComponent()->GetCollisionObjectType()) {
         case ECC_WorldStatic: {
            // Last time when I didn't make this temporary, we tried making our lambda automatically capture location and it failed miserably
            FVector location = clickHitResult.Location;

            // Create a little decal where we clicked
            pawnRef->CreateClickVisual(location);

            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, clickHitResult.Location.ToString());
            for(AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies) {
               ally->QueueAction(TFunction<void()>([ally, location]() { ally->GetUnitController()->Move(location); }));
            }
         } break;
         case ENEMY_CHANNEL: {
            AEnemy* enemy = Cast<AEnemy>(clickHitResult.GetActor());
            if(controllerRef->IsInputKeyDown(EKeys::LeftShift)) {
               for(AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies) {
                  ally->QueueAction(TFunction<void()>([ally, enemy]() { ally->GetUnitController()->BeginAttack(enemy); }));
               }
            }
         } break;
         default: break;
      }
   }
}

ECursorStateEnum UDefaultCursorClickFunctionality::GetCursorState() const
{
   return pawnRef->GetCursorState();
}

void UDefaultCursorClickFunctionality::ToggleSingleAllySelection()
{
   if(AActor* hitActor = pawnRef->GetHitActorClick(clickHitResult); IsValid(hitActor)) {
      if(AAlly* allyRef = Cast<AAlly>(hitActor); allyRef) {
         if(allyRef->GetSelected()) {
            allyRef->SetSelected(false);
            pawnRef->OnAllyDeselectedDelegate.Broadcast(allyRef);

         } else {
            allyRef->SetSelected(true);
            pawnRef->OnAllySelectedDelegate.Broadcast(true);
         }
      }
   }
}

void UDefaultCursorClickFunctionality::AttackMoveQueue()
{
   pawnRef->GetHitResultClick(clickHitResult);
   if(clickHitResult.IsValidBlockingHit()) {
      const FVector moveLocation = clickHitResult.Location;
      for(AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies) {
         ally->QueueAction(TFunction<void()>([ally, moveLocation]() { ally->GetUnitController()->AttackMove(moveLocation); }));
      }
      pawnRef->CreateClickVisual(moveLocation);
      pawnRef->SetSecondaryCursor();
   }
}

void UDefaultCursorClickFunctionality::SelectSingleUnitUnderClick()
{
   if(!pawnRef->clickedOnBrowserHud) { // Ensure we didn't click on the browser widget in a meaningful way
      controllerRef->GetBasePlayer()->ClearSelectedAllies();
      if(AUnit* selectedUnit = Cast<AUnit>(pawnRef->GetHitActorClick(clickHitResult)); selectedUnit) {
         selectedUnit->SetSelected(true);
         // Kind of jank but this is what I thought of at the moment so we don't have to check this everywhere we bind to this delegate
         if(selectedUnit->GetClass()->IsChildOf(AAlly::StaticClass()))
            pawnRef->OnAllySelectedDelegate.Broadcast(false);
         else
            pawnRef->OnUnitSelectedDelegate.Broadcast();
      } else {
         // We selected the ground
         pawnRef->ChangeCursor(ECursorStateEnum::Select);
         pawnRef->OnGroundSelectedDelegate.Broadcast();
      }
   }
}

void UDefaultCursorClickFunctionality::SelectEnemy()
{
   if(AEnemy* selectedEnemy = Cast<AEnemy>(pawnRef->GetHitActorClick(clickHitResult)); selectedEnemy) {
      controllerRef->GetBasePlayer()->ClearSelectedAllies();
      selectedEnemy->SetSelected(true);
      pawnRef->OnUnitSelectedDelegate.Broadcast();
   }
}

void UDefaultCursorClickFunctionality::ClickInteract()
{
   if(AActor* interactableObject = Cast<AActor>(pawnRef->GetHitActorClick(clickHitResult)); interactableObject) {
      for(ABaseHero* hero : controllerRef->GetBasePlayer()->selectedHeroes) {
         hero->GetHeroController()->BeginInteract(interactableObject);
      }
   }
}

void UDefaultCursorClickFunctionality::ClickTalk()
{
   if(AActor* interactableObject = Cast<AActor>(pawnRef->GetHitActorClick(clickHitResult)); interactableObject) {
      for(ABaseHero* hero : controllerRef->GetBasePlayer()->selectedHeroes) {
         hero->GetHeroController()->BeginInteract(interactableObject);
      }
   }
}

void UDefaultCursorClickFunctionality::ClickUseItem()
{
   pawnRef->GetHitResultClick(clickHitResult);
   if(clickHitResult.IsValidBlockingHit()) {
      int hIndex = controllerRef->GetHUDManager()->GetInventoryHUD()->hIndex;
      check((unsigned)hIndex < (unsigned)controllerRef->GetBasePlayer()->heroes.Num());

      // Get the hero using this item by looking at the inventory index (there's an inventory for each hero)
      ABaseHero* heroUsingInventory = controllerRef->GetBasePlayer()->heroes[controllerRef->GetHUDManager()->GetInventoryHUD()->hIndex];

      // Lookup this item's ability
      auto       itemAbility    = UItemFunctionLibrary::GetConsumableInfo(heroUsingInventory->GetCurrentItem()).abilityClass;
      const auto heroController = heroUsingInventory->GetHeroController();
      if(heroController->SetupSpellTargetting(clickHitResult, itemAbility)) {
         heroController->StopAutomation();
         pawnRef->SetSecondaryCursor();
      }
   }
}

void UDefaultCursorClickFunctionality::ClickCastSpell()
{
   pawnRef->GetHitResultClick(clickHitResult);
   if(clickHitResult.IsValidBlockingHit()) {
      for(AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies) {
         const auto allyController = ally->GetAllyAIController();
         // If this ally has a spell selected, and we do not want the spell to go off if the ally is casting another spell
         if(IsValid(allyController->GetCurrentlySelectedSpell()) && ally->GetState() != EUnitState::STATE_CASTING) {
            if(allyController->SetupSpellTargetting(clickHitResult, allyController->GetCurrentlySelectedSpell())) {
               // Stop AI if we successfully cast the spell and return to manual control
               allyController->StopAutomation();
            }
         }
      }
   }
}

void UDefaultCursorClickFunctionality::ClickAttackMove()
{
   pawnRef->GetHitResultClick(clickHitResult);
   if(clickHitResult.IsValidBlockingHit()) {
      const FVector moveLocation = clickHitResult.Location;

      for(AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies) {
         ally->GetUnitController()->AttackMove(moveLocation);
      }
      pawnRef->CreateClickVisual(moveLocation);
      pawnRef->SetSecondaryCursor();
   }
}
