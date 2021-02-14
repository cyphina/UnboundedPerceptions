// Created 2/10/21 12:22 AM

#pragma once

#include "GameplayTagContainer.h"
#include "Item.h"

class UUpGoal;

/** Organize quest types based on importance to story */
UENUM(BlueprintType)
enum class EQuestCategory : uint8 { MainQuest, SideQuest, Event };

/** State of quest completion */
UENUM(BlueprintType)
enum class EQuestState : uint8 { currentQuests, completedQuests, failedQuests };

/** Struct for possible quest rewards */
USTRUCT(BlueprintType, NoExport)
struct FQuestReward
{
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int gold;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int exp;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   TArray<FMyItem> items;
};

/** Struct representing when quest expires */
USTRUCT(BlueprintType, NoExport)
struct FQuestExpiration
{
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int chapter = 0;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int section = 0;
};

USTRUCT(BlueprintType, NoExport)
struct FQuestInfo
{
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   FGameplayTag id;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   FText name;

   /** The starting description of the quest (the current description is stored inside the quest actor) */
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   FText desc;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int suggestedLvl;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   FQuestReward questReward;

   /**
    * Difficulty from 1 (easy) to 10 (extremely hard)
    */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(ClampMin="1", ClampMax="10"))
   float difficulty;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   EQuestCategory questCategory;

   /**
   * Does this quest have an expiration time?  That is if this point of the story is reached, then the quest automatically fails
   */
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   FQuestExpiration questExpiration;

   const TArray<UUpGoal*>& GetSubgoals() const { return subgoals; }

private:
   /**
    * A list of all the goals in this quest information
    */
   UPROPERTY(EditAnywhere, Instanced, Meta=(AllowPrivateAccess))
   TArray<UUpGoal*> subgoals;
};
