// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Searchable.h"

ASearchable::ASearchable()
{
   PrimaryActorTick.bCanEverTick = false;
}

void ASearchable::BeginPlay()
{
   Super::BeginPlay();
}

void ASearchable::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);
}
