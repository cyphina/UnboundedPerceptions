// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerController.h"
#include "UserInput.generated.h"

/**
 * This is the CameraPawnController base class, aka God's Will.
 */

UENUM(BlueprintType)
enum class ECursorStateEnum : uint8 { Select UMETA(DisplayName = "Select"), Moving, Attack, PanUp, PanDown, PanLeft, PanRight, Interact, Talking, Item, Magic, UI };

class ABasePlayer;
class AUnit;
class AAlly;
class AEnemy;
class ABaseHero;
class UMyGameInstance;
class AHUDManager;
class ARTSGameMode;
class ARTSGameState;
class ARTSCameraPawn;
class UQuestManager;

// Callback delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllySelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllyDeselected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnitSelected);

UCLASS()
class MYPROJECT_API AUserInput : public APlayerController
{
   GENERATED_BODY()

 public:
   AUserInput();

   /**Replacement for BeginPlay because in 4.19 GameInstance BeginPlay always starts off but actor beginplay goes before PlayerController for some reason...*/

   void BeginPlay() override;
   void Tick(float deltaSeconds) override;

   virtual void SetupInputComponent() override; // Bind functionality to input
   int          cameraBoundsX;
   int          cameraBoundsY;

#pragma region references

 private:
   /*PlayerState reference*/
   UPROPERTY(VisibleAnywhere, BlueprintGetter = "GetBasePlayer", Category = "References")
   ABasePlayer* basePlayer;

   /**GameInstance reference.  GameInstance is used for storing consistant data between levels*/
   UPROPERTY(VisibleAnywhere, BlueprintGetter = "GetMyGameInstance", Category = "References")
   UMyGameInstance* gameInstance;

   /**HUDManager ref.  Set this in userinput because it depends on userinput being created first.  Also client-side HUD manipulation doesn't do very much,
    *since we don't store any important data in the huds */
   AHUDManager* hudManager;

   ARTSGameMode* gameMode;

   ARTSGameState* gameState;

   ARTSCameraPawn* cameraPawn;

 public:
   /**offset used for widgets when dragging around*/
   UPROPERTY(BlueprintReadWrite)
   FVector2D offset;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   FORCEINLINE AHUDManager* GetHUDManager() const { return hudManager; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   FORCEINLINE ARTSGameState* GetGameState() const { return gameState; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   FORCEINLINE ARTSGameMode* GetGameMode() const { return gameMode; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   FORCEINLINE ABasePlayer* GetBasePlayer() const { return basePlayer; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   FORCEINLINE ARTSCameraPawn* GetCameraPawn() const { return cameraPawn; }

   UFUNCTION(BlueprintGetter)
   FORCEINLINE UMyGameInstance* GetMyGameInstance() const { return gameInstance; }

 private:
   // These references store information about hit in member so we don't have to reconstruct every tick
   AActor*    hitActor;
   FHitResult hitResult;
#pragma endregion

 public:
#pragma region selection

   UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
   FOnAllySelected OnAllySelectedDelegate;

   UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
   FOnAllyDeselected OnAllyDeselectedDelegate;

   UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
   FOnUnitSelected OnUnitSelectedDelegate;

   /**Stores what object collision types we hit when tracing for left clicks*/
   UPROPERTY(BlueprintReadOnly, Category = "Control Settings")
   TArray<TEnumAsByte<EObjectTypeQuery>> leftClickQueryObjects;

   /**Stores what object collision types we hit when tracing for right clicks*/
   UPROPERTY(BlueprintReadOnly, Category = "Control Settings")
   TArray<TEnumAsByte<EObjectTypeQuery>> rightClickQueryObjects;

   /**Helper function when all selected ally units are cleared */
   UFUNCTION(BlueprintCallable, Category = "Helper")
   void ClearSelectedAllies();

   /*UFUNCTION(BlueprintPure, BlueprintCallable, Category = "AI")
   FORCEINLINE AAllyAIController*			GetAllyAIController() const { return allyControllerRef; }*/

   /**Creates a dot where we click*/
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Visual")
   void CreateClickVisual(FVector visualLocation);

#pragma endregion

#pragma region camera

   static const int baseCameraMoveSpeed = 30;

   UPROPERTY(BlueprintReadWrite, Category = "Control Settings")
   bool isCamNavDisabled;

   /**Camera move speed multiplier*/
   UPROPERTY(BlueprintReadWrite, Category = "Control Settings")
   float camMoveSpeedMultiplier = 1;

   /**Set Camera move speed between clamped values 0 and 3*/
   UFUNCTION(exec, Category = "Control Settings")
   void SetCamMoveSpeedMultiplier(float newCamMoveSpeed) { camMoveSpeedMultiplier = FMath::Clamp<float>(newCamMoveSpeed, 0, 3); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helper")
   FORCEINLINE bool IsUnitOnScreen(AUnit* unitToCheck);

 private:
   UFUNCTION(BlueprintCallable, Category = "Camera Movement")
   void MoveX(float axisValue);

   UFUNCTION(BlueprintCallable, Category = "Camera Movement")
   void MoveY(float axisValue);

   UFUNCTION(BlueprintCallable, Category = "Camera Movement")
   void EdgeMovementX();

   UFUNCTION(BlueprintCallable, Category = "Camera Movement")
   void EdgeMovementY();
#pragma endregion

#pragma region Cursor
 public:
   UPROPERTY(BlueprintReadWrite, Category = "Control Settings")
   TArray<ECursorStateEnum> cursorDirections;

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

 private:
   UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
   ECursorStateEnum cursorState;

   // if there is a cursor on screen not created by hovering
   UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Control Settings")
   bool hasSecondaryCursor;

   // Check every tick what cursor should be displayed depending on what is hovered
   void CursorHover();
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
   void ToggleBreakMenu();
   void TabNextAlly();

#pragma endregion
};
