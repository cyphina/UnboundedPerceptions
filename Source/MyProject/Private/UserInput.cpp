// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UserInput.h"
#include "MyGameInstance.h"
#include "RTSGameMode.h"
#include "RTSGameState.h"
#include "RTSPawn.h"
#include "UI/HUDManager.h"
#include "RTSCheatManager.h"

AUserInput::AUserInput()
{
   // Initialization of variables
   PrimaryActorTick.bCanEverTick = false;
   basePlayer                    = nullptr;
   gameInstance                  = nullptr;

   bShowMouseCursor   = true;
   DefaultMouseCursor = EMouseCursor::Type::GrabHandClosed;
   // HitResultTraceDistance = 100000.f; set in parent

   CheatClass = URTSCheatManager::StaticClass();
}

void AUserInput::BeginPlay()
{
   gameInstance = Cast<UMyGameInstance>(GetGameInstance());
   gameMode     = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode());
   gameState    = Cast<ARTSGameState>(GetWorld()->GetGameState());
   cameraPawn   = Cast<ARTSPawn>(GetPawn());

   basePlayer = Cast<ABasePlayer>(PlayerState);

   GetWorld()->SpawnActor<AHUDManager>(hudManagerClass, FTransform(), FActorSpawnParameters());
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

   InputComponent->BindAction("ToggleBreakMenu", IE_Pressed, this, &AUserInput::ToggleBreakMenu);
   InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AUserInput::ToggleInventory);
   InputComponent->BindAction("ToggleCharacterSheet", IE_Pressed, this, &AUserInput::ToggleCharacterMenu);
   InputComponent->BindAction("ToggleEquipmentMenu", IE_Pressed, this, &AUserInput::ToggleEquipmentMenu);
   InputComponent->BindAction("ToggleSpellbookMenu", IE_Pressed, this, &AUserInput::ToggleSpellbookMenu);
   InputComponent->BindAction("ToggleQuestWidget", IE_Pressed, this, &AUserInput::ToggleQuestList);
   InputComponent->BindAction("ToggleQuestJournal", IE_Pressed, this, &AUserInput::ToggleQuestJournal);
}

FORCEINLINE IWidgetToggler* AUserInput::GetWidgetToggler() const
{
   return hudManagerRef;
}

FORCEINLINE IHUDProvider* AUserInput::GetWidgetProvider() const
{
   return hudManagerRef;
}

void AUserInput::ToggleBreakMenu() const
{
   hudManagerRef->AddHUD(static_cast<uint8>(HUDs::HS_Break));
}

void AUserInput::ToggleInventory() const
{
   if(GetBasePlayer()->selectedHeroes.Num() > 0 && NotInMinigame()) { hudManagerRef->AddHUD(static_cast<uint8>(HUDs::HS_Inventory)); }
}

void AUserInput::ToggleQuestJournal() const
{
   if(NotInMinigame()) hudManagerRef->AddHUD(static_cast<uint8>(HUDs::HS_QuestJournal));
}

void AUserInput::ToggleQuestList() const
{
   if(NotInMinigame()) hudManagerRef->AddHUD(static_cast<uint8>(HUDs::HS_QuestList));
}

void AUserInput::ToggleCharacterMenu() const
{
   if(GetBasePlayer()->selectedHeroes.Num() > 0 || GetWidgetToggler()->IsWidgetOnScreen(HUDs::HS_Character)) {
      if(NotInMinigame()) hudManagerRef->AddHUD(static_cast<uint8>(HUDs::HS_Character));
   }
}

void AUserInput::ToggleEquipmentMenu() const
{
   int numSelectedHeroes = GetBasePlayer()->selectedHeroes.Num();
   if(numSelectedHeroes > 0 && NotInMinigame()) { hudManagerRef->AddHUD(static_cast<uint8>(HUDs::HS_Equipment)); }
}

void AUserInput::ToggleSpellbookMenu() const
{
   int numSelectedHeroes = GetBasePlayer()->selectedHeroes.Num();
   if(numSelectedHeroes > 0 && NotInMinigame()) { hudManagerRef->AddHUD(static_cast<uint8>(HUDs::HS_Spellbook)); }
}

bool AUserInput::NotInMinigame() const
{
   return GetPawn() == GetCameraPawn();
}
