// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSGameMode.h"

void ARTSGameMode::BeginPlay()
{
	Super::BeginPlay(); 
}

ARTSGameMode::ARTSGameMode() : Super()
{
	//set defaults
	static ConstructorHelpers::FClassFinder<APawn> playerPawnClassFinder(TEXT("/Game/RTS_Tutorial/Blueprints/CameraPawn"));
	DefaultPawnClass = playerPawnClassFinder.Class;
	//static ConstructorHelpers::FClassFinder<APlayerController> playerControllerClassFinder(TEXT("/Game/RTS_Tutorial/Blueprints/CameraPawnController"));
	//PlayerControllerClass = playerControllerClassFinder.Class;
}

