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

void AHeroAIController::OnPossess(APawn* InPawn)
{
   Super::OnPossess(InPawn);
   heroRef = Cast<ABaseHero>(GetPawn());
}

void AHeroAIController::BeginInteract(AActor* interactor)
{
   if(IsValid(interactor) && interactor->GetClass()->ImplementsInterface(UInteractable::StaticClass())) {
      Stop();

      // Make sure this is set before we call GetInteractableLocation when handling a door
      heroRef->currentInteractable    = interactor;
      heroRef->SetTargetActor(interactor);
      heroRef->unitProperties.turnAction.BindLambda([this]() { IInteractable::Execute_Interact(heroRef->currentInteractable, heroRef); });

      heroRef->state->ChangeState(EUnitState::STATE_INTERACTING);
      UStaticMeshComponent* meshComp = interactor->FindComponentByClass<UStaticMeshComponent>();

      // If the interactable cannot be moved, then we have to move to the interact point since the point is fixed
      if(meshComp && meshComp->Mobility != EComponentMobility::Movable) {
         const FVector interactableLoc = IInteractable::Execute_GetInteractableLocation(interactor);

         // If our target location is reachable (seems to not work until a certain point after the level is loaded)
         float pathCost = 0;
         if(UNavigationSystemV1::GetCurrent(GetWorld())->GetPathCost(GetWorld(), heroRef->GetActorLocation(), interactableLoc, pathCost) !=
            ENavigationQueryResult::Fail) {
            heroRef->SetTargetLocation(interactableLoc);
            if(AdjustPosition(10, interactableLoc))
               // If we're already here execute the turn action
               heroRef->unitProperties.turnAction.Execute();
         } else {
            // If we can't reach the target location component, move towards the actor
            FNavLocation resLocation;
            if(UNavigationSystemV1::GetCurrent(GetWorld())->GetRandomReachablePointInRadius(interactableLoc, heroRef->interactRange, resLocation))
               AdjustPosition(10, resLocation);
         }
      } else {
         // If our target actor can move, we'll starting walking towards the actor.  If it is blocked off lets hope the designer put it in a path where we can walk towards it eventually
         heroRef->SetTargetActor(interactor);
         if(AdjustPosition(heroRef->interactRange, interactor))
            heroRef->unitProperties.turnAction.Execute();
      }
   }
}

void AHeroAIController::BeginUseItem(int itemToUseID)
{
   // Item usage is just a wrapper around spell casting so once we start using the item we move into position as if we're casting a spell
   heroRef->SetCurrentItem(itemToUseID);
   heroRef->state->ChangeState(EUnitState::STATE_ITEM);
   TSubclassOf<UMySpell> itemAbility = UItemManager::Get().GetConsumableInfo(itemToUseID)->abilityClass;
   PressedCastSpell(itemAbility);

   // If this item has targetting, setup the item using cursor
   if(itemAbility.GetDefaultObject()->GetTargetting() != FGameplayTag::RequestGameplayTag("Skill.Targetting.None")) {
      heroRef->controllerRef->GetCameraPawn()->SetSecondaryCursor(ECursorStateEnum::Item);
   }
}

void AHeroAIController::Stop()
{
   Super::Stop();
   // If we are the hero not blocking interaction (we need to store this since we don't want any other heros performing blocking interactions)
   if(GetCPCRef()->GetBasePlayer()->heroInBlockingInteraction != heroRef)
      heroRef->currentInteractable = nullptr;
   // heroRef->currentItem         = 0;  It's possible we don't need to erase this on stop since it doesn't affect anything
}
