// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "StorageContainer.h"

#include "AllOf.h"
#include "UserInput.h"
#include "BasePlayer.h"

#include "Items/Backpack.h"
#include "UI/HUDManager.h"

#include "BasePlayer.h"
#include "Inventory.h"
#include "WorldObjects/BaseHero.h"

#include "LevelSaveStructs.h"
#include "RTSIngameWidget.h"

AStorageContainer::AStorageContainer() :
   AInteractableBase()
{
   sphereCollision = CreateDefaultSubobject<USphereComponent>(FName("InteractRange"));
   sphereCollision->SetupAttachment(sceneLocation);
   sphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
   sphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
   sphereCollision->SetCollisionResponseToChannel(FRIENDLY_CHANNEL, ECR_Overlap);
   sphereCollision->SetSphereRadius(200.f);
   sphereCollision->SetUsingAbsoluteScale(true);
}

void AStorageContainer::BeginPlay()
{
   Super::BeginPlay();

   backpack = NewObject<UBackpack>(this);
   backpack->SetItemMax(maxStorage);

   if(!ensure(Algo::AllOf(backpack->AddItems(initialItems))))
   {
      UE_LOG(LogTemp, Error, TEXT("Problem setting up initial items for storage container %s. Not enough space to add all items."), *GetName());
   }

   sphereCollision->OnComponentEndOverlap.AddDynamic(this, &AStorageContainer::OnLeaveRange);
}

void AStorageContainer::Interact_Implementation(ABaseHero* hero)
{
   if(CanInteract_Implementation())
   {
      GetHUDProvider()->GetIngameHUD()->GetStorageHUD()->SetBackPack(backpack);
      GetWidgetToggler()->AddHUD(static_cast<uint8>(EHUDs::HS_Storage));
      GetPlayerControllerRef()->GetBasePlayer()->heroInBlockingInteraction = hero;
   }
}

bool AStorageContainer::CanInteract_Implementation() const
{
   return Super::CanInteract_Implementation() && !GetPlayerControllerRef()->GetBasePlayer()->heroInBlockingInteraction;
}

FVector AStorageContainer::GetInteractableLocation_Implementation() const
{
   return Super::GetInteractableLocation_Implementation();
}

void AStorageContainer::OnLeaveRange(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int otherBodyIndex)
{
   if(GetWidgetToggler()->IsWidgetOnScreen(EHUDs::HS_Storage) && GetPlayerControllerRef()->GetBasePlayer()->heroInBlockingInteraction == otherActor)
   {
      GetHUDProvider()->GetIngameHUD()->GetStorageHUD()->SetBackPack(nullptr);
      GetWidgetToggler()->AddHUD(static_cast<uint8>(EHUDs::HS_Storage));
      GetPlayerControllerRef()->GetBasePlayer()->heroInBlockingInteraction = nullptr;
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
   if(storageInfo)
   {
      LoadInteractableData(storageInfo->interactableInfo);
      backpack->EmptyAll();
      backpack->LoadBackpack(storageInfo->backpackInfo);
      GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Storage container named %s found with %d items"), *GetName(), backpack->Count()));
   }
}

TScriptInterface<IWidgetToggler> AStorageContainer::GetWidgetToggler() const
{
   return GetPlayerControllerRef()->GetWidgetToggler();
}

TScriptInterface<IHUDProvider> AStorageContainer::GetHUDProvider() const
{
   return GetPlayerControllerRef()->GetWidgetProvider();
}

AUserInput* AStorageContainer::GetPlayerControllerRef() const
{
   return Cast<AUserInput>(UGameplayStatics::GetGameInstance(GetWorld())->GetFirstLocalPlayerController());
}
