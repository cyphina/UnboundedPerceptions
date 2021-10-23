#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RTSCameraPawn.generated.h"

/**
 * DEPRECATED - Used for old pawn that controls the camera overlooking the world.  Can probably remove this one day
 */

UCLASS(Deprecated)
class MYPROJECT_API ADEPRECATED_RTSCameraPawn : public APawn
{
   GENERATED_BODY()

 public:
   // Sets default values for this pawn's properties
   ADEPRECATED_RTSCameraPawn();

 protected:
   // Called when the game starts or when spawned
   virtual void BeginPlay() override;

 public:
   // Called every frame
   virtual void Tick(float DeltaTime) override;

   // Called to bind functionality to input
   virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Accessor")
   void SetCameraArmLength(float newLength);
};
