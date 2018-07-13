// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "SubGoalWidget.h"

void USubGoalWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateText();
	goalIndex = assignedQuest->questInfo.subgoals.Find(goalInfo);
}


