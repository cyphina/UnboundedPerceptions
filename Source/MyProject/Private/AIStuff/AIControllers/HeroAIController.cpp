// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "HeroAIController.h"

#include "WorldObjects/BaseHero.h"
#include "BasePlayer.h"

#include "SpellSystem/MySpell.h"

#include "Interactable.h"

#include "ItemManager.h"
#include "Items/Consumable.h"

#include "UserInput.h"
#include "RTSPawn.h"
#include <Runtime\NavigationSystem\Public\NavigationSystem.h>

#include "ManualSpellComponent.h"
#include "NoTargeting.h"
#include "RTSStateComponent.h"
#include "SpellTargetingTypes.h"
#include "TargetComponent.h"

void AHeroAIController::OnPossess(APawn* InPawn)
{
   Super::OnPossess(InPawn);
   heroRef = Cast<ABaseHero>(GetPawn());
}

void AHeroAIController::BeginInteract(AActor* interactable)
{
   if(IsValid(interactable) && interactable->GetClass()->ImplementsInterface(UInteractable::StaticClass())) {
      StopCurrentAction();

      // Make sure this is set before we call GetInteractableLocation when handling a door
      heroRef->SetCurrentInteractable(interactable);

      GetStateComponent()->ChangeState(EUnitState::STATE_INTERACTING);
      UStaticMeshComponent* meshComp = interactable->FindComponentByClass<UStaticMeshComponent>();

      // If the interactable cannot be moved, then we have to move to the interact point since the point is fixed
      if(meshComp && meshComp->Mobility != EComponentMobility::Movable) {
         const FVector interactableLoc = IInteractable::Execute_GetInteractableLocation(interactable);

         // If our target location is reachable (seems to not work until a certain point after the level is loaded)
         float pathCost = 0;
         if(UNavigationSystemV1::GetCurrent(GetWorld())->GetPathCost(GetWorld(), heroRef->GetActorLocation(), interactableLoc, pathCost) !=
            ENavigationQueryResult::Fail) {
            heroRef->GetTargetComponent()->SetTarget(interactableLoc);
            if(AdjustPosition(10, interactableLoc, [this]() { IInteractable::Execute_Interact(heroRef->currentInteractable, heroRef); })) {
               // We could add some functionality here if we are already in position...
            }

         } else {
            // If we can't reach the target location component, move towards the actor
            FNavLocation resLocation;
            if(UNavigationSystemV1::GetCurrent(GetWorld())->GetRandomReachablePointInRadius(interactableLoc, ABaseHero::INTERACT_RANGE, resLocation))
               AdjustPosition(10, resLocation, [this]() { IInteractable::Execute_Interact(heroRef->currentInteractable, heroRef); });
         }
      } else {
         // If our target actor can move, we'll starting walking towards the actor.  If it is blocked off lets hope the designer put it in a path where we can walk towards it eventually
         heroRef->GetTargetComponent()->SetTarget(interactable);
         if(AdjustPosition(heroRef->INTERACT_RANGE, interactable, [this]() { IInteractable::Execute_Interact(heroRef->currentInteractable, heroRef); })) {
            // We could add some functionality here if we are already in position...
         }
      }
   }
}

void AHeroAIController::BeginUseItem(int itemToUseID, int slotIndex)
{
   // Item usage is just a wrapper around spell casting so once we start using the item we move into position as if we're casting a spell
   heroRef->SetCurrentItem(itemToUseID, slotIndex);
   GetStateComponent()->ChangeState(EUnitState::STATE_ITEM);
   const TSubclassOf<UMySpell> itemAbility = UItemManager::Get().GetConsumableInfo(itemToUseID)->abilityClass;
   GetManualSpellComponent()->PressedCastSpell(itemAbility);

   if(itemAbility.GetDefaultObject()->GetTargeting() != TSubclassOf<UUpSpellTargeting_None>()) {
      Cast<AUserInput>(GetWorld()->GetFirstPlayerController())->GetCameraPawn()->SetSecondaryCursor(ECursorStateEnum::Item);
   }
}

void AHeroAIController::StopCurrentAction()
{
   Super::StopCurrentAction();
   const AUserInput* PC = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
   if(ABasePlayer* basePlayer = PC ? PC->GetBasePlayer() : nullptr) {
      if(basePlayer->heroInBlockingInteraction != heroRef) {
         heroRef->currentInteractable = nullptr;
         heroRef->currentItemId.Reset();
         heroRef->currentItemSlotIndex.Reset();
      }
   }
}
