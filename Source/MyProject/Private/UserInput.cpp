// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UserInput.h"
#include "MyGameInstance.h"
#include "RTSGameMode.h"
#include "RTSGameState.h"
#include "RTSPawn.h"
#include "UI/HUDManager.h"
#include "MyCheatManager.h"

AUserInput::AUserInput()
{
   // Initialization of variables
   PrimaryActorTick.bCanEverTick = false;
   basePlayer                    = nullptr;
   gameInstance                  = nullptr;

   bShowMouseCursor   = true;
   DefaultMouseCursor = EMouseCursor::Type::GrabHandClosed;
   // HitResultTraceDistance = 100000.f; set in parent

   CheatClass = UMyCheatManager::StaticClass();
}

void AUserInput::BeginPlay()
{
   gameInstance = Cast<UMyGameInstance>(GetGameInstance());
   gameMode     = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode());
   gameState    = Cast<ARTSGameState>(GetWorld()->GetGameState());
   cameraPawn   = Cast<ARTSPawn>(GetPawn());

   basePlayer = Cast<ABasePlayer>(PlayerState);

   hudManager = GetWorld()->SpawnActor<AHUDManager>(AHUDManager::StaticClass(), FTransform(), FActorSpawnParameters());
   Super::BeginPlay();
}

void AUserInput::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);
}

void AUserInput::SetupInputComponent()
{
   Super::SetupInputComponent();
   check(InputComponent);

   InputComponent->BindAction("Break", IE_Pressed, this, &AUserInput::ToggleBreakMenu);
}

void AUserInput::ToggleBreakMenu()
{
   GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_Break));
}
