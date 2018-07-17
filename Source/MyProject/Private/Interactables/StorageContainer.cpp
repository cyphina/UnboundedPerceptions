// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "StorageContainer.h"
#include "UserInput.h"
#include "Items/Backpack.h"
#include "UI/HUDManager.h"
#include "BasePlayer.h"
#include "WorldObjects/BaseHero.h"

AStorageContainer::AStorageContainer() : AInteractableBase()
{
	scene = CreateDefaultSubobject<USceneComponent>(FName("Scene"));
	SetRootComponent(scene);

	interactableMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	interactableMesh->SetupAttachment(RootComponent);
	interactableMesh->SetCollisionProfileName("Interactable");

	sphereCollision = CreateDefaultSubobject<USphereComponent>(FName("InteractRange"));

	sphereCollision->SetupAttachment(RootComponent);
	sphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	sphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	sphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Overlap); //Friendly
}

void AStorageContainer::BeginPlay()
{
	controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
	backpack = NewObject<UBackpack>(this);

	backpack->AddItems(initialItems);
	sphereCollision->OnComponentEndOverlap.AddDynamic(this, &AStorageContainer::OnLeaveRange);
}

void AStorageContainer::Interact_Implementation(ABaseHero* hero)
{
	if(CanInteract_Implementation())
		controllerRef->GetHUDManager()->AddHUD(backpack, hero);
}

bool AStorageContainer::CanInteract_Implementation()
{
	return Super::CanInteract_Implementation();
}

FVector AStorageContainer::GetInteractableLocation_Implementation()
{
	return Super::GetInteractableLocation_Implementation();
}


void AStorageContainer::OnLeaveRange(UPrimitiveComponent* overlappedComp, AActor* otherActor,
	UPrimitiveComponent* otherComp, int otherBodyIndex)
{
	if(controllerRef->GetHUDManager()->IsWidgetOnScreen(HUDs::HS_Storage) && controllerRef->GetBasePlayer()->interactedHero == otherActor)
	{
		controllerRef->GetHUDManager()->AddHUD(backpack, nullptr);
	}
}
