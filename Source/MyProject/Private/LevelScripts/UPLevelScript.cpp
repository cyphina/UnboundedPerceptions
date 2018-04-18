// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UPLevelScript.h"
#include "UserInput.h"
#include "RTSGameState.h"
#include "Quests/QuestManager.h"
#include "MyGameInstance.h"
#include "SaveLoadClass.h"

void AUPLevelScript::BeginPlay()
{
	Super::BeginPlay();
	controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
	if(shouldTimePass)
		Cast<ARTSGameState>(GetWorld()->GetGameState())->AddGameTime(timeToPass);

	UMyGameInstance* gameInstance = Cast<UMyGameInstance>(GetGameInstance());
	gameInstance->GetSaveManager()->SetupLoad();
}
