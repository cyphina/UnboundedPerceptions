// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "StartLevelScript.h"
#include "MyGameInstance.h"
#include "Quests/QuestManager.h"
#include "UserInput.h"

void AStartLevelScript::BeginPlay()
{
   Super::BeginPlay();
   controllerRef->SetActorTickEnabled(false);
   controllerRef->ChangeCursor(ECursorStateEnum::Select);
   // UGameplayStatics::GetGameMode()->SetActorTickEnabled(false);
   // GetWorld()->GetGameState()->SetActorTickEnabled(false);
}
