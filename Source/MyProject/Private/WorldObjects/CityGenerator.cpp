// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "CityGenerator.h"

ACityGenerator::ACityGenerator()
{
   PrimaryActorTick.bCanEverTick = false;

   splineComponent = CreateDefaultSubobject<USplineComponent>("trackPath");
   splineComponent->SetupAttachment(RootComponent);
}

void ACityGenerator::OnConstruction(const FTransform& transform)
{
   // Store number of spline points

   Super::OnConstruction(transform);

   numSplinePoints = splineComponent->GetNumberOfSplinePoints();

   // Build roaddata

   // Remove excess track if we decrease the number of spline points
   while (numSplinePoints < roadData.Num()) {
      roadData.RemoveAt(roadData.Num() - 1);
      trackList[roadData.Num() - 1]->DestroyComponent();
   }

   // Add extra track if the number of spline points is increased
   while (numSplinePoints > roadData.Num())
      roadData.Add(FRoadData());

   // Set the buildings array to the same length as the tracks
   if (buildingMeshes.Num() > 0) {
      trackList.SetNum(roadData.Num() - 1);

      // Clear out building meshes
      for(auto x : leftMeshList)
         if(IsValid(x))
            x->DestroyComponent();

      for(auto x : rightMeshList)
         if(IsValid(x))
            x->DestroyComponent();

      leftMeshList.Empty();
      rightMeshList.Empty();

      // Create tracks and buildings
      for (int i = 0; i < roadData.Num() - 1; ++i) {
         BuildTrackElement(i);
         BuildSideBuildings(i);
      }
   }
}

#if WITH_EDITOR
void ACityGenerator::PostEditChangeProperty(FPropertyChangedEvent& propertyChanged)
{
   Super::PostEditChangeProperty(propertyChanged);
   /*FName propertyName = propertyChanged.Property ? propertyChanged.Property->GetFName() : NAME_None;
   UE_LOG(LogTemp, Warning, TEXT("%s"), *propertyName.ToString());
   if (propertyName == GET_MEMBER_NAME_CHECKED(ACityGenerator, splineComponent) || propertyName == GET_MEMBER_NAME_CHECKED(ACityGenerator, roadData)) {
      while (numSplinePoints < roadData.Num())
         roadData.RemoveAt(roadData.Num() - 1);
      while (numSplinePoints > roadData.Num())
         roadData.Add(FRoadData());
   }*/
}
#endif 

void ACityGenerator::BeginPlay()
{
   Super::BeginPlay();
}

void ACityGenerator::BuildTrackElement(int index)
{
   USplineMeshComponent* track = NewObject<USplineMeshComponent>(this, *(FString("track") + FString::FromInt(index)));
   track->RegisterComponent();
   track->SetMobility(EComponentMobility::Movable);
   track->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

   FRoadData& trackRoadData     = roadData[index];
   FRoadData& nextTrackRoadData = roadData[index + 1];

   track->SetStaticMesh(roadMesh);
   collision ? track->SetCollisionProfileName("Scenery") : track->SetCollisionProfileName("NoCollision");

   // Set track direction, curvature, scale, and collision
   track->SetForwardAxis(trackRoadData.meshAxis);
   track->SetStartRoll(trackRoadData.trackBank); // Track tilt
   track->SetStartScale(FVector2D(trackRoadData.trackWidth, trackRoadData.trackThickness)); // Scale track's thickness horizontally and vertically

   track->SetEndRoll(nextTrackRoadData.trackBank);
   track->SetEndScale(FVector2D(nextTrackRoadData.trackWidth, nextTrackRoadData.trackThickness));
   track->SetCollisionProfileName("BlockAll");

   trackList[index] = track;

   // Setup track positioning
   ESplineCoordinateSpace::Type e            = ESplineCoordinateSpace::Local;
   FVector                      startLoc     = splineComponent->GetLocationAtSplinePoint(index, e);
   FVector                      startTangent = splineComponent->GetTangentAtSplinePoint(index, e);
   FVector                      endLoc       = splineComponent->GetLocationAtSplinePoint(index + 1, e);
   FVector                      endTangent   = splineComponent->GetTangentAtSplinePoint(index + 1, e);
   track->SetStartAndEnd(startLoc, startTangent, endLoc, endTangent, true);
}

void ACityGenerator::BuildSideBuildings(int index)
{
   if (roadData[index].left) {
      UStaticMeshComponent* sideBuilding = NewObject<UStaticMeshComponent>(this, *(FString("buildingLeft") + FString::FromInt(index)));
      sideBuilding->RegisterComponent();
      sideBuilding->SetMobility(EComponentMobility::Movable);
      sideBuilding->AttachToComponent(trackList[index], FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
      sideBuilding->SetCollisionProfileName("VisionBlocker");
	  leftMeshList.Add(sideBuilding);

      UStaticMesh* buildingMesh      = buildingMeshes[roadData[index].meshIndex];
      float        buildingExtent    = buildingMesh->GetBounds().BoxExtent.Size();
      FVector      buildingDirection = FRotator(0, 90, 0).RotateVector(trackList[index]->GetStartTangent().GetSafeNormal());
      sideBuilding->SetRelativeLocation(trackList[index]->GetStartPosition() + buildingDirection * distanceFromRoad + buildingDirection * buildingExtent + FVector(0, distanceFromGround, 0));
      sideBuilding->SetStaticMesh(buildingMesh);
   } else {
      if (leftMeshList.Num() > index && leftMeshList[index]->IsValidLowLevel()) leftMeshList[index]->DestroyComponent();
   }

   if (roadData[index].right) {
      UStaticMeshComponent* sideBuilding = NewObject<UStaticMeshComponent>(this, *(FString("buildingRight") + FString::FromInt(index)));
      sideBuilding->RegisterComponent();
      sideBuilding->SetMobility(EComponentMobility::Movable);
      sideBuilding->AttachToComponent(trackList[index], FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
      sideBuilding->SetCollisionProfileName("VisionBlocker");
	  rightMeshList.Add(sideBuilding);

      UStaticMesh* buildingMesh      = buildingMeshes[roadData[index].meshIndex];
      float        buildingExtent    = buildingMesh->GetBounds().BoxExtent.Size();
      FVector      buildingDirection = FRotator(0, -90, 0).RotateVector(trackList[index]->GetStartTangent().GetSafeNormal());
      sideBuilding->SetRelativeLocation(trackList[index]->GetStartPosition() + buildingDirection * distanceFromRoad + buildingDirection * buildingExtent + FVector(0, distanceFromGround, 0));
      sideBuilding->SetStaticMesh(buildingMesh);
   } else {
      if (rightMeshList.Num() > index && rightMeshList[index]->IsValidLowLevel()) rightMeshList[index]->DestroyComponent();
   }
}