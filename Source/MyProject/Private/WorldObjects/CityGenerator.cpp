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
   // Build roaddata
   // Construct track
   // Draw spline point numbers

   Super::OnConstruction(transform);

   numSplinePoints = splineComponent->GetNumberOfSplinePoints();

   while (numSplinePoints < roadData.Num()) {
      roadData.RemoveAt(roadData.Num() - 1);
      trackList[roadData.Num() - 1]->DestroyComponent();
   }

   while (numSplinePoints > roadData.Num())
      roadData.Add(FRoadData());

   if (buildingMeshes.Num() > 0) {
      trackList.SetNum(roadData.Num() - 1);

      for(auto x : leftMeshList)
         if(IsValid(x))
            x->DestroyComponent();

      for(auto x : rightMeshList)
         if(IsValid(x))
            x->DestroyComponent();

      leftMeshList.Empty();
      rightMeshList.Empty();

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

   track->SetForwardAxis(trackRoadData.meshAxis);
   track->SetStartRoll(trackRoadData.trackBank);
   track->SetStartScale(FVector2D(trackRoadData.trackWidth, trackRoadData.trackThickness));

   track->SetEndRoll(nextTrackRoadData.trackBank);
   track->SetEndScale(FVector2D(nextTrackRoadData.trackWidth, nextTrackRoadData.trackThickness));
   track->SetCollisionProfileName("BlockAll");

   trackList[index] = track;

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