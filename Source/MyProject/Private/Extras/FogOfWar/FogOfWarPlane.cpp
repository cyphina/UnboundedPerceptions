#include "MyProject.h"
#include "RTSGameState.h"
#include "ProceduralMeshComponent.h"
#include "FogOfWarPlane.h"
#include "BasePlayer.h"
#include "Ally.h"
#include "FogOfWarComponent.h"

AFogOfWarPlane::AFogOfWarPlane()
{
   PrimaryActorTick.bCanEverTick = false;
   customMesh                    = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("FOWPlaneCustomMesh"));
}

void AFogOfWarPlane::BeginPlay()
{
   Super::BeginPlay();
   gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState());
   for(APlayerState* playerState : gameStateRef->PlayerArray) {
      basePlayerRefs.Add(Cast<ABasePlayer>(playerState));
   }
}

void AFogOfWarPlane::UpdateVisionPlane()
{
   if(HasAuthority()) {
      int index = 0;
      for(ABasePlayer* basePlayer : basePlayerRefs) {
         for(AAlly* ally : basePlayer->GetAllies()) {
            auto FOWComponent = ally->FindComponentByClass<UFogOfWarComponent>();
            FOWComponent->FindVisibilityPoints();

            // Winding counter clockwise (central point, left vertex, right vertex)
            for(int i = 1; i < FOWComponent->GetVisionPolygonVertices().Num() - 1; ++i) {
               AddTriangle(0, i, i + 1);
            }

            // Final triangle to connect back to beginning of polygon
            AddTriangle(0, FOWComponent->GetVisionPolygonVertices().Num() - 1, 1);
            customMesh->CreateMeshSection(index, FOWComponent->GetVisionPolygonVertices(), triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(),
                                          TArray<FProcMeshTangent>(), false);
            triangles.Empty();
            ++index;
         }
      }
   }
}

void AFogOfWarPlane::AddTriangle(const int32 v1, const int32 v2, const int32 v3)
{
   triangles.Add(v1);
   triangles.Add(v2);
   triangles.Add(v3);
}
