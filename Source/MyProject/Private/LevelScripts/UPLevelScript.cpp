// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UPLevelScript.h"
#include "UserInput.h"
#include "RTSGameState.h"
#include "Quests/QuestManager.h"
#include "RTSGameMode.h"
#include "SaveLoadClass.h"

void AUPLevelScript::BeginPlay()
{	
	controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());

	if(shouldTimePass)
		Cast<ARTSGameState>(GetWorld()->GetGameState())->AddGameTime(timeToPass);

	ARTSGameMode* gameMode = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode());

	//When a new UPLevel is loaded, if this load was due to loading a save game, then setup what was saved.  May need some kind of delay?
	gameMode->GetSaveManager()->SetupLoad();

	Super::BeginPlay(); //remember this calls blueprint BeginPlay()
}
