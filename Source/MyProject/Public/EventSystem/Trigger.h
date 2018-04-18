// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Trigger.generated.h"

/**
 * Class for creating an event that changes only a few objects at a time
 */

class AUserInput;
class UMyGameInstance;

UENUM(BlueprintType)
enum class TriggerType : uint8
{
	None, /** Doesn't do anything */
	ChangeDialog, /** Given an NPC (object 1), changes dialog of topic (value 2 (is a fully qualified gameplaytag name)) to dialog (value 3).  If (value3) empty, then changes default dialog to (value2)*/
	ModifyStats, /** For each object, assumes object is a unit.  Changes Stat (data N) (ordered numerically by stats, skills, vitals, mechanics) by adding (data N+1) to it*/
	OpenHUDTrigger, /** Takes one TriggerValue denoting the hud to be opened enum value corresponding int value.  Look at HUDManager for HUDs enum and corresponding int values */
	ChangePartyTrigger, /** Takes up to four trigger objects denoting the new party members in their new order*/
	ActivateOtherTrigger, /** Activate another trigger (object 1) */
	DeactivateOtherTrigger, /* Deactivate another trigger (object 1) */
	ChangeTriggerType, /* Changes trigger (object 1) to type (data 1)*/
	AddQuestTrigger, /*Add quest from questlistmap with key (value1), and if (value2) is not 0, it will start automatically*/
};

USTRUCT(BlueprintType, NoExport)
struct FTriggerData 
{
	FTriggerData();

	static FTriggerData defaultTrigger; //not const so can be passed through trigger functionality but should never be modified

	//Can this trigger be activated?  Can be toggled on and off by other triggers
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties")
	bool										enabled = true;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	TriggerType									triggerType = TriggerType::None;
	//Number of times this trigger can activate.  Set to -1 to be able to call this infinately
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	int											numCalls = 1;
	//Hard pointers load assets upon creations
	UPROPERTY(EditAnywhere, Category = "Parameters")
	TArray<FString>								triggerObjects = TArray<FString>();
	UPROPERTY(EditAnywhere, Category = "Parameters")
	TArray<FString>								triggerValues = TArray<FString>();
};

UCLASS(Blueprintable)
class MYPROJECT_API UTriggerManager : public UObject
{
	GENERATED_BODY()

	UTriggerManager() {}
	~UTriggerManager() {}

	/** record for triggers so they can be stored across levels */
	TMap<FName, FTriggerData>		triggerRecords;

public:

	AUserInput*						cpcRef;
	UMyGameInstance*				gameInstanceRef;

	UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Managers")
	TMap<FName, FTriggerData>		GetTriggerRecords() const { return triggerRecords; }

	/**Activates a trigger given a trigger data.  If the objects referred to are not loaded, then we must store the trigger somewhere
	and the object will check for any triggers performed on it when it is loaded 
	@param triggerData - Reference to triggerData incase we need to care about the # times activated and such
	*/
	UFUNCTION(BlueprintCallable, Category = "TriggerLibrary")
	void									ActivateTrigger(UPARAM(ref) FTriggerData& triggerData);

private:

	void									ChangeDialog(const FTriggerData& tdata);
	void									ModifyStats(const FTriggerData& tdata);
	void									OpenHUDTrigger(const FTriggerData& tdata);
	void									ChangePartyTrigger(const FTriggerData& tdata);
	void									ActivateOtherTrigger(const FTriggerData& tdata);
	void									DeactivateOtherTrigger(const FTriggerData& tdata);
	void									ChangeTriggerType(const FTriggerData& tdata);
	void									AddQuestTrigger(const FTriggerData& tdata);
	void									TriggerEffect(FTriggerData& tdata);

};
