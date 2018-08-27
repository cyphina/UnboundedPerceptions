// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "GameplayTagContainer.h"
#include "EventSystem/Trigger.h"
#include "Items/Item.h"
#include "Quest.generated.h"

class UQuestManager;
class UQuestListSlot;
class UNamedInteractableDecorator;

/**
 * All about quests!
 */

///---ENUMS---
/**State of a subgoal*/
UENUM(BlueprintType)
enum class EGoalState : uint8
{
	/**Currently being partaken*/
	currentGoal,
	/**Need to finish other goals first*/
	lockedGoal,
	/**Finished the goal*/
	completedGoal,
	/**Goal unsucessful*/
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

	FGoalInfo() : goalType(EGoalType::Custom), goalState(EGoalState::lockedGoal), isCustomGoal(false), goalText(FText()), additionalNames(TArray<FText>()), amount(0), shouldUpdateQuestDescription(false), updatedDescription(FText()), 
	requiredSubGoalIndices(TArray<int>()), shouldUseRadius(false), radius(0), circleColor(FLinearColor::Black), canFailQuest(false), canCompleteQuest(false), goalLocation(FVector()) {} 

	FGoalInfo(EGoalType gT, EGoalState gS, bool isC, FText gText, TArray<FText> aN, int aTH, bool sUQD, FText uD, TArray<int> fSGI, bool sUR, float r, FLinearColor cC, bool cFQ, bool cCQ, FVector gL) :
		goalType(gT), goalState(gS), isCustomGoal(isC), goalText(gText), additionalNames(aN), amount(aTH), shouldUpdateQuestDescription(sUQD), updatedDescription(uD),
		requiredSubGoalIndices(fSGI), shouldUseRadius(sUR), radius(r), circleColor(cC), canFailQuest(cFQ), canCompleteQuest(cCQ), goalLocation(gL) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Metadata")
	EGoalType						goalType;
	UPROPERTY(BlueprintReadOnly, Category = "Goal Metadata")
	EGoalState						goalState = EGoalState::lockedGoal;
	/**Custom goal will have a specific trigger to succeed.  It can also be used to give the goal a custom description*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Metadata")
	bool							isCustomGoal;
	/**Goal's description*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Game Data")
	FText							goalText;
	/**Names used for various things
	 *If the goalType is hunting, this is the type name of the monster to hunt 
	 *If the goalType is finding, this is the item you should find (index 0) and then the NPC to turn it into (index 1)
	 *If the goalType is talking, this is the NPC you need to talk to (index 0), and the dialog topic to talk about (index 1)
	 *If the goalType is custom, we can use this for something else (identifier for this goal so that a trigger can complete it)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Game Data")
	TArray<FText>					additionalNames;
	/**If this quest requires some numerical value representing how many (how many to hunt/gather/interact)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Game Data")
	int								amount = 0;
	/**Do we update the quest description after this goal is completed?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Game Data")
	bool							shouldUpdateQuestDescription;
	/**If we update the quest description after this goal is completed, do so here*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Game Data")
	FText							updatedDescription;
	/**What goals need to be completed to unlock this one?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Game Data")
	TArray<int>						requiredSubGoalIndices;
	/**If we fail this goal the quest will fail*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Game Data")
	bool							canFailQuest; 
	/**If we finish this goal the quest will be done*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Game Data")
	bool							canCompleteQuest;
	/**Should any triggers be run when this goal begins?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Game Data")
	TArray<FTriggerData>			prevGoalTriggers;
		/**Should any triggers be run when this goal is over?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Game Data")
	TArray<FTriggerData>			afterGoalTriggers;
	/**Do we have a location tracker on the minimap?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Minimap")
	bool							shouldUseRadius;
	/**What's the size of this location tracker?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Minimap")
	float							radius;
	/**What's the color of the location tracker*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Minimap")
	FLinearColor					circleColor;
	/**Location to spawn the tracker*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Minimap")
	FVector							goalLocation = invalidGoalLocation;

	bool operator==(const FGoalInfo& otherGoal) const
	{
		return goalText.EqualTo(otherGoal.goalText) && goalState == otherGoal.goalState;
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
	TArray<FMyItem>					items;
};

//Struct for quest properties (that may be replicated?)
USTRUCT(BlueprintType, NoExport)
struct FQuestInfo
{
	/**
	 *Unique quest id
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag					id;
	/**Displayed name of the quest.  We can have quests with the same 
	 *display name which we can troll around with or something
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText							name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText							desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int								suggestedLvl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuestReward					questReward;
	/**
	 *Difficulty from 1 (easy) to 10 (extremely hard)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float							difficulty;
	/**What kind of quest is this?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestCategory					questCategory;
	/**
	 *A list of all the goals in this quest information
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGoalInfo>				subgoals; 
};

UCLASS(Blueprintable) //actor info has replication details incase we ever go multiplayer
class MYPROJECT_API AQuest : public AInfo
{
	GENERATED_BODY()

	/**
	 *Index of what goals are currently in progress
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Bookkeeping", meta = (AllowPrivateAccess = true))
	TArray<int>						currentGoalIndices; 

	/**
	 *Completed quest goals
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Bookkeeping", meta = (AllowPrivateAccess = true))
	TArray<FGoalInfo>				completedGoals;

	/**
	 *Current state of the quest e.g. "Completed"
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Bookkeeping", meta = (AllowPrivateAccess = true))
	EQuestState						currentState;

	/**
	 *Current description as seen in quest journal
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Bookkeeping", meta = (AllowPrivateAccess = true))
	FText							currentDescription;

	/**
	 *Reference to quest manager
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Bookkeeping", meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
	UQuestManager*					questManagerRef;
	
	/**
	 *Updates currentGoals to match that of the currentGoalIndices
	 */
	void							UpdateSubGoals();

	/**
	 *Called at spawn time to find out how many items already obtained for find type goal
	 */
	void							FindInitialItemAmount(int goalIndex);

public:
	/**
	 *Goals currently in progress
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Bookkeeping")
	TArray<FGoalInfo>				currentGoals;

	/**
	 *Map that links a goal (via its index) to the number representing the thing that needs to be kept track (amount to be killed, amount to interact with)
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Bookkeeping", meta = (AllowPrivateAccess = true))
	TMap<int,int>					currentAmounts;

	/**
	 *Map that links a goal to the actors that have been interacted with so we don't interact with the same actors twice... granted the quest asks to interact with multiple actors
	 */
	TMap<int, TArray<UNamedInteractableDecorator*>>			interactedActors;

	/**
	 *Struct with information about quest
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Information")
	FQuestInfo						questInfo; 

	UFUNCTION()
	void							BeginPlay() override;

	/**
	 *Accessor to the indices of the goals that are currently being handled in the quest)
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessor")
	TArray<int>						GetCurrentGoalIndices() const { return currentGoalIndices; }

	/**
	 *Accesor to goals
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessor")
	FGoalInfo						GetGoalAtIndex(int goalIndex);

	/**
	 *Quest state accessor for BP
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessor")
	EQuestState						GetQuestState() const { return currentState; }

	/**
	 *Initiates completion of a subgoal
	 *@param goalIndex - Index of the goal in quest subgoal array
	 *@param fail - Whether this goal failed or not
	 */
	UFUNCTION(BlueprintCallable, Category = "Updating")
	void							CompleteSubGoal(int goalIndex, bool fail); 

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

	/**Returns the text of what the goal should be based on the goal's type and information from the quest*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helper")
	static FText					MakeGoalText(AQuest* assignedQuest, FGoalInfo goalInfo, int goalIndex);
};
