// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "CityGenerator.generated.h"

USTRUCT(BlueprintType)
struct FRoadData {
   GENERATED_USTRUCT_BODY()
   /**Should there be a city mesh be on the left?*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   bool left = false;
   /**Should there be a city mesh be on the right?*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   bool right = false;
   /**Sets the track roll?*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   float trackBank = 0;
   /**Should how far track is scaled in X?*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   float trackWidth = 1.f;
   /**Sets how far track is scaled in Y?*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   float trackThickness = 1.f;
   /**What mesh this part of the track has*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int meshIndex = 0;
   /**Sets orientation of forward axis of the track (for meshes with different forward axis)*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   TEnumAsByte<ESplineMeshAxis::Type> meshAxis = ESplineMeshAxis::X;
};

UCLASS()
class MYPROJECT_API ACityGenerator : public AActor
{
   GENERATED_BODY()

 public:
   ACityGenerator();

   /**How far should the buildings be from the road*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Params")
   int distanceFromRoad = 20.f;

   /**How far should the buildings be from the ground*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Params")
   int distanceFromGround = 0.f;

   /**List of all the parameters for the different pieces*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Params")
   TArray<FRoadData> roadData;

   /**Mesh for the road or street or something*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Params")
   UStaticMesh* roadMesh;

   /**Possible building meshes that can be created along the path*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Params")
   TArray<UStaticMesh*> buildingMeshes;

   /**Array of meshes that has been created*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Params")
   TArray<USplineMeshComponent*> trackList;

   /**Array of left side meshes that has been created*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Params")
   TArray<UStaticMeshComponent*> leftMeshList;

   /**Array of right side meshes that has been created*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Params")
   TArray<UStaticMeshComponent*> rightMeshList;

   /**Enables/Disables collision to reduce lag*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Params")
   bool collision;

 protected:
   void OnConstruction(const FTransform& Transform) override;

   /**Called after property is modified*/
#if WITH_EDITOR
   void PostEditChangeProperty(FPropertyChangedEvent& propertyChanged) override;
#endif

   virtual void BeginPlay() override;

   UPROPERTY(VisibleAnywhere)
   USplineComponent* splineComponent;

 private:
   int numSplinePoints;

   /**Creates tracks along the spline points*/
   UFUNCTION(BlueprintCallable, Category = "Generation")
   void BuildTrackElement(int index);

   /**Creates building static meshes and offsets them to the left and right of the spline control point*/
   UFUNCTION(BlueprintCallable, Category = "Generation")
   void BuildSideBuildings(int index);
};
