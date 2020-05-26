// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "WorldObjects/BaseHero.h"
#include "Pickup.h"
#include "UserInput.h"
#include "UI/HUDManager.h"
#include "RTSGameMode.h"
#include "Quests/QuestManager.h"
#include "Items/HeroInventory.h"
#include "LevelSaveStructs.h"
#include "RTSIngameWidget.h"

APickup::APickup()
{
   // Set up the mesh for the pickup, and set the item name, help text, and item value
   interactableMesh->SetSimulatePhysics(false);
   PrimaryActorTick.bCanEverTick = false;

   interactableMesh->SetCollisionProfileName("InteractableWalkable");
}

void APickup::BeginPlay()
{
   Super::BeginPlay();
   CPCRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
   OnPickupDelegate.AddDynamic(this, &APickup::OnPickedUp);
   // create object here
}

void APickup::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);
}

void APickup::Interact_Implementation(ABaseHero* hero)
{
   // Put code here that places the item in the characters inventory
   if (CanInteract_Implementation()) {
      if (hero->backpack) {
         const int initialItemCount = item.count;
         if(!hero->backpack->AddItem(item))
            CPCRef->GetHUDManager()->GetIngameHUD()->DisplayHelpText(NSLOCTEXT("Pickup", "NoSpaceLeft", "No space in inventory to pickup everything!"));
         CPCRef->GetGameMode()->GetQuestManager()->OnItemPickup(FMyItem(item.id, initialItemCount));
         OnPickupDelegate.Broadcast();
      }
   }
}

FVector APickup::GetInteractableLocation_Implementation() const
{
   return GetActorLocation();
}

void APickup::OnPickedUp()
{
   if (item.count == 0) { Destroy(); }
   CPCRef->GetHUDManager()->GetInventoryHUD()->LoadItems();
}

void APickup::OnComponentBeginOverlap(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
}

void APickup::SaveInteractable(FMapSaveInfo& mapData)
{
   mapData.pickupList.Add(GetName());
}

void APickup::LoadInteractable(FMapSaveInfo& mapData)
{
   // If we have already been picked up in our save, delete this
   if (!mapData.pickupList.Find(GetName())) {
      GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Pickup named %s was found"), *GetName()));
      Destroy();
   } else {
      GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Pickup named %s not found"), *GetName()));
   }
}
