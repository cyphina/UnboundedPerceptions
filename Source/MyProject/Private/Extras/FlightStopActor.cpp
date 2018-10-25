// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "FlightStopActor.h"

// Sets default values
AFlightStopActor::AFlightStopActor()
{
   // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
   PrimaryActorTick.bCanEverTick = true;
   sM                            = CreateDefaultSubobject<UStaticMeshComponent>(FName("SM"));
   SetRootComponent(sM);

   // Init splines
   nextFlightStop     = CreateDefaultSubobject<USplineComponent>(FName("SplineComp"));
   previousFlightStop = CreateDefaultSubobject<USplineComponent>(FName("PreviousFlightStop"));

   // Attach to root component
   nextFlightStop->SetupAttachment(sM);
   previousFlightStop->SetupAttachment(sM);
}

// Called when the game starts or when spawned
void AFlightStopActor::BeginPlay()
{
   Super::BeginPlay();
}

// Called every frame
void AFlightStopActor::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);
}
