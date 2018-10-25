// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "FloatingActor.generated.h"

UCLASS()
class MYPROJECT_API AFloatingActor : public AActor
{
   GENERATED_BODY()

 private:
   // start location
   FVector actorInitialLocation;
   // bounce location
   FVector targetLocation;
   // timeline to cause floating
   FTimeline tL;
   // function to handle every tick
   UFUNCTION()
   void HandleProgress(float Value);

 public:
   // Sets default values for this actor's properties
   AFloatingActor();

   // Called when the game starts or when spawned
   virtual void BeginPlay() override;

   // Called every frame
   virtual void Tick(float DeltaSeconds) override;

   UPROPERTY(VisibleAnywhere)
   UStaticMeshComponent* staticMesh;

   /*Max height value that actor is going to reach*/
   UPROPERTY(EditAnywhere, Category = "Timeline")
   float maxBounceHeight;

   UPROPERTY(EditAnywhere, Category = "Timeline")
   UCurveFloat* curveFloat;
};
