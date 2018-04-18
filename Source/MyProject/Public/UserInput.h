// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/PlayerController.h"
#include "AbilitySystemInterface.h"
#include "UserInput.generated.h"

/**
 * This is the CameraPawnController base class, aka God's Will.
 */

UENUM(BlueprintType)
enum class ECursorStateEnum : uint8
{
	Select UMETA(DisplayName="Select"),
	Moving, 
	Attack,
	PanUp,
	PanDown,
	PanLeft,
	PanRight,
	Interact,
	Talking,
	Item,
	Magic,
	UI
};

class ABasePlayer;
class AUnit;
class AAlly;
class AEnemy;
class ABaseHero;
class UMyGameInstance;
class AHUDManager;
class UQuestManager;

//Callback delegates
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

	void					BeginPlay() override;  
	void					Tick(float deltaSeconds) override;

	virtual void			SetupInputComponent() override; // Bind functionality to input

#pragma region references

private:
	/*PlayerState reference*/
	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetBasePlayer, Category = "References")
	ABasePlayer*			basePlayer;

	/**GameInstance reference.  GameInstance is used for storing consistant data between levels*/
	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetMyGameInstance, Category = "References")
	UMyGameInstance*		gameInstance;

	/**HUDManager ref.  Set this in userinput because it depends on userinput being created first */
	AHUDManager*			hudManager;

public:
	/**offset used for widgets when dragging around*/
	UPROPERTY(BlueprintReadWrite)
	FVector2D				offset;

	UFUNCTION(BlueprintCallable, Category = "Accessors") 
	AHUDManager*			GetHUDManager() const;
	
	UFUNCTION(BlueprintGetter)
	FORCEINLINE ABasePlayer* GetBasePlayer() const { return basePlayer; }

	UFUNCTION(BlueprintGetter)
	FORCEINLINE UMyGameInstance* GetMyGameInstance() const { return gameInstance; }

private:
	//These references store information about hit in member so we don't have to reconstruct every tick
	AActor*					hitActor;
	FHitResult				hitResult;
#pragma endregion

public:
#pragma region selection

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
	FOnAllySelected			OnAllySelectedDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
	FOnAllyDeselected		OnAllyDeselectedDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
	FOnUnitSelected			OnUnitSelectedDelegate;

	//Stores what object collision types we hit when querying for them
	UPROPERTY(BlueprintReadOnly, Category = "Control Settings")
	TArray<TEnumAsByte<EObjectTypeQuery>>	queryableTargetObjects;

	//Creates a dot where we click
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Visual")
	void									CreateClickVisual(FVector visualLocation);
#pragma endregion

#pragma region camera
	UPROPERTY(BlueprintReadWrite, Category = "Control Settings")
	bool									isEdgeMoving;

	UPROPERTY(BlueprintReadWrite, Category = "Control Settings")
	bool									isCamNavDisabled;

	/**Camera move speed multiplier*/
	UPROPERTY(BlueprintReadOnly, Category = "Control Settings")
	float									camMoveSpeedMultiplier = 1;	

	/**Get Camera Pawn Rotation*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Helper")
	FRotator								GetCameraPawnRotation();

	/**Set Camera move speed between clamped values 0 and 3*/
	UFUNCTION(exec, Category = "Control Settings")
	void									SetCamMoveSpeedMultiplier(float newCamMoveSpeed) { camMoveSpeedMultiplier = FMath::Clamp<float>(newCamMoveSpeed,0,3); }
#pragma endregion

#pragma region Cursor
	UPROPERTY(BlueprintReadWrite, Category = "Control Settings")
	TArray<ECursorStateEnum>				cursorDirections;

	/**Similar to change cursor but used for changing to secondary cursors.  If cursorType is not a secondary cursor, toggles off secondary cursors*/ 
	void									SetSecondaryCursor(ECursorStateEnum cursorType);

	//Changes cursor to be a different picture.  Called when hovering over different objects.  Changes hardware cursor and cursorState
	void									ChangeCursor(ECursorStateEnum newCursorState);

	//Show spell cursor
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Visual")
	void									ShowSpellCircle(float AOE);

	//Hide spell cursor
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Visual")
	void									HideSpellCircle();

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	ECursorStateEnum						cursorState;

	//if there is a cursor on screen not created by hovering
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Control Settings")
	bool									hasSecondaryCursor;

	//Check every tick what cursor should be displayed depending on what is hovered
	void									CursorHover(); 
#pragma endregion

#pragma region commands
public:
	//Stop all selected unit actions and hides the spell circle! 
	UFUNCTION(BlueprintCallable, Category = "Action")
	void									StopSelectedAllyCommands();

private:
	//Spline Flight Path Controls
	void									PrevFlight(); 
	void									NextFlight();
	void									Inventory(); 
	void									QuestJournal();
	void									QuestList();
	void									Character(); 
	void									Equipment(); 
	void									Spellbook();
	void									RightClick();
	void									ToggleBreakMenu();

#pragma endregion

#pragma region Vision

	FHitResult								visionHitResult;
	FCollisionObjectQueryParams				queryParamVision;

public:
	/**List of all enemies we've already done hit test*/
	UPROPERTY(BlueprintReadOnly, Category = "Vision")
	TArray<AEnemy*>							checkedEnemies;
	
	/**Lists what enemies are visible so we don't have to keep doing line traces which is an expensive op*/
	UPROPERTY(BlueprintReadOnly, Category = "Vision")
	TArray<AEnemy*>							visibleEnemies; 

	/**
	 * Visiblity of enemies is like a state machine which has six states
	 * Enemy enters vision range and we can see it - Add to possible enemies in radius and add to visible units
	 * Enemy enters vision range but is behind a wall - Add to possible enemies in radius but not to visible units
	 * Enemy leaves vision range - Remove from possible enemies in radius and from visible units
	 * Enemy leaves vision range but was behind a wall so we never saw it - Remove from possible enemies in radius but not from visible units
	 * Enemy peaks a wall and is now in vision - Add to visible enemies but not to possible enemies in radius
	 * Enemy walks behind a wall and is not in vision - Remove from visible enemies but not from possible enemies in radius
	 */
	UFUNCTION(BlueprintCallable, Category = "Vision")
	void									UpdateVisibleEnemies();
	
#pragma endregion
	
};