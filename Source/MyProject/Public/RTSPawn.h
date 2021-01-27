#pragma once

#include "ECursorStates.h"
#include "Item.h"
#include "GameFramework/Pawn.h"
#include "GameBaseHelpers/CursorClickFunctionality.h"
#include "RTSPawn.generated.h"

class UBackpack;
class ARTSPawn;
class ABaseHero;
class AUnit;
class AUserInput;
class AAlly;
class ICursorClickFunctionality;
class USkillSlot;
class UCameraComponent;

/**
 * @brief This is the object that represents the player controlling all the units so the camera is attached to this pawn.
 * The RTS Pawn holds controls related to main gameplay. Contrast this to the UserInput which holds control logic for all gameplay types (it's consistent even if we
 * swap out pawn.
 */
UCLASS()
class MYPROJECT_API ARTSPawn : public APawn
{
   GENERATED_BODY()

public:
   ARTSPawn();

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helper")
   bool IsAnyAllySelected() const;

   UCameraComponent* GetCameraComponent() const { return camera; }

   void DisableInput(APlayerController* PlayerController) override;

protected:
   void BeginPlay() override;
   void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
   void Tick(float DeltaTime) override;
   void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
   void PossessedBy(AController* newController) override;
   void UnPossessed() override;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
   class USceneComponent* scene;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
   class USpringArmComponent* cameraArm;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
   UCameraComponent* camera;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
   class USpringArmComponent* mapArm;

private:
   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
   AUserInput* controllerRef;

#pragma region mouse
public:
   /** Scales length of the line we check when we line trace during a click event to query what is under the cursor */
   static const int CLICK_TRACE_LENGTH_MULTIPLIER = 5;

   UPROPERTY(BlueprintReadWrite)
   FVector2D startMousePos;

   UPROPERTY(BlueprintReadWrite)
   FVector2D endMousePos;

   /** Check if we're drag clicking and thus making a feasible selection rect*/
   UFUNCTION(BlueprintCallable, BlueprintPure)
   bool isSelectionRectActive() const;

   /** If there is a cursor on screen not created by hovering */
   UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Control Settings")
   bool hasSecondaryCursor;

   /** Used to correctly pick an arrow cursor when we are edge panning (and possibly panning with MMB in the future)*/
   UPROPERTY(BlueprintReadWrite, Category = "Control Settings")
   TArray<ECursorStateEnum> cursorDirections;

   /** Stores what object collision types we hit when tracing for left clicks*/
   UPROPERTY(BlueprintReadOnly, Category = "Control Settings")
   TArray<TEnumAsByte<EObjectTypeQuery>> leftClickQueryObjects;

   /** Stores what object collision types we hit when tracing for right clicks*/
   UPROPERTY(BlueprintReadOnly, Category = "Control Settings")
   TArray<TEnumAsByte<EObjectTypeQuery>> rightClickQueryObjects;

   /** Similar to change cursor but used for changing to secondary cursors.  If cursorType is not a secondary cursor, toggles off secondary cursors*/
   void SetSecondaryCursor(const ECursorStateEnum cursorType = ECursorStateEnum::Select);

   // Changes cursor to be a different picture.  Called when hovering over different objects.  Changes hardware cursor and cursorState
   UFUNCTION(BlueprintCallable, Category = "Cursor")
   void ChangeCursor(ECursorStateEnum newCursorState);

   /** Show spell circle indicator when casting an AOE spell */
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Visual")
   void ShowSpellCircle(float AOE);

   // Hide spell cursor
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Visual")
   void HideSpellCircle();

   /** Creates a dot where we click*/
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Visual")
   void CreateClickVisual(FVector visualLocation);

   /** Gets what kind of cursor is currently displaying which can tell us what kind of action a user is performing or what things they are hovering over*/
   UFUNCTION(BlueprintGetter = cursorState, BlueprintPure)
   FORCEINLINE ECursorStateEnum GetCursorState() const { return cursorState; }

   /** Returns the object hit from a cursor line trace used when the player clicks*/
   UFUNCTION(BlueprintCallable)
   void GetHitResultClick(FHitResult& clickHitResult) const;

   /** Line trace specifically for right click (only hits ground and enemies)*/
   UFUNCTION(BlueprintCallable)
   void GetHitResultRightClick(FHitResult& clickHitResult) const;

   /** Returns an the actor hit from a cursor line trace or nullptr if no actor was hit*/
   UFUNCTION(BlueprintCallable)
   AActor* GetHitActorClick(FHitResult& clickHitResult);

   /** Draws a rectangle on the screen that we can select units with by dragging*/
   UFUNCTION(BlueprintCallable)
   void CreateSelectionRect();

   /** Hopefully this gets copy elided to its destination and LTCG inlines it*/
   FLinearColor GetSelectionRectColor() const;

   /** Should we enable quick casting (casting without having to click after pressing our spell key. Also makes attack move quick*/
   bool bQuickCast;

private:
   /**Stores last actor hit by the cursor hover trace.  Used so we don't retrace if we hit the same actor, but sometimes canceling actions like clicking the ground should
    *reset this*/
   UPROPERTY()
   AActor* hitActor;

   FHitResult hitResult;

   /** Cache these params so we don't have to constantly remake them*/
   FCollisionQueryParams clickCollisionQueryParams;

   UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
   ECursorStateEnum cursorState;

   // Check every tick what cursor should be displayed depending on what is hovered
   void CursorHover();

   TArray<TArray<TWeakObjectPtr<AAlly>>, TFixedAllocator<5>> controlGroups;

   // Used to pan camera over to first unit in a control group if the control group button is double tapped
   FTimerHandle controlDoubleTapHandle;

   // Used to continually track the control group leader as the key is held down
   FTimerHandle controlDoubleTapHoldHandle;

   TUniquePtr<ICursorClickFunctionality> clickFunctionalityClass;

#pragma endregion

#pragma region camera

public:
   int cameraBoundsX;
   int cameraBoundsY;

   static const float maxArmLength;
   static const float minArmLength;
   static const float defaultArmLength;
   static const int   baseCameraMoveSpeed = 30;

   /**Makes zooming in and out slower/faster.  Can be set in settings menu*/
   UPROPERTY(BlueprintReadWrite, Category = "Camera Settings")
   float zoomMultiplier = 3.f;

   UPROPERTY(BlueprintReadWrite, Category = "Camera Settings")
   bool isCamNavDisabled;

   /**Camera move speed multiplier*/
   UPROPERTY(BlueprintReadWrite, Category = "Camera Settings")
   float camMoveSpeedMultiplier = 1;

   /**If a BLUI event was raised due to clicking on the browser in a way that sucessfully interacted with the browser widget, then the camera pawn
    *needs to know so it register the click as a UI event rather than gameplay event*/
   UPROPERTY(BlueprintReadWrite, Category = "Camera Settings")
   bool clickedOnBrowserHud = false;

   /**Set Camera move speed between clamped values 0 and 3*/
   UFUNCTION(exec, Category = "Camera Settings")
   void SetCamMoveSpeedMultiplier(float newCamMoveSpeed) { camMoveSpeedMultiplier = FMath::Clamp<float>(newCamMoveSpeed, 0, 3); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera Settings")
   bool IsUnitOnScreen(AUnit* unitToCheck) const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera Settings")
   FORCEINLINE float GetMaxArmLength() const { return maxArmLength; }

   UFUNCTION(BlueprintCallable, Category = "Camera Settings")
   void SetCameraArmLength(float newLength) const;

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

   void EdgeMovementX(float axisValue);
   void EdgeMovementY(float axisValue);

#pragma endregion

#pragma region commands
public:
   /** Stop all selected unit actions, stops AI behavior, and hides the spell circle! */
   UFUNCTION(BlueprintCallable, Category = "Action")
   void StopSelectedAllyCommands();

private:
   void RightClick();
   void RightClickShift();
   void LeftClick();
   void LeftClickReleased();
   void LeftClickShift();

   void TabThroughSelection();
   void TabThroughGroup() const;
   void TabSingleSelection() const;

   void UseAbility(int abilityIndex);
   /** Delegate to pass to BindAction so we can have a parameter for useability*/
   DECLARE_DELEGATE_OneParam(FAbilityUseDelegate, int);

   void AttackMoveInitiate();

   /** Select any control group that is bound to a key*/
   void SelectControlGroup(int controlIndex);
   DECLARE_DELEGATE_OneParam(FSelectControlGroupDelegate, int);

   /**
    *-1 when reset, 0-4 to signify what control group button was last pressed
    * Used to check when we double tap to move our camera to the control group
    */
   uint8 lastControlTappedIndex = 0;

   /** The index of the control group key being held down*/
   uint8 controlBeingHeldIndex = 0;

   /** Pans the camera to the first unit in the control group if we double tap the control select button*/
   FTimerHandle ControlGroupDoupleTapHandle;
   
   UFUNCTION()
   void ControlGroupDoubleTapTimer();

   UFUNCTION()
   void ControlGroupDoubleTapHoldFollow();

   UFUNCTION()
   void StopContolGroupFollow(int releaseIndex);

   /** Create a control group */
   void MakeControlGroup(int controlGroupIndex);
   DECLARE_DELEGATE_OneParam(FMakeControlGroupDelegate, int);

   void OnSkillSlotSelected(int skillIndex);
   void OnUnitSlotSelected(AUnit* unitSelected);
   void OnInventorySlotSelected(int slotIndex);
   void OnStorageSlotSelected(int slotIndex);
   void OnEquipmentSlotSelected(int slotIndex);
   void OnShopSlotSelected(int slotIndex);

   UFUNCTION()
   void OnItemSlotDroppedFromInventory(int dragSlotIndex, int dropSlotIndex, UBackpack* dragPack, UBackpack* dropPack);

   UFUNCTION()
   void OnItemSlotDroppedFromStorage(int dragSlotIndex, int dropSlotIndex, UBackpack* dragPack, UBackpack* dropPack);
   
   void HandleInventoryItemSelected(ABaseHero* heroWithInvShown, int itemUsedSlotIndex, FMyItem itemUsed) const;
   void HandleTransferStorageItems(ABaseHero* heroWithInvShown, int itemUsedSlotIndex, FMyItem itemToDeposit) const;
   void HandleSellItemToStore(ABaseHero* heroWithInvShown, int itemUsedSlotIndex, FMyItem itemToDeposit) const;

#pragma endregion

#pragma region selection
public:

#pragma endregion
};
