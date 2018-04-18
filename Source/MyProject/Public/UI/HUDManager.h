// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Info.h"
#include "UserWidgetExtensions/AnimHudWidget.h"
#include "UserWidgetExtensions/MyUserWidget.h"
#include "EventSystem/Trigger.h"
#include "HUDManager.generated.h"

class ARTSGameMode;
class AUserInput;
class UMainWidget;
class UCharacterMenu;
class UInventory;
class UEquipmentMenu;
class UActionbarInterface;
class UQuestList;
class UQuestJournal;
class UMinimap;
class UDialogUI;
class UDialogBox;
class UBreakMenu;
class USettingsMenu;

/**All the togglable huds/huds that need some callback when added or hidden 
in the game should be listed here.  Modify HUDCount if we add more.  Dont have HUDCount inside since it makes iterating through huds harder*/
UENUM(BlueprintType)
enum class HUDs : uint8
{
	HS_Ingame, //Basic Combat HUD
	HS_Inventory, //Inventory
	HS_Shop_General, //Shops
	HS_Equipment, //Equipment
	HS_Character, //Character Menu
	HS_Actionbar, //Action bar
	HS_Spellbook, //Spellbook
	HS_QuestJournal, //Quest Journal
	HS_QuestList, //Quest list (like mmo list) widget and minimap indicators
	HS_Dialog, //Dialog prompt for dialog situations
	HS_Minimap, //Minimap
	HS_Social, //More interaction options for socialble NPCs
	HS_Break, //Save/Load/Options menu
	HS_Settings, //Settings Menu
	HS_SaveLoad
};

//The purpose of the HUDManager is for easy swapping in and out widgets.  Not too useful for getting references though
/*HOW TO ADD A NEW HUD
 * 1. Set up how we reference it via add HUD by using proper setup in BPMainHUD since all widgets are part of the mainhud
 * 2. Setup when we can add it to screen by modifying ApplyHUD
 * 3. Setup callbacks if needed
 */
UCLASS(Blueprintable)
class MYPROJECT_API AHUDManager : public AInfo
{
	GENERATED_BODY()

	static const int								HUDCount = 15; //Number of huds we have total.  Change if adding more. Assertion fails if we dont have enough space to remind us to update this size
	
	ARTSGameMode*									gameMode;
	AUserInput*										playerControllerRef;
	TBitArray<FDefaultBitArrayAllocator>			currentlyDisplayedWidgetsBitSet = TBitArray<FDefaultBitArrayAllocator>(false,HUDCount); //widgets that are on screen
	
	int												showMouseCursorCount = 0; //Counter to keep track of how many huds are on screen that want us to show the special hud cursor
	int												enableClickEventsCount = 0; //Counter to keep track of how mnay huds are on screen that disable click events

	/**Applies a hud to the screen: returns true if successful, false otherwise.
	Flip flops, that is, if we have HUD already applied, we turn off */
	bool											ApplyHUD(uint8 newState, bool bShowMouseCursor, bool enableClickEvents, bool canOpenCombat, bool hasAnim);				
	void											UpdateWidgetTracking(int removeIndex, bool canOpenCombat, bool showMouseCursor, bool enableClickEvents); //helper function to update tracking of widgets on screen or widgets blocking actions
																									
public:	

	AHUDManager();

	/**Array storage of all the widgets we'll use*/
	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	TArray<UMyUserWidget*>							widgetReferences;

	/**number of action inhibiting widgets (blocks during combat or such)*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Widgets")
	int												numWidgetsBlocking;

	/**Called when the game starts or when spawned*/
	virtual void									BeginPlay() override;

	/**Quick setting widgets when creating them*/
	UFUNCTION(BlueprintCallable, Category = "HUDManager")
	void											SetWidget(uint8 newState, UMyUserWidget* widgetRef);

	/**Toggle a hud on the screen on/off.  Do not call with dialogbox hud, instead call AddHUDDialog */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "HUDManager")
	void											AddHUD(uint8 newState);
	virtual void									AddHUD_Implementation(uint8 newState);

	/**Allows us to quickly add dialogBox loaded up with conversation.  Else we would have to set the dialog manually, then call AddHUD.  Plus it allows us to 
	 * setup a trigger when we finish conversation which may lead to something else.
	 * @param conversationName - Name of the conversation.  Leave blank to just close
	 * @param onDialogEndTrigger - Trigger which will activate on dialog end.  Could be something game changing, or just open up another menu.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dialog Initiation")
	void											AddHUDDialog(FName conversationName, FTriggerData& onDialogEndTrigger);

	UFUNCTION(BlueprintCallable, Category = "Dialog Initiation")
	bool											IsWidgetOnScreen(HUDs hudToCheck) const { return currentlyDisplayedWidgetsBitSet[static_cast<int>(hudToCheck)]; }
#pragma region accessors

	///--Accessors--///
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
	UMainWidget*									GetMainHUD();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
	UCharacterMenu*									GetCharacterHUD();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
	UEquipmentMenu*									GetEquipHUD() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
	UInventory*										GetInventoryHUD() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
	UActionbarInterface*							GetActionHUD() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
	UQuestList*										GetQuestList() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
	UQuestJournal*									GetQuestJournal() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
	UMinimap*										GetMinimap() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
	UDialogBox*										GetDialogBox() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
	UDialogUI*										GetSocialWindow() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
	UBreakMenu*										GetBreakMenu() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
	USettingsMenu*									GetSettingsMenu() const;
#pragma endregion
};

