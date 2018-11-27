// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "MinigamePawn.h"


// Sets default values
AMinigamePawn::AMinigamePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMinigamePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMinigamePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMinigamePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMinigamePawn::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);
	StartMinigame();
}

