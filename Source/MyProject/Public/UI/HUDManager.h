// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Info.h"
#include "HUDProvider.h"
#include "WidgetToggler.h"

#include "HUDTypes.h"
#include "UserWidgetExtensions/AnimHudWidget.h"
#include "UserWidgetExtensions/MyUserWidget.h"
#include "EventSystem/Trigger.h"
#include "DialogStructs.h"
#include "HUDManager.generated.h"

class ARTSGameMode;
class AUserInput;
class UMainWidget;
class AShopNPC;
class UDIRender;

/**
 * @brief Provides functionality to toggle HUDs on and off, and provides a place to get references to all the HUDs
 */
UCLASS(Blueprintable)
class MYPROJECT_API AHUDManager : public AInfo, public IHUDProvider, public IWidgetToggler
{
   GENERATED_BODY()

 public:
   AHUDManager();

   virtual void BeginPlay() override;

 public:
   void AddHUD(uint8 newState) override;

   void ShowDialogWithSource(FName conversationName, EDialogBoxCloseCase dialogSource) override;
   void ShowDialogCustomLines(TArray<FDialogData> linesToDisplay, EDialogBoxCloseCase dialogSource) override;

   void ShowConfirmationBox(FName funcName = "", UObject* funcObject = nullptr, FText newTitle = FText::GetEmpty(), FText newDesc = FText::GetEmpty()) override;
   void ShowInputBox(FName funcName = "", UObject* funcObject = nullptr, FText newTitle = FText::GetEmpty(), FText newDesc = FText::GetEmpty()) override;

   void BP_AddHUD(uint8 newState) override { AddHUD(newState); }
   void BP_AddHUDDialog(FName conversationName, EDialogBoxCloseCase dialogSource) override { ShowDialogWithSource(conversationName, dialogSource); }
   void BP_AddHUDDialogString(TArray<FDialogData> linesToDisplay, EDialogBoxCloseCase dialogSource) override { ShowDialogCustomLines(linesToDisplay, dialogSource); }
   
   FORCEINLINE void BP_AddConfirmationBox(const FText& newTitle, const FText& newDesc, FName funcName = "", UObject* funcObject = nullptr) override
   {
      ShowConfirmationBox(funcName, funcObject, newTitle, newDesc);
   }
   FORCEINLINE void BP_AddInputBox(FText newTitle, const FText& newDesc, FName funcName = "", UObject* funcObject = nullptr) override
   {
      ShowInputBox(funcName, funcObject, newTitle, newDesc);
   }

   bool IsWidgetOnScreen(EHUDs hudToCheck) const override final { return currentlyDisplayedWidgetsBitSet[static_cast<int>(hudToCheck)]; }

 public:
   URTSIngameWidget* GetIngameHUD() const override;
   UBreakMenu*       GetBreakMenu() const override;
   USettingsMenu*    GetSettingsMenu() const override;
   UConfirmationBox* GetConfirmationBox() const override;
   URTSInputBox*     GetInputBox() const override;
   UStartMenu*       GetStartMenu() const override;

 public:
   /**
   * @brief Class used to show the damage numbers in world space
   */
   UPROPERTY(EditDefaultsOnly)
   TSubclassOf<UDIRender> damageIndicatorClass;

 protected:
   UPROPERTY(EditDefaultsOnly)
   TSubclassOf<UMainWidget> mainMenuClass;

   UPROPERTY(EditDefaultsOnly)
   TSubclassOf<UStartMenu> startMenuClass;

   UPROPERTY(BlueprintReadOnly)
   UStartMenu* startMenu;

   UPROPERTY(BlueprintReadOnly)
   UMainWidget* mainWidget;

   /**
    * @brief Stores references to all the widgets for easy on/off toggling via Enum indexing
    */
   UPROPERTY(BlueprintReadWrite, Category = "Widgets")
   TArray<UMyUserWidget*> widgetReferences;

   /**
    * @brief Number of widgets that are blocking (we can't perform actions outside the UI when these are open)
    */
   UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Widgets")
   int numWidgetsBlocking;

 private:
   /** Setup widgets in our reference storage array to toggle them on and off later on through AddHUD*/
   void SetWidget(UMyUserWidget* widgetRef, EHUDs newState);

   void SetupWidgetReferences();

   /**
   * @brief Applies a hud to the screen: returns true if successful, false otherwise.
   * Flip flops - If we have HUD already applied, we turn off
   * @param newState - The EHUD value corresponding to this HUD.
   * @param enableClickEvents - Can we click (for in-game interactions) with this hud open?
   * @param canOpenCombat - Can this HUD be opened during combat?  If not, then don't let the player perform regular actions when it's open by switching to the UI cursor
   * @param bBlocking - Can other MyUserWidgets be opened while this one is open?
   */
   bool ApplyHUD(uint8 newState, bool enableClickEvents, bool canOpenCombat, bool bBlocking = false);

   /**
   * @brief Helper function to update tracking of widgets on screen or widgets blocking actions
   * @param removeIndex  - Identifies what widget to remove
   * @param enableClickEvents - Can we click (for in-game interactions) while this widget is open?
   * @param canOpenCombat - Is this widget considered blocking?
   */
   void UpdateWidgetTracking(int removeIndex, bool enableClickEvents, bool canOpenCombat);

   private:
   /**
   * @brief Uses UE4 reflection to inject this HUDManager into any property named hudManagerRef. Allows us to inject into private members
   * @param objectToInject - The UObject with a property called "hudManagerRef"
   */
   FORCEINLINE void InjectDependency(UObject* objectToInject)
   {
      FObjectProperty* objectProperty = FindFProperty<FObjectProperty>(objectToInject->GetClass(), "hudManagerRef"); // Find the property on that object
      objectProperty->SetPropertyValue_InContainer(objectToInject, this); // Get that property and set its value in the container (which is the actual object instance)
   }

   void InjectDependentClasses();

   /**
   * @brief Creates the first widget that goes on screen that holds every other widget
   */
   void CreateMainWidget();
   
   bool ShowHiddenWidget(UMyUserWidget* widgetToApply) const;
   void HideWidgetOnScreen(UMyUserWidget* widgetToApply) const;

   /**
    * @brief Number of huds we have total. As long as we add new HUD types before this ENUM entry it should be properly updated
    */
   static const int HUDCount = static_cast<uint8>(EHUDs::HS_Count);

   ARTSGameMode* gameMode;
   AUserInput*   playerControllerRef;

   /**
    * @brief Widgets that are on screen
    */
   static TBitArray<FDefaultBitArrayAllocator> currentlyDisplayedWidgetsBitSet;

   int  showMouseCursorCount   = 0;     // Counter to keep track of how many huds are on screen that want us to show the special hud cursor
   int  enableClickEventsCount = 0;     // Counter to keep track of how many huds are on screen that disable in-game click events
   bool bBlocked               = false; // Can we open any other widgets right now or are we blocked?
};
