#pragma once

/** Organize quest types based on importance to story */
UENUM(BlueprintType)
enum class EQuestCategory : uint8
{
   MainQuest,
   SideQuest,
   Event
};

/** State of quest completion */
UENUM(BlueprintType)
enum class EQuestState : uint8
{
   CurrentQuests,
   CompletedQuests,
   FailedQuests
};
