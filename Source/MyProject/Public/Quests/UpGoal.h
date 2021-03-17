// Created 2/8/21 10:07 PM

#pragma once

#include "GameplayTagContainer.h"
#include "Trigger.h"
#include "UpGoal.generated.h"

class AEnemy;
class AInteractableBase;
class AQuest;
class ANPC;

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
};

USTRUCT(BlueprintType, NoExport)
struct FGoalStyleInfo
{
   /** Do we have a location tracker on the minimap? */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Minimap")
   bool shouldUseRadius = false;

   /** What's the size of this location tracker? */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Minimap")
   float radius = 0.f;

   /** What's the color of the location tracker */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Minimap")
   FLinearColor circleColor = FLinearColor::White;

   /** Location to spawn the tracker */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Minimap", Meta=(MakeEditWidget))
   FVector goalLocation = invalidGoalLocation;

   static const inline FVector invalidGoalLocation = FVector(-66666, -66666, -66666);
};

/** Has information about the subgoal's CDO (no instanced data here) */
USTRUCT(BlueprintType, NoExport)
struct FGoalInfo
{
   FGoalInfo() :
      canFailQuest(false), canCompleteQuest(false),
      updatedDescription(FText())
   {
   }

   FGoalInfo
   (EGoalState gS, bool  isC, FText   gText, TArray<FText> aN, int aTH, bool sUQD, FText uD, TArray<int> fSGI, bool sUR, float r, FLinearColor cC,
    bool       cFQ, bool cCQ, FVector gL) :
      canFailQuest(cFQ), canCompleteQuest(cCQ),
      updatedDescription(uD)
   {
   }

   /**If we fail this goal the quest will fail*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Game Data")
   bool canFailQuest;

   /**If we finish this goal the quest will be done*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Game Data")
   bool canCompleteQuest;

   /**Names used for various things
    *If the goalType is hunting, this is the type name of the monster to hunt
    *If the goalType is finding, this is the item you should find (index 0) and then the NPC to turn it into (index 1)
    *If the goalType is talking, this is the NPC you need to talk to (index 0), and the dialog topic to talk about (index 1)
    *If the goalType is custom, we can use this for something else (identifier for this goal so that a trigger can complete it)
    */
   // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Game Data")
   // TArray<FText> additionalNames;

   /** Do we update the quest description (like adding new entries) after this goal is completed? */
   bool ShouldUpdateDescription() const { return !updatedDescription.IdenticalTo(FText::GetEmpty()); }

   /** If we update the quest description after this goal is completed, do so here */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Game Data")
   FText updatedDescription;
};

UCLASS(Abstract, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class MYPROJECT_API UUpGoal : public UObject
{
   GENERATED_BODY()

public:
   UFUNCTION(BlueprintCallable)
   const FGoalInfo& GetGoalInfo() const { return goalInfo; }

   UFUNCTION(BlueprintCallable)
   const FGoalStyleInfo& GetGoalStyleInfo() const { return goalStyleInfo; }

   UFUNCTION(BlueprintCallable, BlueprintPure)
   const TArray<int>& GetRequiredSubgoalIndices() const { return requiredSubgoalIndices; }

   int ClearPrereqGoal(int finishedGoalIndex) { return requiredSubgoalIndices.Remove(finishedGoalIndex); }

   bool bPrereqGoalsFinished() const { return requiredSubgoalIndices.Num() > 0; }

   /**Should any triggers be run when this goal begins?*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Game Data")
   TArray<FTriggerData> prevGoalTriggers;

   /**Should any triggers be run when this goal is over?*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Game Data")
   TArray<FTriggerData> afterGoalTriggers;

   EGoalState GetGoalState() const { return goalState; }

   void SetGoalState(EGoalState newGoalState) { goalState = newGoalState; }

   virtual FText MakeGoalText() const PURE_VIRTUAL(UUpGoal::MakeGoalText, return FText::GetEmpty(););

protected:
   UPROPERTY(EditAnywhere, Category = "Goal Properties")
   FGoalInfo goalInfo;

   UPROPERTY(EditAnywhere, Category = "Goal Properties")
   FGoalStyleInfo goalStyleInfo;

   UPROPERTY(BlueprintReadOnly, Category = "Goal Properties")
   EGoalState goalState = EGoalState::lockedGoal;

   /** What goals need to be completed to unlock this one? Indices are relative to the order they are listed in the quest */
   UPROPERTY(EditAnywhere, Category = "Goal Game Data")
   TArray<int> requiredSubgoalIndices;

   static inline FText EmptyGoalArgText = NSLOCTEXT("Quest", "EmptyGoalArg", "(Empty-CDO-Placeholder)");
   
   // TODO: Maybe add some delegates to when the goals are completed? If I can find a reason to that is...
};

UCLASS()
class MYPROJECT_API UUpHuntingGoal : public UUpGoal
{
   GENERATED_BODY()

public:
   UFUNCTION(BlueprintCallable, BlueprintPure)
   int GetNumCurrentlyHunted() const { return numCurrentlyHunted; }

   UFUNCTION(BlueprintCallable)
   void SetNumCurrentlyHunted(int newHuntedAmt) { numCurrentlyHunted = newHuntedAmt; }

   UFUNCTION(BlueprintCallable, BlueprintPure)
   int GetNumberOfEnemiesToHunt() const { return numberOfEnemiesToHunt; };

   UFUNCTION(BlueprintCallable, BlueprintPure)
   TSubclassOf<AEnemy> GetEnemyToHuntClass() const { return enemyToHunt; };

   FText MakeGoalText() const override;

protected:
   UPROPERTY(EditAnywhere, Category = "Goal Specifications")
   TSubclassOf<AEnemy> enemyToHunt;

   UPROPERTY(EditAnywhere, Category = "Goal Specifications")
   int numberOfEnemiesToHunt;

   int numCurrentlyHunted;
};

UCLASS()
class MYPROJECT_API UUpGatherGoal : public UUpGoal
{
   GENERATED_BODY()

public:

   UUpGatherGoal();
   
   UFUNCTION(BlueprintCallable, BlueprintPure)
   int GetNumCurrentlyGathered() const { return numItemsCurrentlyGathered; }

   UFUNCTION(BlueprintCallable)
   void SetNumCurrentlyGathered(int newGatheredAmt) { numItemsCurrentlyGathered = newGatheredAmt; }

   UFUNCTION(BlueprintCallable)
   int32 GetItemToGatherId() const { return FCString::Atoi(*itemToGatherId.RowName.ToString()); }

   UFUNCTION(BlueprintCallable)
   int GetNumberOfItemToGather() const { return numberOfItemToGather; }

   UFUNCTION(BlueprintCallable, BlueprintPure)
   TSubclassOf<ANPC> GetNPCToTurnInItemsTo() const { return npcToTurnInItemsTo; };

   FText MakeGoalText() const override;

protected:
   // TODO: Statically load this and set it up?
   UPROPERTY(EditAnywhere, Category = "Goal Specifications")
   FDataTableRowHandle itemToGatherId;

   UPROPERTY(EditAnywhere, Category = "Goal Specifications")
   TSubclassOf<ANPC> npcToTurnInItemsTo;

   UPROPERTY(EditAnywhere, Category = "Goal Specifications")
   int numberOfItemToGather = 1;

   int numItemsCurrentlyGathered = 0;
};

UCLASS()
class MYPROJECT_API UUpInteractGoal : public UUpGoal
{
   GENERATED_BODY()

public:

   UUpInteractGoal();
   
   UFUNCTION(BlueprintCallable)
   TSubclassOf<AInteractableBase> GetInteractableClass() const { return interactableClass; }

   UFUNCTION(BlueprintCallable)
   const FText& GetInteractableName() const { return interactableName; }

   FText MakeGoalText() const override;

protected:
   UPROPERTY(EditAnywhere, Category = "Goal Specifications")
   TSubclassOf<AInteractableBase> interactableClass;

   UPROPERTY(EditAnywhere, Category = "Goal Specifications")
   FText interactableName;
};

UCLASS()
class MYPROJECT_API UUpTalkGoal : public UUpGoal
{
   GENERATED_BODY()

public:

   UUpTalkGoal();
   
   FText MakeGoalText() const override;

   TSubclassOf<ANPC> GetNPCToTurnInItemsTo() const { return npcToTurnInItemsTo; }

   FGameplayTag GetTopicToTalkAbout() const { return topicToTalkAbout; }

protected:
   UPROPERTY(EditAnywhere, Category = "Goal Specifications")
   TSubclassOf<ANPC> npcToTurnInItemsTo;

   UPROPERTY(EditAnywhere, Category = "Goal Specifications", meta = (Categories="Dialog"))
   FGameplayTag topicToTalkAbout;
};

/*
 * Complete a goal of this type via triggers.
 */
UCLASS()
class MYPROJECT_API UUpCustomGoal : public UUpGoal
{
   GENERATED_BODY()

public:
   FText MakeGoalText() const override;

protected:
   /** Goal's custom description*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal Specifications")
   FText goalDescription;
};
