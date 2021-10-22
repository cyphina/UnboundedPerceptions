// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "StorageContainer.h"

#include "AllOf.h"
#include "UserInput.h"
#include "BasePlayer.h"

#include "Items/Backpack.h"
#include "UI/HUDManager.h"

#include "WorldObjects/BaseHero.h"

#include "LevelSaveStructs.h"
#include "RTSIngameWidget.h"
#include "StorageInventory.h"

AStorageContainer::AStorageContainer() : AInteractableBase()
{
   sphereCollision = CreateDefaultSubobject<USphereComponent>(FName("InteractRange"));
   sphereCollision->SetupAttachment(sceneLocation);
   sphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
   sphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
   sphereCollision->SetCollisionResponseToChannel(ALLY_OBJECT_CHANNEL, ECR_Overlap);
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

void AStorageContainer::Interact_Implementation(ABaseHero* Hero)
{
   if(CanInteract_Implementation())
   {
      GetHUDProvider()->GetIngameHUD()->GetStorageHUD()->SetBackPack(backpack);
      GetWidgetToggler()->AddHUD(EHUDs::HS_Storage);
      GetPlayerControllerRef()->GetBasePlayer()->SetHeroBlockingInteraction(Hero);
   }
}

bool AStorageContainer::CanInteract_Implementation() const
{
   return Super::CanInteract_Implementation() && !GetPlayerControllerRef()->GetBasePlayer()->GetHeroBlockingInteraction();
}

FVector AStorageContainer::GetInteractableLocation_Implementation() const
{
   return Super::GetInteractableLocation_Implementation();
}

void AStorageContainer::OnLeaveRange(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int otherBodyIndex)
{
   if(GetWidgetToggler() && GetWidgetToggler()->IsWidgetOnScreen(EHUDs::HS_Storage) &&
      GetPlayerControllerRef()->GetBasePlayer()->GetHeroBlockingInteraction() == otherActor)
   {
      GetHUDProvider()->GetIngameHUD()->GetStorageHUD()->SetBackPack(nullptr);
      GetWidgetToggler()->AddHUD(EHUDs::HS_Storage);
      GetPlayerControllerRef()->GetBasePlayer()->SetHeroBlockingInteraction(nullptr);
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
   if(GetPlayerControllerRef())
   {
      return GetPlayerControllerRef()->GetWidgetToggler();
   }
   return nullptr;
}

TScriptInterface<IHUDProvider> AStorageContainer::GetHUDProvider() const
{
   return GetPlayerControllerRef()->GetWidgetProvider();
}

AUserInput* AStorageContainer::GetPlayerControllerRef() const
{
   return Cast<AUserInput>(UGameplayStatics::GetGameInstance(GetWorld())->GetFirstLocalPlayerController());
}
