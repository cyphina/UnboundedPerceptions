// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "GameplayTagContainer.h"
#include "EventSystem/Trigger.h"
#include "Quest.generated.h"

struct FMyItem;
class UQuestManager;
class UQuestListSlot;

/**
 * All about quests!
 */

///---ENUMS---
/**State of a subgoal*/
UENUM(BlueprintType)
enum class EGoalState : uint8
{
	currentGoal,
	completedGoal,
	failedGoal
};

/**What task we must perform to complete a goal*/
UENUM(BlueprintType)
enum class EGoalType : uint8
{
	/**Needs a trigger to complete/update
	 * Includes:
	 * Use a certain item on something
	 * Complete a puzzle (can be a puzzle activated by an interactable like an arcade machine)
	 * Craft/Combine an item
	 */
	Custom, 
	/**Kill some enemies to complete it*/
	Hunt,
	/**Obtain a certain number of items to complete*/
	Find,
	/**Talk to an NPC about a certain topic to complete*/
	Talk,
	/**Interact with a certain interactable*/
	Interact
};

/**Organize quest types based on importance to story*/
UENUM(BlueprintType)
enum class EQuestCategory : uint8
{
	MainQuest,
	SideQuest,
	Event
};

/**State of quest completion*/
UENUM(BlueprintType)
enum class EQuestState : uint8
{
	currentQuests,
	completedQuests,
	failedQuests
};

///--Structs---
/**Information about goals*/
USTRUCT(BlueprintType, NoExport)
struct FGoalInfo
{
	static const FVector invalidGoalLocation;

	FGoalInfo() : goalType(EGoalType::Custom), goalState(EGoalState::currentGoal), isCustomGoal(false), goalText(FText()), additionalName(FText()), amountToHunt(0), shouldUpdateQuestDescription(false), updatedDescription(FText()), 
	followingSubGoalIndices(TArray<int>()), shouldUseRadius(false), radius(0), circleColor(FLinearColor::Black), canFailQuest(false), canCompleteQuest(false), goalLocation(FVector()) {} 

	FGoalInfo(EGoalType gT, EGoalState gS, bool isC, FText gText, FText aN, int aTH, bool sUQD, FText uD, TArray<int> fSGI, bool sUR, float r, FLinearColor cC, bool cFQ, bool cCQ, FVector gL) :
		goalType(gT), goalState(gS), isCustomGoal(isC), goalText(gText), additionalName(aN), amountToHunt(aTH), shouldUpdateQuestDescription(sUQD), updatedDescription(uD),
		followingSubGoalIndices(fSGI), shouldUseRadius(sUR), radius(r), circleColor(cC), canFailQuest(cFQ), canCompleteQuest(cCQ), goalLocation(gL) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGoalType						goalType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGoalState						goalState;
	/**Custom goal will have a specific trigger to succeed*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool							isCustomGoal;
	/**Goal's description*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText							goalText;
	/**Name used for various things
	 *If the goalType is hunting, this is the name of one monster to hunt (at least for this goal)
	 *If the goalType is finding, this is the item you should have in your inventory
	 *If the goalType is talking, this is the NPC you need to talk to
	 *If the goalType is custom, we can use this for something else (identifier for this goal so that trigger can complete it)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText							additionalName;
	/**If this is a hunt quest, how many things do we have to hunt*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int								amountToHunt;
	/**Do we update the quest description after this goal is completed?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool							shouldUpdateQuestDescription;
	/**If we update the quest description after this goal is completed, do so here*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText							updatedDescription;
	/**Does this subgoal unlock another?  Stores index of the next goal that should be stored in questinfo*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int>						followingSubGoalIndices; 
	/**Do we have a location tracker on the minimap?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool							shouldUseRadius;
	/**What's the size of this location tracker?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float							radius;
	/**What's the color of the location tracker*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor					circleColor;
	/**If we fail this goal the quest will fail*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool							canFailQuest; 
	/**If we finish this goal the quest will be done*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool							canCompleteQuest; 
	/**Location to spawn the tracker*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector							goalLocation = invalidGoalLocation;

	bool operator==(const FGoalInfo& otherGoal) const
	{
		return (goalText.EqualTo(otherGoal.goalText) && additionalName.EqualTo(otherGoal.additionalName));
	}
};
//struct for possible quest rewards
USTRUCT(BlueprintType, NoExport)
struct FQuestReward
{
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int								gold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int								exp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMyItem>				items;
};

//Struct for quest properties (that may be replicated?)
USTRUCT(BlueprintType, NoExport)
struct FQuestInfo
{
	/**Unique quest id*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag					id;
	/**Displayed name of the quest.  We can have quests with the same 
	 *display name which we can troll around with or something*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText							name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText							desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int								suggestedLvl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuestReward					questReward;
	/**Difficulty from 1 (easy) to 10 (extremely hard)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float							difficulty;
	/**What kind of quest is this?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestCategory					questCategory;
	/**A list of all the goals in this quest information*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGoalInfo>				subgoals; 
};

UCLASS(Blueprintable) //actor info has replication details incase we ever go multiplayer
class MYPROJECT_API AQuest : public AInfo
{
	GENERATED_BODY()

	/**index of what goals are currently in progress*/
	UPROPERTY(BlueprintReadOnly, Category = "Bookkeeping", meta = (AllowPrivateAccess = true))
	TArray<int>						currentGoalIndices; 

	/**indices that we start off at when starting the quest*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Bookkeeping", meta = (AllowPrivateAccess = true))
	TArray<int>						startingGoalIndices; 

	/**completed quest goals*/
	UPROPERTY(BlueprintReadOnly, Category = "Bookkeeping", meta = (AllowPrivateAccess = true))
	TArray<FGoalInfo>				completedGoals;

	/**current state of the quest e.g. "Completed"*/
	UPROPERTY(BlueprintReadOnly, Category = "Bookkeeping", meta = (AllowPrivateAccess = true))
	EQuestState						currentState;

	/**current description as seen in quest journal*/
	UPROPERTY(BlueprintReadOnly, Category = "Bookkeeping", meta = (AllowPrivateAccess = true))
	FText							currentDescription;

	/**reference to quest manager*/
	UPROPERTY(BlueprintReadOnly, Category = "Bookkeeping", meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
	UQuestManager*					questManagerRef;
	
	/**Updates currentGoals to match that of the currentGoalIndices*/
	void							UpdateSubGoals();

public:
	/**goals currently in progress*/
	UPROPERTY(BlueprintReadOnly, Category = "Bookkeeping")
	TArray<FGoalInfo>				currentGoals;

	/**map of index of hunt goal and amount needed to hunt*/
	UPROPERTY(BlueprintReadOnly, Category = "Bookkeeping", meta = (AllowPrivateAccess = true))
	TMap<int,int>					currentHuntedAmounts;

	/**information about quest*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Information")
	FQuestInfo						questInfo; 

	UFUNCTION()
	void							BeginPlay() override;

	/**Accessor to current goal indices, that is the index of the goal within the quest (default value)*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessor")
	TArray<int>						GetCurrentGoalIndices() const { return currentGoalIndices; }

	/**Accesor to goals*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessor")
	FGoalInfo						GetGoalAtIndex(int goalIndex);

	/**Quest state accessor for BP*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessor")
	EQuestState						GetQuestState() const { return currentState; }

	/**Initiates completing a subgoal*/
	UFUNCTION(BlueprintCallable, Category = "Updating")
	void							CompleteSubGoal(int goalIndex, bool fail); //transition between next goal

	/**Trigger to activate on quest finished.
	UPROPERTY(EditDefaultsOnly, Category = "Updating")
	FTriggerData					onQuestCompletedTrigger; 
	*/

	/**Initiates completing the quest*/
	UFUNCTION(BlueprintNativeEvent, Category = "Updating")
	void							CompleteQuest(bool fail); //finish up a quest!
	void							CompleteQuest_Implementation(bool fail);

	/**called when starting or restarting a quest, setup what goals will be first*/
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void							SetupStartingGoals(); 

	/**This function is here since we can't have functions nor static const uproperties in structs=*/ 
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Constants") 
	FVector							getInvalidVector() const { return FGoalInfo::invalidGoalLocation; } 

	/**we need this for our deferred spawn in QuestManager::AddNewQuest()*/
	void							SetQuestManagerRef(UQuestManager* managerRef) { if (!questManagerRef) questManagerRef = managerRef; } 
};
