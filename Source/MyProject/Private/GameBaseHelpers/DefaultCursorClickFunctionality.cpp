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
<<<<<<< HEAD
=======
#include "PartyDelegateContext.h"
#include "RTSGlobalCVars.h"
>>>>>>> componentrefactor
#include "RTSIngameWidget.h"
#include "RTSPawn.h"
#include "TargetedAttackComponent.h"
#include "UnitController.h"
#include "UserInput.h"
#include "SpellSystem/MySpell.h"
#include "GameBaseHelpers/ECursorStates.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
<<<<<<< HEAD

UDefaultCursorClickFunctionality::UDefaultCursorClickFunctionality(ARTSPawn* pawnRef, AUserInput* controllerRef) :
   pawnRef{pawnRef}, controllerRef{controllerRef} {}

void UDefaultCursorClickFunctionality::HandleLeftClick()
{
   // Set start click for selection rectangle
   float floatX, floatY;
   controllerRef->GetMousePosition(floatX, floatY);
   pawnRef->startMousePos = FVector2D{floatX, floatY};

   switch(pawnRef->GetCursorState()) {
      case ECursorStateEnum::Attack: SelectEnemy();
         break;
=======
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
>>>>>>> componentrefactor
      case ECursorStateEnum::Select:
      case ECursorStateEnum::Moving:
      case ECursorStateEnum::PanUp:
      case ECursorStateEnum::PanDown:
      case ECursorStateEnum::PanLeft:
<<<<<<< HEAD
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
=======
      case ECursorStateEnum::PanRight: SelectSingleUnitUnderClick(); break;
      case ECursorStateEnum::Interact: ClickInteract(); break;
      case ECursorStateEnum::Item: ClickUseItem(); break;
      case ECursorStateEnum::Magic: ClickCastSpell(); break;
      case ECursorStateEnum::Talking: ClickTalk(); break;
      case ECursorStateEnum::AttackMove: ClickAttackMove(); break;
>>>>>>> componentrefactor
      default: break;
   }

   pawnRef->clickedOnBrowserHud = false;
<<<<<<< HEAD

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

void UDefaultCursorClickFunctionality::HandleLeftClickRelease()
{
   if(controllerRef->GetBasePlayer()->selectedAllies.Num() > 0) { controllerRef->GetBasePlayer()->SetFocusedUnit(controllerRef->GetBasePlayer()->selectedAllies[0]); }
}

void UDefaultCursorClickFunctionality::HandleRightClick()
{
   if(GetCursorState() == ECursorStateEnum::Magic) {
      pawnRef->StopSelectedAllyCommands();
      return;
   }

   if(GetCursorState() != ECursorStateEnum::UI) {
      pawnRef->GetHitResultRightClick(clickHitResult);
      if(clickHitResult.bBlockingHit) {
         switch(clickHitResult.GetComponent()->GetCollisionObjectType()) {
            case ECC_WorldStatic:
               {
                  // Last time when I didn't make this temporary, we tried making our lambda automatically capture location and it failed miserably
                  const FVector location = clickHitResult.Location;

                  // Create a little decal where we clicked
                  pawnRef->CreateClickVisual(location);

                  // Stop the unit and move
                  pawnRef->StopSelectedAllyCommands();
                  for(AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies) {
                     ally->GetUnitController()->Move(FVector(location));
                     ally->GetUnitController()->StopAutomation();
                  }
               }
               break;
            case ENEMY_CHANNEL:
               {
                  AEnemy* enemy = Cast<AEnemy>(clickHitResult.GetActor());

                  pawnRef->StopSelectedAllyCommands();
                  for(AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies) {
                     Cast<AAllyAIController>(ally->GetUnitController())->GetTargetedAttackComponent()->BeginAttack(enemy);
                     ally->GetUnitController()->StopAutomation();
                  }
               }
               break;
=======
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
               pawnRef->CancelSelectedUnitsActionBeforePlayerCommand();

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
            case ENEMY_OBJECT_CHANNEL:
            case ALLY_OBJECT_CHANNEL:
            {
               if(AUnit* unit = Cast<AUnit>(clickHitResult.GetActor()))
               {
                  pawnRef->CancelSelectedUnitsActionBeforePlayerCommand();
                  if(!unit->GetIsUnitHidden())
                  {
                     IssueAttackToSelectedUnits(unit);
                  }
                  else
                  {
                     const FVector location = unit->GetActorLocation();
                     pawnRef->CreateClickVisual(location);
                     IssueMoveToSelectedUnits(location);
                  }
               }
            }
            break;
>>>>>>> componentrefactor
            default: break;
         }
      }
   }
}

<<<<<<< HEAD
void UDefaultCursorClickFunctionality::HandleShiftLeftClick()
{
   // Set start click for selection rectangle
   float floatX, floatY;
   controllerRef->GetMousePosition(floatX, floatY);
   pawnRef->startMousePos = FVector2D{floatX, floatY};

   //TODO: Handle queueing of other actions
   switch(pawnRef->GetCursorState()) {
=======
void DefaultCursorClickFunctionality::HandleShiftLeftClick()
{
   SelectionRectSetup();
   //TODO: Handle queueing of other actions
   switch(pawnRef->GetCursorState())
   {
>>>>>>> componentrefactor
      case ECursorStateEnum::Select:
      case ECursorStateEnum::Moving:
      case ECursorStateEnum::Attack:
      case ECursorStateEnum::PanUp:
      case ECursorStateEnum::PanDown:
      case ECursorStateEnum::PanLeft:
<<<<<<< HEAD
      case ECursorStateEnum::PanRight: ToggleSingleAllySelection();
         break;
      case ECursorStateEnum::AttackMove: AttackMoveQueue();
         break;
=======
      case ECursorStateEnum::PanRight: ToggleSingleAllySelection(); break;
      case ECursorStateEnum::AttackMove: AttackMoveQueue(); break;
      case ECursorStateEnum::Magic: SpellCastQueue(); break;
      case ECursorStateEnum::Item: ItemUsageQueue(); break;
>>>>>>> componentrefactor
      default: break;
   }
   pawnRef->clickedOnBrowserHud = false;
}

<<<<<<< HEAD
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
         case ECC_WorldStatic:
            {
               // Last time when I didn't make this temporary, we tried making our lambda automatically capture location and it failed miserably
               FVector location = clickHitResult.Location;

               // Create a little decal where we clicked
               pawnRef->CreateClickVisual(location);

               GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, clickHitResult.Location.ToString());
               for(AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies) {
                  ally->QueueAction(TFunction<void()>([ally, location]() { ally->GetUnitController()->Move(location); }));
               }
            }
            break;
         case ENEMY_CHANNEL:
            {
               AEnemy* enemy = Cast<AEnemy>(clickHitResult.GetActor());
               if(controllerRef->IsInputKeyDown(EKeys::LeftShift)) {
                  for(AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies) {
                     ally->QueueAction(TFunction<void()>([ally, enemy]() {
                        Cast<AAllyAIController>(ally->GetUnitController())->GetTargetedAttackComponent()->BeginAttack(enemy);
                     }));
                  }
               }
            }
            break;
=======
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

            QueueActionToSelectedUnits([location](AUnit* unit) { unit->GetUnitController()->Move(location); });
         }
         break;
         case ENEMY_OBJECT_CHANNEL:
         {
            AEnemy* enemy = Cast<AEnemy>(clickHitResult.GetActor());

            QueueActionToSelectedUnits([enemy](AUnit* unit) {
               if(UTargetedAttackComponent* targetedAttackComp = unit->GetUnitController()->FindComponentByClass<UTargetedAttackComponent>())
               {
                  targetedAttackComp->BeginAttack(enemy);
               }
            });
         }
         break;
>>>>>>> componentrefactor
         default: break;
      }
   }
}

<<<<<<< HEAD
ECursorStateEnum UDefaultCursorClickFunctionality::GetCursorState() const
=======
ECursorStateEnum DefaultCursorClickFunctionality::GetCursorState() const
>>>>>>> componentrefactor
{
   return pawnRef->GetCursorState();
}

<<<<<<< HEAD
void UDefaultCursorClickFunctionality::ToggleSingleAllySelection()
{
   AActor* hitActor = pawnRef->GetHitActorClick(clickHitResult);
   if(IsValid(hitActor)) {

      if(AAlly* allyRef = Cast<AAlly>(hitActor)) {
         if(allyRef->GetSelected()) {
            allyRef->SetSelected(false);
            pawnRef->OnAllyDeselectedDelegate.Broadcast(allyRef);

         } else {
            allyRef->SetSelected(true);
            pawnRef->OnAllySelectedDelegate.Broadcast(true);
=======
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
>>>>>>> componentrefactor
         }
      }
   }
}

<<<<<<< HEAD
void UDefaultCursorClickFunctionality::AttackMoveQueue()
{
   pawnRef->GetHitResultClick(clickHitResult);
   if(clickHitResult.IsValidBlockingHit()) {
      const FVector moveLocation = clickHitResult.Location;
      for(AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies) {
         ally->QueueAction(TFunction<void()>([ally, moveLocation]() {
            Cast<AAllyAIController>(ally->GetUnitController())->GetTargetedAttackComponent()->BeginAttackMove(moveLocation);
         }));
      }
      pawnRef->CreateClickVisual(moveLocation);
      pawnRef->SetSecondaryCursor(ECursorStateEnum::AttackMove);
   }
}

void UDefaultCursorClickFunctionality::SpellCastQueue() {}

void UDefaultCursorClickFunctionality::ItemUsageQueue() {}

void UDefaultCursorClickFunctionality::SelectSingleUnitUnderClick()
{
   if(!pawnRef->clickedOnBrowserHud) {
      // Ensure we didn't click on the browser widget in a meaningful way
      controllerRef->GetBasePlayer()->ClearSelectedAllies();
      if(AUnit* selectedUnit = Cast<AUnit>(pawnRef->GetHitActorClick(clickHitResult))) {
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
=======
void DefaultCursorClickFunctionality::AttackMoveQueue()
{
   pawnRef->GetHitResultClick(clickHitResult);
   if(clickHitResult.IsValidBlockingHit())
   {
      const FVector moveLocation = clickHitResult.Location;
      QueueActionToSelectedUnits([moveLocation](AUnit* unit) {
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
      for(AUnit* selectedUnit : controllerRef->GetBasePlayer()->GetSelectedUnits())
      {
         if(UManualSpellComponent* manSpellComp = selectedUnit->GetUnitController()->FindComponentByClass<UManualSpellComponent>())
         {
            const TSubclassOf<UMySpell> selectedSpell = manSpellComp->GetCurrentlySelectedSpell();
            if(manSpellComp->OnSpellConfirmInput(clickHitResult, selectedSpell))
            {
               selectedUnit->GetUnitController()->QueueAction([this, currentHitResult = this->clickHitResult, manSpellComp, selectedSpell]() {
                  manSpellComp->StartSpellCastAction(currentHitResult, selectedSpell);
               });
               ResetSecondaryCursorState();
            }
         }
>>>>>>> componentrefactor
      }
   }
}

<<<<<<< HEAD
void UDefaultCursorClickFunctionality::SelectEnemy()
{
   if(AEnemy* selectedEnemy = Cast<AEnemy>(pawnRef->GetHitActorClick(clickHitResult))) {
      controllerRef->GetBasePlayer()->ClearSelectedAllies();
      selectedEnemy->SetSelected(true);
      pawnRef->OnUnitSelectedDelegate.Broadcast();
   }
}

void UDefaultCursorClickFunctionality::ClickInteract()
{
   if(AActor* interactableObject = Cast<AActor>(pawnRef->GetHitActorClick(clickHitResult))) {
      for(ABaseHero* hero : controllerRef->GetBasePlayer()->selectedHeroes) {
         hero->GetHeroController()->BeginInteract(interactableObject);
      }
   }
}

void UDefaultCursorClickFunctionality::ClickTalk()
{
   if(AActor* interactableObject = Cast<AActor>(pawnRef->GetHitActorClick(clickHitResult))) {
      for(ABaseHero* hero : controllerRef->GetBasePlayer()->selectedHeroes) {
         hero->GetHeroController()->BeginInteract(interactableObject);
=======
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

      heroController->QueueAction([heroController, currentHitResult = this->clickHitResult, itemAbility]() {
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
               controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnEnemySelectedWithouDebugging().Execute(selectedUnit);
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
               controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnEnemySelectedWithouDebugging().Execute(selectedUnit);
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
>>>>>>> componentrefactor
      }
   }
}

<<<<<<< HEAD
void UDefaultCursorClickFunctionality::ClickUseItem()
{
   pawnRef->GetHitResultClick(clickHitResult);
   if(clickHitResult.IsValidBlockingHit()) {
      UHeroInventory* heroInventoryRef = controllerRef->GetWidgetProvider()->GetIngameHUD()->GetInventoryHUD();

      const int hIndex = heroInventoryRef->GetHeroIndex();
      check(static_cast<unsigned>(hIndex) < static_cast<unsigned>(controllerRef->GetBasePlayer()->GetHeroes().Num()));

      ABaseHero* heroUsingInventory = controllerRef->GetBasePlayer()->GetHeroes()[hIndex];

      const auto itemAbility    = UItemFunctionLibrary::GetConsumableInfo(heroUsingInventory->GetCurrentItem().GetValue()).abilityClass;
      const auto heroController = heroUsingInventory->GetHeroController();
      if(heroController->GetManualSpellComponent()->SetupSpellTargeting(clickHitResult)) {
         heroController->StopAutomation();
         pawnRef->SetSecondaryCursor(ECursorStateEnum::Select);
      }
   }
}

void UDefaultCursorClickFunctionality::ClickCastSpell()
{
   pawnRef->GetHitResultClick(clickHitResult);
   if(clickHitResult.IsValidBlockingHit()) {
      TArray<AAlly*> validSpellCastingAllies = GetSelectedAllies().FilterByPredicate([this](const AAlly* const ally) {
         return CheckAllyWantToCast(ally) && AttemptAllyCastOnTarget(ally);
      });
      Algo::ForEach(validSpellCastingAllies, [](const AAlly* spellCastingAlly) { spellCastingAlly->GetUnitController()->StopAutomation(); });
   }
}

bool UDefaultCursorClickFunctionality::CheckAllyWantToCast(const AAlly* ally)
{
   return ally->GetState() != EUnitState::STATE_CASTING;
}

bool UDefaultCursorClickFunctionality::AttemptAllyCastOnTarget(const AAlly* ally)
{
   AAllyAIController* allyController = Cast<AAllyAIController>(ally->GetUnitController());
   return allyController->GetManualSpellComponent()->SetupSpellTargeting(clickHitResult);
}

void UDefaultCursorClickFunctionality::ClickAttackMove()
{
   pawnRef->GetHitResultClick(clickHitResult);
   if(clickHitResult.IsValidBlockingHit()) {
      const FVector moveLocation = clickHitResult.Location;

      for(AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies) {
         ally->GetUnitController()->FindComponentByClass<UTargetedAttackComponent>()->BeginAttackMove(moveLocation);
      }
      pawnRef->CreateClickVisual(moveLocation);
      pawnRef->SetSecondaryCursor(ECursorStateEnum::Select);
   }
}

const TArray<AAlly*>& UDefaultCursorClickFunctionality::GetSelectedAllies() const
{
   return controllerRef->GetBasePlayer()->selectedAllies;
=======
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

      IssueAttackMoveToSelectedUnits(moveLocation);

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
>>>>>>> componentrefactor
}
