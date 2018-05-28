// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "WorldObjects/BaseHero.h"
#include "Pickup.h"
#include "UserInput.h"
#include "UI/HUDManager.h"
#include "Items/Inventory.h"

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
	if (hero->backpack && hero->backpack->AddItem(item))
	{
		OnPickupDelegate.Broadcast();
	}
}

FVector APickup::GetInteractableLocation_Implementation()
{
	return GetActorLocation();
}

void APickup::OnPickedUp()
{
	interactableMesh->SetVisibility(false); //don't want to destroy since our inventory has this as the reference but we may later on make data and vision seperate
	interactableMesh->SetSimulatePhysics(false);
	interactableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CPCRef->GetHUDManager()->GetInventoryHUD()->LoadItems();
}

void APickup::OnComponentBeginOverlap(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{

}
