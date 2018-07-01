#include "MyProject.h"
#include "RTSConditional.h"
#include "RTSGameMode.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "Quests/Quest.h"
#include "Quests/QuestManager.h"
#include "WorldObjects/BaseHero.h"
#include "GameplayTagContainer.h"
#include "NPC.h"

#define LOCTEXT_NAMESPACE "RTSConditional"

FConditionData FConditionData::defaultCond = FConditionData();

void UConditionalManager::Init()
{
	cpcRef = Cast<AUserInput>(GetOuter()->GetWorld()->GetFirstPlayerController());
	gameModeRef = Cast<ARTSGameMode>(GetOuter());
}

bool UConditionalManager::GetCondition(FConditionData& condData) const
{
	bool conditionValue = false;
	switch (condData.conditionalType)
	{
	case EConditionalType::None: break;
	case EConditionalType::HasQuestCond: 
		conditionValue = GetHasCurrentQuestConditionVal(condData); break;
	case EConditionalType::QuestCompletionCond:
		conditionValue = GetQuestConditionVal(condData); break;
	case EConditionalType::HadConversationCond:
		conditionValue = GetHadConversationConditionVal(condData); break;
	case EConditionalType::OwnsItemCond:
		conditionValue = GetOwnedItemConditionVal(condData); break;
	case EConditionalType::CustomCond:
		conditionValue = FCString::Atoi(*condData.conditionalValues[0]);
	default: break;
	}

	if (condData.reverseResult)
		return !conditionValue;
	return conditionValue;
}

FText UConditionalManager::GetConditionString(TArray<FConditionData> conditions) const
{
	FFormatOrderedArguments args;
	FText conditionString = NSLOCTEXT("RTSConditional", "Condition", "Requires: ");

	for (int i = 0; i < conditions.Num() - 1; ++i)
	{
		args.Add(conditionString);
		args.Add(GetConditionMessage(conditions[i]));
		conditionString = FText::Format(LOCTEXT("ConditionString", "{0} {1}"), args);
	}

	args.Add(conditionString);
	args.Add(GetConditionMessage(conditions[conditions.Num() - 1]));
	conditionString = FText::Format(LOCTEXT("ConditionString", "{0} {1}."), args);

	return conditionString;
}

bool UConditionalManager::GetQuestConditionVal(FConditionData& condData) const
{
	checkf(condData.conditionalValues.Num() == 1, TEXT("Error with number of quest condition values (%d items)"), condData.conditionalValues.Num());
	if (gameModeRef->GetQuestManager()->completedQuests.FindByPredicate([&](AQuest* questToFilter) { return questToFilter->questInfo.id.ToString() == "QuestName." + condData.conditionalValues[0]; }))
		return true;
	return false;
}

bool UConditionalManager::GetOwnedItemConditionVal(FConditionData& condData) const
{
	checkf(condData.conditionalValues.Num() == 1, TEXT("Error with number of owned item condition values (%d items)"), condData.conditionalValues.Num());
	for (ABaseHero* hero : cpcRef->GetBasePlayer()->heroes)
	{
		if (hero->backpack->FindItem(FCString::Atoi(*condData.conditionalValues[0])) != -1)
			return true;
	}
	return false;
}

bool UConditionalManager::GetHadConversationConditionVal(FConditionData& condData) const
{
	checkf(condData.conditionalValues.Num() == 2, TEXT("Error with number of owned item condition values (%d items)"), condData.conditionalValues.Num());
	return true;
}

bool UConditionalManager::GetHasCurrentQuestConditionVal(FConditionData& condData) const
{
	checkf(condData.conditionalValues.Num() == 1, TEXT("Error with number of quest condition values (%d items)"), condData.conditionalValues.Num());
	if (gameModeRef->GetQuestManager()->completedQuests.FindByPredicate([&](AQuest* questToFilter) { return questToFilter->questInfo.id.ToString() == "QuestName." + condData.conditionalValues[0]; }))
		return true;
	return false;
}

FText UConditionalManager::GetConditionMessage(FConditionData& cond) const
{
	FFormatOrderedArguments args;

	switch (cond.conditionalType)
	{
	case EConditionalType::None: return FText::GetEmpty();
	case EConditionalType::HasQuestCond:
		//Add the questName (not quest id to format string args)
		args.Add(gameModeRef->GetQuestManager()->questMap->questClassList[FGameplayTag::RequestGameplayTag(
			*(FString("QuestName.") + cond.conditionalValues[0]))]->GetDefaultObject<AQuest>()->questInfo.name);
		return cond.reverseResult ? FText::Format(LOCTEXT("QuestCompleteCond", "noncompletion of {0}"), args)
			: FText::Format(LOCTEXT("QuestCompleteCond", "completion of {0}"), args);
	case EConditionalType::QuestCompletionCond:
		//Add the questName (not quest id to format string args)
		args.Add(gameModeRef->GetQuestManager()->questMap->questClassList[FGameplayTag::RequestGameplayTag(
			*(FString("QuestName.") + cond.conditionalValues[0]))]->GetDefaultObject<AQuest>()->questInfo.name);
		return cond.reverseResult ? FText::Format(LOCTEXT("QuestCompleteCond", "noncompletion of {0}"), args)
			: FText::Format(LOCTEXT("QuestCompleteCond", "completion of {0}"), args);
	case EConditionalType::HadConversationCond:
		args.Add(FText::FromString(cond.conditionalValues[0]));
		//TODO: Convert topic names into FText via some kind of function
		args.Add(FText::FromString(cond.conditionalValues[1]));
		return cond.reverseResult ? FText::Format(LOCTEXT("ConversationTopicCond", "haven't talked to {0} about {1}"), args)
			: FText::Format(LOCTEXT("ConversationTopicCond", "talked to {0} about {1}"), args);
	case EConditionalType::OwnsItemCond:
		args.Add(FText::FromString(cond.conditionalValues[0]));
		//TODO: Create a table of all the possibly item names
		return cond.reverseResult ? FText::Format(LOCTEXT("ConversationTopicCond", "no {0} in inventory"), args)
			: FText::Format(LOCTEXT("ConversationTopicCond", "posession of {0}"), args);
	default: return FText::GetEmpty();
	}
}

#undef LOCTEXT_NAMESPACE