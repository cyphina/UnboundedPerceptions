// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "StartMenu.h"
#include "RTSGameMode.h"
#include "UserInput.h"
#include "HUDManager.h"
#include "MyGameInstance.h"
#include "SettingsMenu.h"
#include "MainWidget.h"

void UStartMenu::NativeConstruct()
{	
	gameModeRef = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode());
	controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());

	//Add setting menu to overlay in blueprints
	Super::NativeConstruct(); //This calls blueprint Construct so call at end
}

bool UStartMenu::CheckIfSaveFileExists(FString saveFileName)
{
	FString saveFilesPath = FPaths::ProjectDir().Append("\\SavedGames\\");
	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
	if(!platformFile.DirectoryExists(*saveFilesPath))
	{
		platformFile.CreateDirectory(*saveFilesPath);
	}
	if(platformFile.FileExists(*(saveFilesPath + "\\" + saveFileName)))
	{
		return true;
	}
	return false;
}

void UStartMenu::LevelTransition()
{
	gameModeRef->StreamLevelAsync(*gameModeRef->GetIntroductionLevelName());
	
	//Renable ticks that are stopped in the level script
	controllerRef->SetActorTickEnabled(true);
	gameModeRef->SetActorTickEnabled(true);
	GetWorld()->GetGameState()->SetActorTickEnabled(true);

	controllerRef->EnableInput(controllerRef);
	controllerRef->GetHUDManager()->GetMainHUD()->speedIndex = 2;
	controllerRef->GetHUDManager()->GetMainHUD()->SetGameSpeed();
	RemoveFromParent();
}
