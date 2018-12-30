// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RTSPawn.generated.h"

UENUM(BlueprintType)
enum class ECursorStateEnum : uint8 { Select UMETA(DisplayName = "Select"), Moving, Attack, PanUp, PanDown, PanLeft, PanRight, Interact, Talking, Item, Magic, UI };

class AUserInput;

/**Pawn that is associated with the main RPG/RTS gameplay input*/
UCLASS()
class MYPROJECT_API ARTSPawn : public APawn
{
   GENERATED_BODY()

 public:
   ARTSPawn();
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
   USceneComponent* scene;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
   USpringArmComponent* cameraArm;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
   UCameraComponent* camera;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
   USpringArmComponent* mapArm;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
   UChildActorComponent* mapCamera;

 protected:
   virtual void BeginPlay() override;

 public:
   virtual void Tick(float DeltaTime) override;
   virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
   virtual void PossessedBy(AController* newController) override;
   virtual void UnPossessed() override;

 private:
   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
   AUserInput* controllerRef;

#pragma region mouse
 public:

   UPROPERTY(BlueprintReadWrite, Category = "Control Settings")
   TArray<ECursorStateEnum> cursorDirections;

   /**Stores what object collision types we hit when tracing for left clicks*/
   UPROPERTY(BlueprintReadOnly, Category = "Control Settings")
   TArray<TEnumAsByte<EObjectTypeQuery>> leftClickQueryObjects;

   /**Stores what object collision types we hit when tracing for right clicks*/
   UPROPERTY(BlueprintReadOnly, Category = "Control Settings")
   TArray<TEnumAsByte<EObjectTypeQuery>> rightClickQueryObjects;

   /**Similar to change cursor but used for changing to secondary cursors.  If cursorType is not a secondary cursor, toggles off secondary cursors*/
   void SetSecondaryCursor(ECursorStateEnum cursorType);

   // Changes cursor to be a different picture.  Called when hovering over different objects.  Changes hardware cursor and cursorState
   void ChangeCursor(ECursorStateEnum newCursorState);

   // Show spell cursor
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Visual")
   void ShowSpellCircle(float AOE);

   // Hide spell cursor
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Visual")
   void HideSpellCircle();

   /**Creates a dot where we click*/
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Visual")
   void CreateClickVisual(FVector visualLocation);

 private:
   // These references store information about hit in member so we don't have to reconstruct every tick
   AActor*    hitActor;
   FHitResult hitResult;

   UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
   ECursorStateEnum cursorState;

   // if there is a cursor on screen not created by hovering
   UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Control Settings")
   bool hasSecondaryCursor;

   // Check every tick what cursor should be displayed depending on what is hovered
   void CursorHover();

#pragma endregion

#pragma region camera

 public:
   int cameraBoundsX;
   int cameraBoundsY;

   static const float maxArmLength;
   static const float minArmLength;
   static const float defaultArmLength;
   static const int baseCameraMoveSpeed = 30;

   /**Makes zooming in and out slower/faster.  Can be set in settings menu*/
   UPROPERTY(BlueprintReadWrite, Category = "Camera Settings")
   float zoomMultiplier = 3.f;

   UPROPERTY(BlueprintReadWrite, Category = "Camera Settings")
   bool isCamNavDisabled;

   /**Camera move speed multiplier*/
   UPROPERTY(BlueprintReadWrite, Category = "Camera Settings")
   float camMoveSpeedMultiplier = 1;

   /**Set Camera move speed between clamped values 0 and 3*/
   UFUNCTION(exec, Category = "Camera Settings")
   void SetCamMoveSpeedMultiplier(float newCamMoveSpeed) { camMoveSpeedMultiplier = FMath::Clamp<float>(newCamMoveSpeed, 0, 3); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera Settings")
   FORCEINLINE bool IsUnitOnScreen(AUnit* unitToCheck);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera Settings")
   FORCEINLINE float GetMaxArmLength() const { return maxArmLength; } 

   UFUNCTION(BlueprintCallable, Category = "Camera Settings")
   void SetCameraArmLength(float newLength);

 private:
   int viewX, viewY;

   void RecalculateViewportSize(FViewport* viewport, uint32 newSize);
   void ZoomIn();
   void ZoomOut();
   void ZoomReset();
   void LockCamera();
   void CameraSpeedOn();
   void CameraSpeedOff();
   void MoveX(float axisValue);
   void MoveY(float axisValue);
   void MMBDragX(float axisValue);
   void MMBDragY(float axisValue);
   void PanX(float axisValue);
   void PanY(float axisValue);
   void PanReset();
   void baseC(float axisValue);
   void EdgeMovementX(float axisValue);
   void EdgeMovementY(float axisValue);

#pragma endregion

#pragma region commands
 public:
   // Stop all selected unit actions and hides the spell circle!
   UFUNCTION(BlueprintCallable, Category = "Action")
   void StopSelectedAllyCommands();

 private:
   // Spline Flight Path Controls
   void PrevFlight();
   void NextFlight();

   void Inventory();
   void QuestJournal();
   void QuestList();
   void Character();
   void Equipment();
   void Spellbook();
   void RightClick();
   void TabNextAlly();
   void UseAbility(int abilityIndex);

   /**Helper function when all selected ally units are cleared */
   UFUNCTION(BlueprintCallable, Category = "Helper")
   void ClearSelectedAllies();

   /**Delegate to pass to BindAction so we can have a parameter for useability*/
   DECLARE_DELEGATE_OneParam(FAbilityUseDelegate, int);

#pragma endregion
};
