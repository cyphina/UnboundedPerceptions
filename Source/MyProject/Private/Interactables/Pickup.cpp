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

APickup::APickup()
{
	//Set up the mesh for the pickup, and set the item name, help text, and item value
	sceneComponent = CreateDefaultSubobject<USceneComponent>(FName("RootComponent"));
	SetRootComponent(sceneComponent);
	interactableMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	interactableMesh->SetupAttachment(sceneComponent);
	sphereCollider = CreateDefaultSubobject<USphereComponent>(FName("Trigger"));
	sphereCollider->SetupAttachment(sceneComponent);
	interactableMesh->SetSimulatePhysics(true);
	interactableMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	interactableMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore); //Enemy
	interactableMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Ignore); //Friendly
}

void APickup::BeginPlay()
{
	Super::BeginPlay();
	CPCRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
	OnPickupDelegate.AddDynamic(this, &APickup::OnPickedUp);
	//create object here
}

void APickup::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);
}

void APickup::Interact_Implementation(ABaseHero* hero)
{
	//Put code here that places the item in the characters inventory
	if (CanInteract_Implementation())
	{
		if (hero->backpack)
		{
			item.count = hero->backpack->AddItem(item);
			OnPickupDelegate.Broadcast();
		}
	}
}

FVector APickup::GetInteractableLocation_Implementation()
{
	return GetActorLocation();
}

void APickup::OnPickedUp()
{
	if (item.count == 0)
	{
		Destroy();
		CPCRef->GetGameMode()->GetQuestManager()->OnItemPickup(item);
	}
	CPCRef->GetHUDManager()->GetInventoryHUD()->LoadItems();
}

void APickup::OnComponentBeginOverlap(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{

}

void APickup::SaveInteractable(FMapSaveInfo& mapData)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("SAVED PICKUP!"));
	mapData.pickupList.Add(GetName());
}
