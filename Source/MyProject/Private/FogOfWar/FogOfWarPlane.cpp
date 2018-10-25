#include "MyProject.h"
#include "RTSGameState.h"
#include "ProceduralMeshComponent.h"
#include "FogOfWarPlane.h"
#include "BasePlayer.h"
#include "Ally.h"

AFogOfWarPlane::AFogOfWarPlane()
{
   PrimaryActorTick.bCanEverTick = true;
   customMesh                    = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("FOWPlaneCustomMesh"));
}

void AFogOfWarPlane::BeginPlay()
{
   Super::BeginPlay();
   gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState());
   for (APlayerState* playerState : gameStateRef->PlayerArray) {
      basePlayerRefs.Add(Cast<ABasePlayer>(playerState));
   }
}

void AFogOfWarPlane::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);
}

void AFogOfWarPlane::UpdateVisionPlane()
{
   if (HasAuthority()) {
      int index = 0;
      for (ABasePlayer* basePlayer : basePlayerRefs) {
         for (AAlly* ally : basePlayer->allies) {
            ally->FindVisibilityPoints();
            for (int i = 1; i < ally->visionPolygonVertices.Num() - 1; ++i) {
               AddTriangle(0, i, i + 1);
            }

            AddTriangle(0, ally->visionPolygonVertices.Num() - 1, 1);
            customMesh->CreateMeshSection(index, ally->visionPolygonVertices, triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
            triangles.Empty();
            ++index;
         }
      }
   }
}

void AFogOfWarPlane::AddTriangle(int32 v1, int32 v2, int32 v3)
{
   triangles.Add(v1);
   triangles.Add(v2);
   triangles.Add(v3);
}
