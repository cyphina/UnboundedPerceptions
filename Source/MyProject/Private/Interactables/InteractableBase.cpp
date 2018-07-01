// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "InteractableBase.h"
#include "Interactables/InteractableActorDecoratorBase.h"


// Sets default values
AInteractableBase::AInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	if(decorator)
	{
		//decorator->Rename(0,this);
		decorator->Init();
	}
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableBase::Interact_Implementation(ABaseHero* hero)
{
	if(CanInteract_Implementation())
		return;
}

FVector AInteractableBase::GetInteractableLocation_Implementation()
{
	return GetActorLocation();
}

bool AInteractableBase::CanInteract_Implementation()
{
	if(!decorator || decorator->Interact())
		return true;
	return false;
}

