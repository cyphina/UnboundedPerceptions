// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "FlyComponent.h"
#include "WorldObjects/BaseHero.h"


// Sets default values for this component's properties
UFlyComponent::UFlyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	owner = Cast<ACharacter>(GetOwner());
	if (owner)
	{
		owner->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &UFlyComponent::OnFlightBoxColliderOverlap);
	}
	// ...
}


// Called when the game starts
void UFlyComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFlyComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	if (flightTimeline.IsPlaying()) flightTimeline.TickTimeline(DeltaTime);
	// ...
}


void UFlyComponent::TickTimeline(float value)
{
	float splineLength = activeSplineComponent->GetSplineLength();

	//Get new location based on provided values from the timeline
	//Reason we're multiplying value with splinelength is because all our designed curves in the UE4 editor have a time range of 0-X
	//Where X is the total flight time

	FVector newLocation = activeSplineComponent->GetLocationAtDistanceAlongSpline(value * splineLength, ESplineCoordinateSpace::World);
	owner->SetActorLocation(newLocation);

	FRotator newRotation = activeSplineComponent->GetRotationAtDistanceAlongSpline(value * splineLength, ESplineCoordinateSpace::World);
	newRotation.Pitch = 0;
	owner->SetActorRotation(newRotation);
}

void UFlyComponent::OnFlightBoxColliderOverlap(UPrimitiveComponent * overlappedComponent, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult & sweepResult)
{
	if (otherActor->IsA<AFlightStopActor>())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Flight!"));
		activeFlightStopActor = Cast<AFlightStopActor>(otherActor);
	}
}

void UFlyComponent::NextFlightPathSelected()
{
	if (activeFlightStopActor)
	{
		activeSplineComponent = activeFlightStopActor->GetNextFlightSplineComp();
		UpdateFlightTimeline(activeFlightStopActor->GetNextFlightCurve());
	}
}

void UFlyComponent::PreviousFlightPathSelected()
{
	if (activeFlightStopActor)
	{
		activeSplineComponent = activeFlightStopActor->GetPreviousFlightSplineComp();
		UpdateFlightTimeline(activeFlightStopActor->GetPreviousFlightCurve());
	}
}

void UFlyComponent::UpdateFlightTimeline(UCurveFloat * curveFloatToBind)
{
	flightTimeline = FTimeline();
	FOnTimelineFloat progressFunction;

	progressFunction.BindUFunction(this, FName("TickTimeline"));

	flightTimeline.AddInterpFloat(curveFloatToBind, progressFunction);
	flightTimeline.SetLooping(false);
	flightTimeline.PlayFromStart();

	flightTimeline.SetTimelineLengthMode(TL_LastKeyFrame);

	FOnTimelineEvent timelineEvent;
	timelineEvent.BindUFunction(this, FName("ResetActiveFlightStopActor"));
	flightTimeline.SetTimelineFinishedFunc(timelineEvent);
}

void UFlyComponent::ResetActiveFlightStopActor()
{
	activeFlightStopActor = nullptr;
}

