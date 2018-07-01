// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "SubGoalWidget.h"

void USubGoalWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateText();
	goalIndex = assignedQuest->questInfo.subgoals.Find(goalInfo);
}

FText USubGoalWidget::MakeGoalText()
{
	FFormatOrderedArguments args;
	args.Add(goalInfo.additionalName);
	switch (goalInfo.goalType)
	{
	case EGoalType::Hunt:
		if (goalInfo.amountToHunt > 1)
			args.Add(NSLOCTEXT("Quests", "Suffix", "s"));
		else
			args.Add(FText());
		args.Add(assignedQuest->currentHuntedAmounts[goalIndex]);
		args.Add(goalInfo.amountToHunt);
		return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Hunt {0}{1}: {2}/{3}"), args);
	case EGoalType::Find:
		return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Find the {0}!"), args);
	case EGoalType::Talk:
		return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Talk to {0}"), args);
	case EGoalType::Interact:
		if (goalInfo.amountToHunt > 1)
			args.Add(NSLOCTEXT("Quests", "Suffix", "s"));
		else
			args.Add(FText());
		args.Add(assignedQuest->currentHuntedAmounts[goalIndex]);
		args.Add(goalInfo.amountToHunt);
		return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Interact with {0}{1}: {2}/{3}"), args);
	case EGoalType::Custom:
		return goalInfo.goalText;
	default:
		return FText();
	}

}


