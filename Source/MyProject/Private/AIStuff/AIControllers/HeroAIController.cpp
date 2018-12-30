// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "HeroAIController.h"

#include "WorldObjects/BaseHero.h"

#include "SpellSystem/MySpell.h"

#include "Interactable.h"

#include "ItemManager.h"
#include "Items/Consumable.h"

#include "UserInput.h"
#include "RTSPawn.h"

void AHeroAIController::Possess(APawn* InPawn)
{
   Super::Possess(InPawn);
   heroRef = Cast<ABaseHero>(GetPawn());
}

void AHeroAIController::BeginInteract(AActor* interactor)
{
   if (interactor->GetClass()->ImplementsInterface(UInteractable::StaticClass())) {
      Stop();
      heroRef->currentInteractable           = interactor;
      heroRef->targetData.targetActor        = interactor;
      heroRef->targetData.targetLocation     = interactor->GetActorLocation();
      heroRef->unitProperties.turnAction.BindLambda([this](){IInteractable::Execute_Interact(heroRef->currentInteractable, heroRef);});

      heroRef->state->ChangeState(EUnitState::STATE_INTERACTING);
      UStaticMeshComponent* meshComp = interactor->FindComponentByClass<UStaticMeshComponent>();

      //If the interactable can be moved, then we have to move to the interactable rather than its interact point since the point is fixed
      if (meshComp && meshComp->Mobility != EComponentMobility::Movable)
         AdjustPosition(heroRef->interactRange, IInteractable::Execute_GetInteractableLocation(interactor, heroRef));
      else
         AdjustPosition(heroRef->interactRange, interactor);
   }
}

void AHeroAIController::BeginUseItem(int itemToUseID)
{
   heroRef->SetCurrentItem(itemToUseID);
   heroRef->state->ChangeState(EUnitState::STATE_ITEM);
   TSubclassOf<UMySpell> itemAbility = UItemManager::Get().GetConsumableInfo(itemToUseID)->abilityClass;
   PressedCastSpell(itemAbility);

   if (itemAbility.GetDefaultObject()->GetTargetting() != FGameplayTag::RequestGameplayTag("Skill.Targetting.None")) {
      heroRef->controllerRef->GetCameraPawn()->SetSecondaryCursor(ECursorStateEnum::Item);
   }
}

void AHeroAIController::Stop()
{
   Super::Stop();
   heroRef->currentInteractable = nullptr;
   //heroRef->currentItem         = 0;  It's possible we don't need to erase this on stop since it doesn't affect anything
}
