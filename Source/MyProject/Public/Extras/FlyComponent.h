// Fill out your copyright notice in the Description page of Project Settings.
// Works with any actor that derives from character with a capsule collider
#pragma once

#include "FlightStopActor.h"
#include "Components/TimelineComponent.h"
#include "Components/ActorComponent.h"
#include "FlyComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UFlyComponent : public UActorComponent
{
   GENERATED_BODY()

 public:
   // Sets default values for this component's properties
   UFlyComponent();

   // Called when the game starts
   virtual void BeginPlay() override;

   // Called every frame
   virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

   // executes when we're pressing then nextflightpath keybind
   void NextFlightPathSelected();

   void PreviousFlightPathSelected();

#pragma region Flight
 private:
   ACharacter* owner;
   FTimeline   flightTimeline;

   // function that ticks the timeline
   UFUNCTION()
   void TickTimeline(float value);

   // active spline component, meaning the flight path the character is currently following
   USplineComponent* activeSplineComponent;

   // selected flight stop actor
   AFlightStopActor* activeFlightStopActor;

   // box overlap function
   UFUNCTION()
   void OnFlightBoxColliderOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

   // updates the flight timeline with a new curve and starts the flight
   void UpdateFlightTimeline(UCurveFloat* curveFloatToBind);

   UFUNCTION()
   void ResetActiveFlightStopActor();
#pragma endregion
};
