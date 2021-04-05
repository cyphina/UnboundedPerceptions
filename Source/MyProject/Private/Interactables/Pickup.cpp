// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "WorldObjects/BaseHero.h"
#include "Pickup.h"
#include "UserInput.h"
#include "UI/HUDManager.h"
#include "Quests/QuestManager.h"
#include "Items/HeroInventory.h"
#include "LevelSaveStructs.h"

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
}

void APickup::Interact_Implementation(ABaseHero* hero)
{
<<<<<<< HEAD
   if(CanInteract_Implementation()) {
=======
   if(CanInteract_Implementation())
   {
>>>>>>> componentrefactor
      if(hero->OnPickupItem().Execute(item)) { Destroy(); }
   }
}

FVector APickup::GetInteractableLocation_Implementation() const
{
   return GetActorLocation();
}

void APickup::OnComponentBeginOverlap
(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep,
 const FHitResult&    sweepResult)
{
   // TODO: Set this object as clickable when we are in range so the player can't hover over it in FOW.
}

void APickup::SaveInteractable(FMapSaveInfo& mapData)
{
   mapData.pickupList.Add(GetName());
}

void APickup::LoadInteractable(FMapSaveInfo& mapData)
{
   // If we have already been picked up in our save, delete this
<<<<<<< HEAD
   if(!mapData.pickupList.Find(GetName())) {
=======
   if(!mapData.pickupList.Find(GetName()))
   {
>>>>>>> componentrefactor
      GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Pickup named %s was found"), *GetName()));
      Destroy();
   } else
   {
      GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Pickup named %s not found"), *GetName()));
   }
}
