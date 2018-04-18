// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "FloatingActor.h"

// Sets default values
AFloatingActor::AFloatingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initialize static mesh of actor
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
}

// Called when the game starts or when spawned
void AFloatingActor::BeginPlay()
{
	Super::BeginPlay();
	
	//checking if the curvefloat that was entered in the blueprints was valid
	if(curveFloat)
	{
		/* ProgressFunction contains signature of the function that is going to execute very tick.  It's a delegate*/
		FOnTimelineFloat progressFunction;

		/* In order to bind the function our ufunction, we need a name its name */
		progressFunction.BindUFunction(this, FName("HandleProgress"));

		//Setup the loop status and function that is going to fire when the timeline ticks
		tL.AddInterpFloat(curveFloat, progressFunction);
		tL.SetLooping(true);

		/*Initialize our home nad target location to make the actor float between them*/
		actorInitialLocation = targetLocation = GetActorLocation();
		targetLocation.Z += maxBounceHeight;

		tL.PlayFromStart();
	}
}

// Called every frame
void AFloatingActor::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);
	tL.TickTimeline(DeltaTime);
}

void AFloatingActor::HandleProgress(float value)
{
	FVector newLocation = FMath::Lerp(actorInitialLocation, targetLocation, value);
	SetActorLocation(newLocation);
}