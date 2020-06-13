// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Info.h"
#include "UserWidgetExtensions/AnimHudWidget.h"
#include "UserWidgetExtensions/MyUserWidget.h"
#include "EventSystem/Trigger.h"
#include "DialogStructs.h"
#include "HUDManager.generated.h"

class ARTSGameMode;
class AUserInput;
class UMainWidget;
class UCharacterMenu;
class UInventory;
class UHeroInventory;
class UStoreInventory;
class UEquipmentMenu;
class UActionbarInterface;
class UQuestList;
class UQuestJournal;
class UMinimap;
class UDialogUI;
class UDialogBox;
class UBreakMenu;
class USettingsMenu;
class UItemExamineWidget;
class UBackpack;
class UConfirmationBox;
class URTSInputBox;
class UStartMenu;
class USpellbookHUD;
class URTSIngameWidget;
class AShopNPC;
class UDIRender;

struct FDialogData;
enum class EDialogBoxCloseCase : uint8;

/**All the togglable huds/huds that need some callback when added or hidden
in the game should be listed here.  Modify HUDCount if we add more.  Dont have HUDCount inside since it makes iterating through huds harder*/
UENUM(BlueprintType)
enum class HUDs : uint8 {
   /**Used to be used to display all the hero information simulaneously, in a sidebar but replaced by actionbar*/
   HS_Ingame,
   /**Inventory for all heroes in party*/
   HS_Inventory,
   /**Inventory for any storage interactable*/
   HS_Storage,
   /**Inventory for any NPC shops... or whatever has a backpack to sell stuff*/
   HS_Shop_General,
   /**Equipment menu*/
   HS_Equipment,
   /**Character Information Menu - Shows information about one character at a time (whoever is first in selection)*/
   HS_Character,
   /**Action bar for information about selected heroes, commands, AI options, channeling bar, etc.*/
   HS_Actionbar,
   /**Spellbook to level up and change spells in actionbar slot*/
   HS_Spellbook,
   /**Quest journal to see detailed information about what quests we're on, what quests we've finished, etc.*/
   HS_QuestJournal,
   /**Quest list is a widget on the side that can change what quest is being tracked on the minimap as well as conveys condensed information about quest*/
   HS_QuestList,
   /**Dialog prompt for dialog situations*/
   HS_Dialog,
   /**Minimap*/
   HS_Minimap,
   /**More interaction options for sociable NPCs*/
   HS_Social,
   /**Menu that leads to Save/Load/Options menu*/
   HS_Break,
   /**Allows graphical and gameplay settings changes*/
   HS_Settings,
   /**Save/Load Menu*/
   HS_SaveLoad,
   /**Image that shows an item's image in closer details*/
   HS_ExamineMenu,
   /**Shows a confirmation box*/
   HS_Confirmation,
   /**Shows a box that lets you input a number*/
   HS_InputBox,
   /**Chatbox to type in commands to be processed via NLP and converted to actions*/
   HS_ChatBox,
   /**Start menu for the beginning of the game*/
   HS_Start,
   /**Widget which has buttons we can use to map our keys*/
   HS_KeyMap,
   /** Number of widgets managed by the hudmanager*/
   HS_Count
};

ENUM_RANGE_BY_COUNT(HUDs, static_cast<uint8>(HUDs::HS_Count));

//! SINGLETON CLASS that is partially Injected
// The purpose of the HUDManager is for easy swapping in and out widgets.  Not too useful for getting references though.
/*HOW TO ADD A NEW HUD
 * 1. Set up how we reference it via add HUD by using proper setup in BPMainHUD since all widgets are part of the mainhud
 * 2. Setup when we can add it to screen by modifying ApplyHUD
 * 3. Setup callbacks if needed
 */
UCLASS(Blueprintable)
class MYPROJECT_API AHUDManager : public AInfo
{
   GENERATED_BODY()

   static const int HUDCount = static_cast<uint8>(
       HUDs::HS_Count); // Number of huds we have total.  Change if adding more. Assertion fails if we dont have enough space to remind us to update this size

   ARTSGameMode* gameMode;
   AUserInput* playerControllerRef;

   TBitArray<FDefaultBitArrayAllocator> currentlyDisplayedWidgetsBitSet = TBitArray<FDefaultBitArrayAllocator>(false, HUDCount); // widgets that are on screen

   int  showMouseCursorCount   = 0;     // Counter to keep track of how many huds are on screen that want us to show the special hud cursor
   int  enableClickEventsCount = 0;     // Counter to keep track of how mnay huds are on screen that disable click events
   bool bBlocked               = false; // Can we open any other widgets right now or are we blocked?

   /**Applies a hud to the screen: returns true if successful, false otherwise.
    *Flip flops, that is, if we have HUD already applied, we turn off
    *@param newState - The EHUD value corresponding to this HUD.
    *@param enableClickEvents - Can we click with this hud open? (includes clicking in HUD itself)
    *@param canOpenCombat - Can this HUD be opened during combat?  If not, then don't let the player perform regular actions when it's open by switchin to the UI cursor
    *@param bBlocking - Can other MyUserWidgets be opened while this one is open?
    */
   bool ApplyHUD(uint8 newState, bool enableClickEvents, bool canOpenCombat, bool bBlocking = false);
   void UpdateWidgetTracking(int removeIndex, bool enableClickEvents,
                             bool canOpenCombat); // helper function to update tracking of widgets on screen or widgets blocking actions

   void InjectDependency(UObject* objectToInject)
   {
      FObjectProperty* objectProperty = FindFProperty<FObjectProperty>(objectToInject->GetClass(), "hudManagerRef"); // Find the property on that object
      objectProperty->SetPropertyValue_InContainer(objectToInject, this); // Get that property and set its value in the container (actually object) and set it to this
   }

 public:
   AHUDManager();

   /**Array storage of all the widgets we'll use*/
   UPROPERTY(BlueprintReadWrite, Category = "Widgets")
   TArray<UMyUserWidget*> widgetReferences;

   /**number of action inhibiting widgets (blocks during combat or such)*/
   UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Widgets")
   int numWidgetsBlocking;

   UPROPERTY(EditDefaultsOnly)
   TSubclassOf<UDIRender> damageIndicatorClass;

   /**Called when the game starts or when spawned*/
   virtual void BeginPlay() override;

   /**Quick setting widgets when creating them*/
   UFUNCTION(BlueprintCallable, Category = "HUDManager")
   void SetWidget(uint8 newState, UMyUserWidget* widgetRef);

   /**Toggle a hud on the screen on/off.  C++ version.*/
   void AddHUD(uint8 newState);

   /**Allows us to quickly add dialogBox loaded up with conversation.  Else we would have to set the dialog manually, then call AddHUD.
    * setup a trigger when we finish conversation which may lead to something else.
    * @param conversationName - Name of the conversation.  Leave blank to just close
    * @param dialogSource - What event caused this dialogBox to open?
    */
   void ShowDialogWithSource(FName conversationName, EDialogBoxCloseCase dialogSource);

   /**Variant of AddHUDDialog for text that not in the convesrsation table. Sometimes has an interacting hero associated with it which should be set before the call to this
    * @param linesToDisplay - DialogInformation to pass in
    * @param dialogSource - What event caused this dialogBox to open?
    */
   UFUNCTION(BlueprintCallable)
   void ShowDialogCustomLines(TArray<FDialogData> linesToDisplay, EDialogBoxCloseCase dialogSource);

   /**Allows us to add the HUD which shows a detailed view of an item.  Didn't overload AddHUD because parameter prevents implcit uint8 conversion
    * @param itemID - ID of the item to show a detailed view of
    */
   void AddItemExamineHUD(int itemID);

   /**Adds a confirmation box that can do something once the confirmation button is pressed
    * @param funcName - UFUNCTION name that is called on the UObject funcObject
    * @param funcObject - Pointer to UObject member function that will be called after the confirmation
    * @param newTitle - Title of the confirmation box
    * @param newDesc - Description of the confirmation box
    */
   void ShowConfirmationBox(FName funcName = "", UObject* funcObject = nullptr, FText newTitle = FText::GetEmpty(), FText newDesc = FText::GetEmpty());

   /**Adds an input box that can do something with the input once the confirmation button is pressed
    * @param funcObject - Pointer to UObject member function that will be called after the confirmation
    */
   void ShowInputBox(FName funcName = "", UObject* funcObject = nullptr, FText newTitle = FText::GetEmpty(), FText newDesc = FText::GetEmpty());

   /**Toggle a hud on the screen on/off.  BP_Version.  Do not call with huds that require open parameters, instead call their respective AddHUD function.*/
   UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "HUD Toggle", meta = (DisplayName = "Add HUD"))
   void         BP_AddHUD(uint8 newState);
   virtual void BP_AddHUD_Implementation(uint8 newState) { AddHUD(newState); }

   /**Add dialog HUD by passing in a conversation name*/
   UFUNCTION(BlueprintCallable, Category = "HUD Toggle", meta = (DisplayName = "Add Hud Dialog with Topic"))
   void BP_AddHUDDialog(FName conversationName, EDialogBoxCloseCase dialogSource) { ShowDialogWithSource(conversationName, dialogSource); }

   /**Add dialog HUD by passing in dialogLines rather than reading off dialogTable*/
   UFUNCTION(BlueprintCallable, Category = "HUD Toggle", meta = (DisplayName = "Add Hud Dialog with Dialog Lines"))
   void BP_AddHUDDialogString(TArray<FDialogData> linesToDisplay, EDialogBoxCloseCase dialogSource) { ShowDialogCustomLines(linesToDisplay, dialogSource); }

   /**Add the confirmationbox HUD by passing in the callback*/
   UFUNCTION(BlueprintCallable, Category = "HUD Toggle", meta = (DisplayName = "Add ConfirmationBox HUD", AutoCreateRefTerm = "newTitle,newDesc"))
   void BP_AddConfirmationBox(const FText& newTitle, const FText& newDesc, FName funcName = "", UObject* funcObject = nullptr)
   {
      ShowConfirmationBox(funcName, funcObject, newTitle, newDesc);
   }

   /**Add the inputbox HUD by passing in the callback*/
   UFUNCTION(BlueprintCallable, Category = "HUD Toggle", meta = (DisplayName = "Add InputBox HUD", AutoCreateRefTerm = "newTitle,newDesc"))
   void BP_AddInputBox(FText newTitle, const FText& newDesc, FName funcName = "", UObject* funcObject = nullptr)
   {
      ShowInputBox(funcName, funcObject, newTitle, newDesc);
   }

   UFUNCTION(BlueprintCallable, Category = "HUD Toggle")
   bool IsWidgetOnScreen(HUDs hudToCheck) const { return currentlyDisplayedWidgetsBitSet[static_cast<int>(hudToCheck)]; }

   UPROPERTY(EditDefaultsOnly)
   TSubclassOf<UMainWidget> mainMenuClass;

   UPROPERTY(EditDefaultsOnly)
   TSubclassOf<UMainWidget> startMenuClass;

#pragma region accessors

   ///--Accessors--///
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   URTSIngameWidget* GetIngameHUD() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UCharacterMenu* GetCharacterHUD();

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UEquipmentMenu* GetEquipHUD() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UHeroInventory* GetInventoryHUD() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UStoreInventory* GetShopHUD() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UInventory* GetStorageHUD() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UActionbarInterface* GetActionHUD() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UQuestList* GetQuestList() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UQuestJournal* GetQuestJournal() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UMinimap* GetMinimap() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UDialogBox* GetDialogBox() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UDialogUI* GetSocialWindow() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UBreakMenu* GetBreakMenu() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   USettingsMenu* GetSettingsMenu() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UItemExamineWidget* GetExamineMenu() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UConfirmationBox* GetConfirmationBox() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   URTSInputBox* GetInputBox() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UStartMenu* GetStartMenu() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   USpellbookHUD* GetSpellBookMenu() const;
#pragma endregion
};
