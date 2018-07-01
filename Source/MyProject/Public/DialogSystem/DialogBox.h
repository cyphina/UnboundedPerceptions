/*Dialog box.  Part of DialogUI*/

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "DialogManager.h"
#include "EventSystem/Trigger.h"
#include "DialogBox.generated.h"

/*A dialog box that can be used anywhere for conversation... or just displaying thoughts
 * When loading up the DialogHUD, the dialog should start with regular text else there will be trouble displaying wrong things like
 * triggers/conditionals/choices.  
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogEnded);

class ANPC;
class ARTSGameMode;

UCLASS()
class MYPROJECT_API UDialogBox : public UMyUserWidget
{
	GENERATED_BODY()

	/**
	 * The dialog information gotten from loading the conversation (as denoted by the conversationTag) from the table
	 */
	TArray<FDialogData>					dialogLines; 


	/**
	 * Array used when parsing information in dialog (if the dialogNode is a triggerNode/condition)
	 */
	TArray<FString>						parsingInfo; 

	/**
	 * Currently used to denote the end of dialogline iteration (kind of like nullptr)
	 */
	FDialogData							defaultDialog{ FDialogData() }; 

	/**
	 * id of node we are currently on
	 */
	int									currentNodeNum = 0; 
	
	/**
	 * flag to see if dialog is properly loaded
	 */
	bool								hasValidDialog = false; 

	/**
	 * Needed to map to effect post dialog.  
	 */
	FName								dialogTopic; 

	ARTSGameMode*						gameModeRef; 

	/**
	 *A trigger that can be set to activate when the dialog ends
	 */
	FTriggerData*						onDialogEndTrigger;

	/**If this node leads to a choice node, then we must do some work to setup the data which goes to our choice buttons.  Keeps nodeNum
	 * at the same location since it only changes when a choice button is pressed
	 */
	void								HandleChoices();

	/**If this node leads to a trigger node, then we must parse the trigger data so it can be activated.  Calls GetNextLine() moving the
	 * nodeNUm to the next proper location since trigger nodes don't actually display any data.
	 */
	void								HandleTriggers();		

	/**If this node leads to a condition node, then we must parse the condition data to see if it's true or false.
	 * If the condition is true, then move nodeNum to the first node condition node is linked to, else move nodeNum to index for second node.
	 */
	void								HandleConditions();		

	/**
	 *Call after finishing a conversation
	 */
	void								ResetDialog();

	/** 
	 *Map of <names,image> to dialog display character potrait for speakers
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Information Maps", Meta = (AllowPrivateAccess=true))
	TMap<FName, UTexture2D*>			dialogPortraitMap;

	/**
	 *Only call this in the constructor.
	 *Default values for our maps keep resetting so we can set them in these functions.  Only call these in the constructor 
	 */
	void InitDialogPortraitMaps();	
	
public:
	UDialogBox(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
	FOnDialogEnded						OnDialogEnded;

	void								Construct_Implementation() override;
	bool								OnWidgetAddToViewport_Implementation() override;

	/** Used primarily to get current dialog line when first dialog is loaded up */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialog Iteration")
	const FDialogData&					GetCurrentLine() const { return dialogLines[currentNodeNum]; }

	/** 
	 *Iterate through dialog lines.  After called, will return the next line to be displayed
	 */
	UFUNCTION(BlueprintCallable, Category = "Dialog Iteration")
	const FDialogData&					GetNextLine();

	/**
	 *Used when added to viewport and when choosing to talk about some topic via SocialWindow
	 *Puts all dialogs with the dialogTopic + # into the dialogLines variable via the DialogManager which
	 *acts as an interface to our dialogTable
	 */
	UFUNCTION(BlueprintCallable, Category = "DialogInformation")
	void								SetConversation(FName dialogTopic);

	/**
	 *Used when added to viewport in the case of some character reflecting upon his/her action (like attempting to open a locked door)
	 *Passes in lines that aren't on the dialogTable.  Reads lines from start to end of the array
	 */
	UFUNCTION(BlueprintCallable, Category = "DialogInformation")
	void								SetDialogLines(TArray<FDialogData> newDialogLines);

	/** Accessor function to get dialog portrait from map of Actor -> Portrait*/
	UFUNCTION(BlueprintCallable, Category = "DialogInformation")
	UTexture2D*							GetDialogPortrait(FName actorName) const { return dialogPortraitMap.Contains(actorName) ? dialogPortraitMap[actorName] : dialogPortraitMap["default"]; }
	
	/** Accessor function to set dialog portraits*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "DialogInformation")
	void								SetDialogPortraits(FName actor1Name, FName actor2Name); 

	/** Call this function to set a trigger when the dialog finishes*/
	void								SetOnDialogFinishedTrigger(FTriggerData* endTrigger) { onDialogEndTrigger = endTrigger; }
	
	UFUNCTION(BlueprintCallable, Category = "DialogInformation")
	FName								GetDialogTopic() const { return dialogTopic; }
	
	/** display buttons so player can pick something to say and setup the next node index each button should return when clicked */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Choices")
	void								ShowChoices(const TArray<FDialogData>& choiceLines);

	/**Picks a response option and returns next dialog node to display after a choice is picked and triggers/conditions associated with that choice are activated
	 * @param choice - Index of the node that the picked node leads to
	 */
	UFUNCTION(BlueprintCallable, Category = "Choices")
	const FDialogData&					PickChoice(int choice);
};
