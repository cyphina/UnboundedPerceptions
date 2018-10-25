// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "FlightStopActor.generated.h"

UCLASS()
class MYPROJECT_API AFlightStopActor : public AActor
{
   GENERATED_BODY()

 protected:
   // FloatCurve corresponding to previous flight spline component
   UPROPERTY(EditAnywhere)
   UCurveFloat* previousFlightCurve;

   // FloatCurve corresponding to the next flight spline component
   UPROPERTY(EditAnywhere)
   UCurveFloat* NextFlightCurve;

   // A static mesh for our flight stop
   UPROPERTY(VisibleAnywhere)
   UStaticMeshComponent* sM;

   // Spline component that describes the flight path fo the next flight
   UPROPERTY(VisibleAnywhere)
   USplineComponent* nextFlightStop;

   // Spline component that describes flight path of previous flight
   UPROPERTY(VisibleANywhere)
   USplineComponent* previousFlightStop;

 public:
   // Sets default values for this actor's properties
   AFlightStopActor();

   // Returns previous flight curve
   UCurveFloat* GetPreviousFlightCurve() const { return previousFlightCurve; }

   // Returns next flight curve
   UCurveFloat* GetNextFlightCurve() const { return NextFlightCurve; }

   USplineComponent* GetNextFlightSplineComp() const { return nextFlightStop; }

   USplineComponent* GetPreviousFlightSplineComp() const { return previousFlightStop; }

   // Called when the game starts or when spawned
   virtual void BeginPlay() override;

   // Called every frame
   virtual void Tick(float DeltaSeconds) override;
};
