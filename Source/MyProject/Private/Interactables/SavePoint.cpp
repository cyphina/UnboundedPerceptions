// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "SavePoint.h"


// Sets default values
ASavePoint::ASavePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASavePoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASavePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASavePoint::Interact_Implementation(ABaseHero* hero)
{

}

FVector ASavePoint::GetInteractableLocation_Implementation()
{
	return GetActorLocation();
}
