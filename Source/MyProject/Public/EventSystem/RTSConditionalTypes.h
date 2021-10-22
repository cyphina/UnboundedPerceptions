#pragma once
#include "Item.h"
#include "QuestEnums.h"
#include "RTSConditional.h"
#include "RTSConditionalTypes.generated.h"

class ANPC;
class UUpGoal;
class AQuest;

UCLASS()
class URTSConditionalHasQuest : public URTSConditional
{
   GENERATED_BODY()

 public:
   bool CheckIsConditionSatisfied_Implementation() override;

   UPROPERTY(EditAnywhere)
   TSubclassOf<AQuest> questPlayerShouldHaveClass;
};

UCLASS()
class URTSConditionalQuestCompleted : public URTSConditional
{
   GENERATED_BODY()

 public:
   bool CheckIsConditionSatisfied_Implementation() override;

   UPROPERTY(EditAnywhere)
   TSubclassOf<AQuest> completedQuestClass;

   UPROPERTY(EditAnywhere)
   EQuestState requiredQuestState;
};

UCLASS()
class URTSConditionalQuestGoalComplete : public URTSConditional
{
   GENERATED_BODY()

 public:
   bool CheckIsConditionSatisfied_Implementation() override;

   UPROPERTY(EditAnywhere)
   TSubclassOf<AQuest> questWithGoalClass;

   // TDOO: Make a custom goal picker widget
   UPROPERTY(EditAnywhere, Meta = (EditCondition = "questWithGoalClass != nullptr"))
   int goalIndex = 0;
};

UCLASS()
class URTSConditionalOwnsItem : public URTSConditional
{
   GENERATED_BODY()

 public:
   bool CheckIsConditionSatisfied_Implementation() override;

   UPROPERTY(EditAnywhere)
   FMyItem itemToOwn;
};

/** Check to see if the player has already talked to the NPC about a topic. If the topic tag is left blank, then checks to see if npc and player has already conversed about the default dialog*/
UCLASS()
class URTSConditionalHadConversation : public URTSConditional
{
   GENERATED_BODY()

 public:
   bool CheckIsConditionSatisfied_Implementation() override;

   UPROPERTY(EditAnywhere)
   TSubclassOf<ANPC> npcToCheckConversationClass;

   UPROPERTY(EditAnywhere, meta = (Categories = "Dialog"))
   FGameplayTag conversationTag;
};

UCLASS()
class URTSConditionalFulfilledStoryTag : public URTSConditionalBase
{
   GENERATED_BODY()

 public:
   bool CheckIsConditionSatisfied_Implementation() override;

   UPROPERTY(EditAnywhere, meta = (Categories = "Story"))
   FGameplayTag storyEventTag;
};
