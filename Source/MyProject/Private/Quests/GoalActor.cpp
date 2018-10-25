// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "GoalActor.h"

// Sets default values
AGoalActor::AGoalActor()
{
   PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AGoalActor::BeginPlay()
{
   Super::BeginPlay();
   SetOwner(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}
