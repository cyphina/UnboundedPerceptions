// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSCameraPawn.h"


// Sets default values
ARTSCameraPawn::ARTSCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARTSCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARTSCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARTSCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

