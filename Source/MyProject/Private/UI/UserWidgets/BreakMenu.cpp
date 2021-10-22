// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BreakMenu.h"

#include "Button.h"
#include "UserInput.h"
#include "UI/HUDManager.h"
#include "RTSGameMode.h"

void UBreakMenu::NativeConstruct()
{
   gameModeRef = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode());
   UMyDraggableWidget::NativeConstruct();

   btnResume->OnClicked.AddDynamic(this, &UBreakMenu::Resume);
   btnSaveLoad->OnClicked.AddDynamic(this, &UBreakMenu::SaveLoad);
   btnOption->OnClicked.AddDynamic(this, &UBreakMenu::Options);
   btnKeys->OnClicked.AddDynamic(this, &UBreakMenu::RemapKeys);
   btnExit->OnClicked.AddDynamic(this, &UBreakMenu::Exit);
}

void UBreakMenu::Resume()
{
   hudManagerRef->AddHUD(EHUDs::HS_Break);
}

void UBreakMenu::SaveLoad()
{
   hudManagerRef->AddHUD(EHUDs::HS_SaveLoad);
   Resume();
}

void UBreakMenu::Options()
{
   hudManagerRef->AddHUD(EHUDs::HS_Settings);
   Resume();
}

void UBreakMenu::RemapKeys()
{
   hudManagerRef->AddHUD(EHUDs::HS_KeyMap);
   Resume();
}

void UBreakMenu::Exit()
{
   gameModeRef->StreamLevelAsync(*gameModeRef->GetStartingLvlName());
}
