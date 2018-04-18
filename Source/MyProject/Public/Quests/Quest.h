// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "Quest.generated.h"

class UMyItem;
class UQuestManager;
class UQuestListSlot;

/**
 * All about quests!
 */

///---ENUMS---
//State of a subgoal
UENUM(BlueprintType)
enum class EGoalState : uint8
{
	currentGoal,
	completedGoal,
	failedGoal
};
//What task we must perform to complete a goal
UENUM(BlueprintType)
enum class EGoalType : uint8
{
	Custom,
	Hunt,
	Find,
	Talk
};
//Organize quest types based on importance to story
UENUM(BlueprintType)
enum class EQuestCategory : uint8
{
	MainQuest,
	SideQuest,
	Event
};
//State of quest completion
UENUM(BlueprintType)
enum class EQuestState : uint8
{
	currentQuests,
	completedQuests,
	failedQuests
};
///--Structs---
//Information about goals
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool							isCustomGoal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText							goalText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText							additionalName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int								amountToHunt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool							shouldUpdateQuestDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText							updatedDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int>						followingSubGoalIndices; //does this subgoal lead to another?  Stores index of the next goal that should be stored in questinfo
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool							shouldUseRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float							radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor					circleColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool							canFailQuest; //if we finish this goal the quest will fail
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool							canCompleteQuest; //if we finish this goal the quest will be done
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
	TArray<UMyItem*>				items;
};
//struct for quest properties
USTRUCT(BlueprintType, NoExport)
struct FQuestInfo
{
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText							name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText							desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int								suggestedLvl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuestReward					questReward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float							difficulty;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestCategory					questCategory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGoalInfo>				subgoals; //stores information about next goals
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

	/**Accessor to current goal indices*/
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
