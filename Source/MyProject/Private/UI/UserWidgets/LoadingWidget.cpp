// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "LoadingWidget.h"
#include "MyGameInstance.h"

ULoadingWidget::ULoadingWidget()
{
#define LOCTEXT_NAMESPACE "LoadingTips"
	
	tipList = { LOCTEXT("Tip1", "If an enemy is tanking itself up because of its healing spells, use a skill like Cursed Blade to reduce the healing they receive"),
				LOCTEXT("Tip2", "Don't be afraid to respec a hero's talent tree just for a single battle.  Every battle in this game is meant to be a challenge!"),
				LOCTEXT("Tip3", "Remember where your friends are..."),
				LOCTEXT("Tip4", "It's only human nature to make the same mistake hundreds of times, but try to understand and forgive")};
#undef LOCTEXT_NAMESPACE
}

void ULoadingWidget::Construct_Implementation()
{
	Super::Construct_Implementation();
	SetTip(tipList[FMath::RandRange(0,tipList.Num() - 1)]);
}
