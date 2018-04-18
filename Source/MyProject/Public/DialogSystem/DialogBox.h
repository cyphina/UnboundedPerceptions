/*Dialog box.  Part of DialogUI*/

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "DialogGraph.h"
#include "DialogManager.h"
#include "EventSystem/Trigger.h"
#include "DialogBox.generated.h"

/*A dialog box that can be used anywhere for conversation... or just displaying thoughts*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogEnded);

class ANPC;
class UMyGameInstance;

UCLASS()
class MYPROJECT_API UDialogBox : public UMyUserWidget
{
	GENERATED_BODY()

	TArray<FDialogData>					dialogLines; //the dialog information gotten from loading the conversation from the table
	TArray<FString>						triggerInfo; //array used when parsing trigger information in dialog
	FDialogData							defaultDialog{ FDialogData() }; //used to denote the end of dialogline iteration
	int									currentNodeNum = 0; //id of node we are currently on
	
	bool								hasValidDialog = false; //flag to see if dialog is properly loaded
	FName								dialogTopic; //needed to map to effect post dialog 
	UMyGameInstance*					gameInstanceRef; 
	FTriggerData*						onDialogEndTrigger;
	//call after finishing a conversation
	void								ResetDialog();

	/** Map of names to dialog display character potrait*/
	UPROPERTY(EditDefaultsOnly, Category = "Information Maps", Meta = (AllowPrivateAccess=true))
	TMap<FName, UTexture2D*>			dialogPortraitMap;

	///<summary> Default values for our maps keep resetting so we can set them in these functions.  Only call these in the constructor </summary>///
	void InitDialogPortraitMaps();	//Only call this in the constructor
	
public:
	UDialogBox(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
	FOnDialogEnded						OnDialogEnded;

	void								Construct_Implementation() override;
	bool								OnWidgetAddToViewport_Implementation() override;

	/** Used primarily to get current dialog line when first dialog is loaded up */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialog Iteration")
	const FDialogData&					GetCurrentLine() const { return dialogLines[currentNodeNum]; }

	/** Iterate through dialog lines */
	UFUNCTION(BlueprintCallable, Category = "Dialog Iteration")
	const FDialogData&					GetNextLine();

	/**Used when added to viewport and when choosing to talk about some topic via SocialWindow */
	UFUNCTION(BlueprintCallable, Category = "DialogInformation")
	void								SetConversation(FName dialogTopic);

	/** Access function to get dialog portrait from map*/
	UFUNCTION(BlueprintCallable, Category = "DialogInformation")
	UTexture2D*							GetDialogPortrait(FName actorName) const { return dialogPortraitMap.Contains(actorName) ? dialogPortraitMap[actorName] : dialogPortraitMap["default"]; }
	
	/** Call this function to end the trigger when the dialog finishes*/
	void								SetOnDialogFinishedTrigger(FTriggerData* endTrigger) { onDialogEndTrigger = endTrigger; }
	
	UFUNCTION(BlueprintCallable, Category = "DialogInformation")
	FName								GetDialogTopic() const { return dialogTopic; }
	
	/** display buttons so player can pick something to say and setup the next node index each button should return when clicked */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Choices")
	void								ShowChoices(const TArray<FDialogData>& choices);

	UFUNCTION(BlueprintCallable, Category = "Choices")
	void								PickChoice(int choice) { currentNodeNum = choice; };

			
};
