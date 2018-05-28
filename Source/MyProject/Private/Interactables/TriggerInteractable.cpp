// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "TriggerInteractable.h"
#include "MyGameInstance.h"

ATriggerInteractable::ATriggerInteractable()
{
	PrimaryActorTick.bCanEverTick = false;
	scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(scene);
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	staticMesh->SetupAttachment(RootComponent);
	staticMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3); //Interactable
}

void ATriggerInteractable::BeginPlay()
{
	Super::BeginPlay();
	gameInstanceRef = Cast<UMyGameInstance>(GetGameInstance());
}

void ATriggerInteractable::Interact_Implementation(ABaseHero* hero)
{
	gameInstanceRef->GetTriggerManager()->ActivateTrigger(triggerActivatedOnInteract);
}

FVector ATriggerInteractable::GetInteractableLocation_Implementation()
{
	return GetActorLocation();
}




