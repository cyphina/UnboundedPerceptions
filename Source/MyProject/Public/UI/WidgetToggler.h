// Created 9/6/20 5:27 PM

#pragma once
#include "HUDTypes.h"
#include "DialogStructs.h"
#include "WidgetToggler.generated.h"

enum class EDialogBoxCloseCase : uint8;

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UWidgetToggler : public UInterface
{
   GENERATED_BODY()
};

class IWidgetToggler
{
   GENERATED_BODY()

public:
   /**Toggle a hud on the screen on/off.  C++ version.*/
   virtual void AddHUD(uint8 newState) = 0;

   /**
    * @brief Allows us to quickly add dialogBox loaded up with conversation.  Else we would have to set the dialog manually, then call AddHUD.
    * The source may setup a trigger when we finish conversation which may lead to something else.
    * @param conversationName - Name of the conversation.  Leave blank to just close
    * @param dialogSource - What event caused this dialogBox to open?
    */
   virtual void ShowDialogWithSource(FName conversationName, EDialogBoxCloseCase dialogSource) = 0;

   /**Variant of AddHUDDialog for text that not in the convesrsation table. Sometimes has an interacting hero associated with it which should be set before the call to this
    * @param linesToDisplay - DialogInformation to pass in
    * @param dialogSource - What event caused this dialogBox to open?
    */
   UFUNCTION(BlueprintCallable)
   virtual void ShowDialogCustomLines(TArray<FDialogData> linesToDisplay, EDialogBoxCloseCase dialogSource) = 0;

   /**Adds a confirmation box that can do something once the confirmation button is pressed
    * @param funcName - Name of the UFUNCTION we want to call (called via reflection)
    * @param funcObject - Pointer to UObject instance that has te UFUNCTION we want to call
    * @param newTitle - Title of the confirmation box
    * @param newDesc - Description of the confirmation box
    */
   virtual void ShowConfirmationBox(FName funcName = "", UObject* funcObject = nullptr, FText newTitle = FText::GetEmpty(), FText newDesc = FText::GetEmpty()) = 0;

   /**Adds an input box that can do something with the input once the confirmation button is pressed
    * @param funcName - Name of the UFUNCTION we want to call (called via reflection)
    * @param funcObject - Pointer to UObject instance that has te UFUNCTION we want to call
    */
   virtual void ShowInputBox(FName funcName = "", UObject* funcObject = nullptr, FText newTitle = FText::GetEmpty(), FText newDesc = FText::GetEmpty()) = 0;

   /**Toggle a hud on the screen on/off.  BP_Version.  Do not call with huds that require open parameters, instead call their respective AddHUD function.*/
   UFUNCTION(BlueprintCallable, Category = "HUD Toggle", meta = (DisplayName = "Add HUD"))
   virtual void BP_AddHUD(uint8 newState) = 0;

   /**Add dialog HUD by passing in a conversation name*/
   UFUNCTION(BlueprintCallable, Category = "HUD Toggle", meta = (DisplayName = "Add Hud Dialog with Topic"))
   virtual void BP_AddHUDDialog(FName conversationName, EDialogBoxCloseCase dialogSource) = 0;

   /**Add dialog HUD by passing in dialogLines rather than reading off dialogTable*/
   UFUNCTION(BlueprintCallable, Category = "HUD Toggle", meta = (DisplayName = "Add Hud Dialog with Dialog Lines"))
   virtual void BP_AddHUDDialogString(TArray<FDialogData> linesToDisplay, EDialogBoxCloseCase dialogSource) = 0;

   /**Add the confirmation box HUD by passing in a callback*/
   UFUNCTION(BlueprintCallable, Category = "HUD Toggle", meta = (DisplayName = "Add ConfirmationBox HUD", AutoCreateRefTerm = "newTitle,newDesc"))
   virtual void BP_AddConfirmationBox(const FText& newTitle, const FText& newDesc, FName funcName = "", UObject* funcObject = nullptr) = 0;

   /**Add the inputbox HUD by passing in the callback*/
   UFUNCTION(BlueprintCallable, Category = "HUD Toggle", meta = (DisplayName = "Add InputBox HUD", AutoCreateRefTerm = "newTitle,newDesc"))
   virtual void BP_AddInputBox(FText newTitle, const FText& newDesc, FName funcName = "", UObject* funcObject = nullptr) = 0;

   UFUNCTION(BlueprintCallable, Category = "HUD Toggle")
   virtual bool IsWidgetOnScreen(EHUDs hudToCheck) const = 0;
};
