/*Dialog box.  Part of DialogUI*/

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "DialogManager.h"
#include "EventSystem/Trigger.h"
#include "DialogBox.generated.h"

/**The type of event that caused the dialog box to open*/
UENUM(BlueprintType)
enum class EDialogSource : uint8 { npc, conversation, trigger, none };

class ANPC;
class AUserInput;
class ARTSGameMode;

/*A dialog box that can be used anywhere for conversation... or just displaying thoughts
 * When loading up the DialogHUD, the dialog should start with regular text else there will be trouble displaying things like
 * triggers/conditionals/choices.
 */
UCLASS()
class MYPROJECT_API UDialogBox : public UMyUserWidget
{
   GENERATED_BODY()

   AUserInput*   controllerRef;
   ARTSGameMode* gameModeRef;

   /**
    * The dialog information gotten from loading the conversation (as denoted by the conversationTag) from the table
    */
   TArray<FDialogData> dialogLines;

   /**
    * Array used when parsing information in dialog (if the dialogNode is a triggerNode/condition)
    */
   TArray<FString> parsingInfo;

   /**
    * Currently used to denote the end of dialogline iteration (kind of like nullptr)
    */
   FDialogData defaultDialog{FDialogData()};

   /**
    * id of node we are currently on
    */
   int currentNodeNum = 0;

   /**
    * flag to see if dialog is properly loaded
    */
   bool hasValidDialog = false;

   /**
    * Needed to map to the dialogWheel post dialog.
    */
   FName dialogTopic;

   EDialogSource dialogSource = EDialogSource::none;

   /**If this node leads to a choice node, then we must do some work to setup the data which goes to our choice buttons.  Keeps nodeNum
    * at the same location since it only changes when a choice button is pressed
    */
   void HandleChoices();

   /**If this node leads to a trigger node, then we must parse the trigger data so it can be activated.  Calls GetNextLine() moving the
    * nodeNUm to the next proper location since trigger nodes don't actually display any data.
    */
   const FDialogData& HandleTriggers();

   /**If this node leads to a condition node, then we must parse the condition data to see if it's true or false.
    * If the condition is true, then move nodeNum to the first node condition node is linked to, else move nodeNum to index for second node.
    */
   const FDialogData& HandleConditions();

   /**
    *Call after finishing a conversation
    */
   void ResetDialog();

   /**
    *Only call this in the constructor.
    *Default values for our maps keep resetting so we can set them in these functions.  Only call these in the constructor
    */
   void InitDialogPortraitMaps();

 public:
   UDialogBox(const FObjectInitializer& ObjectInitializer);

   void NativeConstruct() override;
   bool OnWidgetAddToViewport_Implementation() override;

   /** Used primarily to get current dialog line when first dialog is loaded up */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialog Iteration")
   const FDialogData& GetCurrentLine() const { return dialogLines[currentNodeNum]; }

   /**
    *Iterate through dialog lines.  After called, will return the next line to be displayed
    */
   UFUNCTION(BlueprintCallable, Category = "Dialog Iteration")
   const FDialogData& GetNextLine();

   /**
    *Used when added to viewport and when choosing to talk about some topic via SocialWindow
    *Puts all dialogs with the dialogTopic + # into the dialogLines variable via the DialogManager which
    *acts as an interface to our dialogTable
    */
   UFUNCTION(BlueprintCallable, Category = "DialogInformation")
   void SetConversation(FName dialogTopic);

   /**
    *Used when added to viewport in the case of some character reflecting upon his/her action (like attempting to open a locked door)
    *Passes in lines that aren't on the dialogTable.  Reads lines from start to end of the array
    */
   UFUNCTION(BlueprintCallable, Category = "DialogInformation")
   void SetDialogLines(TArray<FDialogData> newDialogLines);

   /**When this dialogBox is opened, we need to tell it what kind of event opened it so it respond properly when it's closed*/
   void SetDialogSource(EDialogSource newDialogSource) { dialogSource = newDialogSource; }

   UFUNCTION(BlueprintCallable, Category = "DialogInformation")
   FName GetDialogTopic() const { return dialogTopic; }

   /** display buttons so player can pick something to say and setup the next node index each button should return when clicked */
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Choices")
   void ShowChoices(const TArray<FDialogData>& choiceLines);

   /**Picks a response option and returns next dialog node to display after a choice is picked and triggers/conditions associated with that choice are activated
    * @param choice - Index of the choice that is picked (0,1,2,...up to num choices)
    */
   UFUNCTION(BlueprintCallable, Category = "Choices")
   const FDialogData& PickChoice(int choice);
};
