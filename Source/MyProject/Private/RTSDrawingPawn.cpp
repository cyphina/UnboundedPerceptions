// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSDrawingPawn.h"


// Sets default values
ARTSDrawingPawn::ARTSDrawingPawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ARTSDrawingPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARTSDrawingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARTSDrawingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

