// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "StorageContainer.h"
#include "UserInput.h"
#include "BasePlayer.h"

#include "Items/Backpack.h"
#include "UI/HUDManager.h"

#include "BasePlayer.h"
#include "Inventory.h"
#include "WorldObjects/BaseHero.h"

#include "LevelSaveStructs.h"

AHUDManager* AStorageContainer::hudManagerRef = nullptr;

AStorageContainer::AStorageContainer() : AInteractableBase()
{
   sphereCollision = CreateDefaultSubobject<USphereComponent>(FName("InteractRange"));

   sphereCollision->SetupAttachment(sceneLocation);
   sphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
   sphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
   sphereCollision->SetCollisionResponseToChannel(FRIENDLY_CHANNEL, ECR_Overlap); 
   sphereCollision->SetSphereRadius(200.f);
   sphereCollision->SetUsingAbsoluteScale(true);
}

void AStorageContainer::BeginPlay()
{
   Super::BeginPlay();

   controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
   backpack      = NewObject<UBackpack>(this);

   backpack->AddItems(initialItems);
   sphereCollision->OnComponentEndOverlap.AddDynamic(this, &AStorageContainer::OnLeaveRange);
}

void AStorageContainer::Interact_Implementation(ABaseHero* hero)
{
   if (CanInteract_Implementation()) {
      controllerRef->GetHUDManager()->GetStorageHUD()->SetBackPack(backpack);
      // Typically we don't let anything modify the HUDManager singleton without being dependency injected but we have too many instances of this class to dependency inject
      controllerRef->GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_Storage));
      controllerRef->GetBasePlayer()->heroInBlockingInteraction = hero;
   }
}

bool AStorageContainer::CanInteract_Implementation() const
{
   return Super::CanInteract_Implementation() && !controllerRef->GetBasePlayer()->heroInBlockingInteraction;
}

FVector AStorageContainer::GetInteractableLocation_Implementation() const
{
   return Super::GetInteractableLocation_Implementation();
}

void AStorageContainer::OnLeaveRange(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int otherBodyIndex)
{
   if (controllerRef->GetHUDManager()->IsWidgetOnScreen(HUDs::HS_Storage) && controllerRef->GetBasePlayer()->heroInBlockingInteraction == otherActor) {
      controllerRef->GetHUDManager()->GetStorageHUD()->SetBackPack(nullptr);
      hudManagerRef->AddHUD(static_cast<uint8>(HUDs::HS_Storage));
      controllerRef->GetBasePlayer()->heroInBlockingInteraction = nullptr;
   }
}

void AStorageContainer::SaveInteractable(FMapSaveInfo& mapData)
{
   FStorageContainerSaveInfo storageInfo;
   storageInfo.interactableInfo = SaveInteractableData().interactableInfo;
   backpack->SaveBackpack(storageInfo.backpackInfo);
   mapData.wardrobeInteractables.Add(storageInfo);
}

void AStorageContainer::LoadInteractable(FMapSaveInfo& mapData)
{
   FStorageContainerSaveInfo* storageInfo = mapData.wardrobeInteractables.FindByHash<AStorageContainer*>(GetTypeHash(*this), this);
   if (storageInfo) {
      LoadInteractableData(storageInfo->interactableInfo);
      backpack->EmptyAll();
      backpack->LoadBackpack(storageInfo->backpackInfo);
      GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Storage container named %s found with %d items"), *GetName(), backpack->Count()));
   }
}
