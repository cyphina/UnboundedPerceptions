#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RTSConditional.generated.h"

/**
 *The conditional is like a function object that returns a bool
 */

class UConditionalManager;

struct FTriggerData;
UENUM(BlueprintType)
enum class EConditionalType : uint8
{
	/** Doesn't do anything */
	None, 
	/** Checks to see if the quest named (value1 = the id without parent tag) is being done currently*/
	HasQuestCond,
	/** Checks for when a certain quest is completed.  Value1 = (Quest GameplayTag ID (without parent tag appended))*/
	QuestCompletionCond,
	/**	Checks for when an item is owned in the inventory.  Value1 = (Item Tag ID)*/
	OwnsItemCond,
	/** Talked to NPC about this topic.  Value1 = (NPC NameTag w/o parent tag), Value2 = (Conversation Topic ID w/o parent tag)*/
	HadConversationCond,
	/** A condition where (value1) is passed in dynamically to essentially make this condition a boolean wrapper.  
	 * Set Value1 to != 0 to make it true, and set it to 0 to make it false*/
	CustomCond
};

USTRUCT(BlueprintType, NoExport)
struct FConditionData  
{
	FConditionData() = default;

	static FConditionData							defaultCond; //not const so can be passed through trigger functionality but should never be modified

	/**Type of the trigger*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	EConditionalType								conditionalType = EConditionalType::None;
	
	/**Reverse the result (if you want the opposite result of the condition*/
	UPROPERTY(EditAnywhere, Category = "Properties")
	bool											reverseResult = false;

	/**String values that denote the parameters of the condition*/
	UPROPERTY(EditAnywhere, Category = "Parameters")
	TArray<FString>									conditionalValues = TArray<FString>();

};

class AUserInput;
class ARTSGameMode;

UCLASS(Blueprintable)
class MYPROJECT_API UConditionalManager : public UObject
{
	GENERATED_BODY()

	AUserInput*						cpcRef = nullptr;
	ARTSGameMode*					gameModeRef = nullptr;

public:
	void							Init();

	/**Returns a boolean representing the condition of the condition data*/
	UFUNCTION(BlueprintCallable, Category = "ConditionalActivation")
	bool							GetCondition(UPARAM(ref) FConditionData& condData) const;
	/**Returns a basic string representing what needs to be done to satisfy this condition*/
	FText							GetConditionString(TArray<FConditionData> conditions) const;

private:

	UFUNCTION(BlueprintCallable, Category = "ConditionMessage")
	FText							GetConditionMessage(FConditionData& condData) const;

	/**Look through questManager completed quest list to see if there's a quest is in there with the questTag*/
	bool							GetQuestConditionVal(FConditionData& condData) const;
	/**Look through inventory of current heroes to see if there's an item with the itemName passed*/
	bool							GetOwnedItemConditionVal(FConditionData& condData) const;
	/**Look through NPC's conversation record to see if there was a conversation had with the name passed*/
	bool							GetHadConversationConditionVal(FConditionData& condData) const;
	/*Look through questManager current quests for quest with this name*/
	bool							GetHasCurrentQuestConditionVal(FConditionData& condData) const;
};