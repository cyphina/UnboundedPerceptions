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
#include "RTSIngameWidget.h"
#include "RTSPawn.h"
#include "TargetedAttackComponent.h"
#include "UnitController.h"
#include "UserInput.h"
#include "SpellSystem/MySpell.h"
#include "GameBaseHelpers/ECursorStates.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

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
            default: break;
         }
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
      case ECursorStateEnum::PanRight: ToggleSingleAllySelection();
         break;
      case ECursorStateEnum::AttackMove: AttackMoveQueue();
         break;
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
   AActor* hitActor = pawnRef->GetHitActorClick(clickHitResult);
   if(IsValid(hitActor)) {

      if(AAlly* allyRef = Cast<AAlly>(hitActor)) {
         if(allyRef->GetSelected()) {
            allyRef->SetSelected(false);
            controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnAllyDeselectedDelegate.Broadcast(allyRef);

         } else {
            allyRef->SetSelected(true);
            controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnAllySelectedDelegate.Broadcast(true);
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
   if(!pawnRef->clickedOnBrowserHud)
   {
      // Ensure we didn't click on the browser widget in a meaningful way
      controllerRef->GetBasePlayer()->ClearSelectedAllies();
      if(AUnit* selectedUnit = Cast<AUnit>(pawnRef->GetHitActorClick(clickHitResult)))
      {
         selectedUnit->SetSelected(true);
         controllerRef->GetBasePlayer()->SetFocusedUnit(selectedUnit);
         // Kind of jank but this is what I thought of at the moment so we don't have to check this everywhere we bind to this delegate
         if(selectedUnit->GetClass()->IsChildOf(AAlly::StaticClass()))
         {
            controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnAllySelectedDelegate.Broadcast(false);
         } else
         {
            controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnUnitSelectedDelegate.Broadcast();
         }
      } else
      {
         // We selected the ground
         pawnRef->ChangeCursor(ECursorStateEnum::Select);
         controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnGroundSelectedDelegate.Broadcast();
      }
   }
}

void UDefaultCursorClickFunctionality::SelectEnemy()
{
   if(AEnemy* selectedEnemy = Cast<AEnemy>(pawnRef->GetHitActorClick(clickHitResult))) {
      controllerRef->GetBasePlayer()->ClearSelectedAllies();
      selectedEnemy->SetSelected(true);
      controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnUnitSelectedDelegate.Broadcast();
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
      }
   }
}

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
}
