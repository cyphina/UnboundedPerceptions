// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSCameraPawn.h"

// Sets default values
ADEPRECATED_RTSCameraPawn::ADEPRECATED_RTSCameraPawn()
{
   // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
   PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADEPRECATED_RTSCameraPawn::BeginPlay()
{
   Super::BeginPlay();
}

// Called every frame
void ADEPRECATED_RTSCameraPawn::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADEPRECATED_RTSCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
   Super::SetupPlayerInputComponent(PlayerInputComponent);
}
