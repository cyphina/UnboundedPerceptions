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
   PrimaryActorTick.bCanEverTick = true;
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

   if(HasAuthority())
   {
      cameraPawn = Cast<ARTSPawn>(GetPawn());
      basePlayer = Cast<ABasePlayer>(PlayerState);
   }

   if(!HasAuthority() || !IsRunningDedicatedServer())
   {
      hudManagerRef = GetWorld()->SpawnActor<AHUDManager>(hudManagerClass, FTransform(), FActorSpawnParameters());
   }

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

TScriptInterface<IWidgetToggler> AUserInput::GetWidgetToggler() const
{
   return hudManagerRef;
}

TScriptInterface<IHUDProvider> AUserInput::GetWidgetProvider() const
{
   return hudManagerRef;
}

void AUserInput::ToggleBreakMenu()
{
   hudManagerRef->AddHUD(static_cast<uint8>(EHUDs::HS_Break));
}

void AUserInput::ToggleInventory()
{
   if(GetBasePlayer()->GetSelectedHeroes().Num() > 0 && NotInMinigame())
   {
      hudManagerRef->AddHUD(static_cast<uint8>(EHUDs::HS_Inventory));
   }
}

void AUserInput::ToggleQuestJournal()
{
   if(NotInMinigame()) hudManagerRef->AddHUD(static_cast<uint8>(EHUDs::HS_QuestJournal));
}

void AUserInput::ToggleQuestList()
{
   if(NotInMinigame()) hudManagerRef->AddHUD(static_cast<uint8>(EHUDs::HS_QuestList));
}

void AUserInput::ToggleCharacterMenu()
{
   if(GetBasePlayer()->GetSelectedHeroes().Num() > 0 || GetWidgetToggler()->IsWidgetOnScreen(EHUDs::HS_Character))
   {
      if(NotInMinigame()) hudManagerRef->AddHUD(static_cast<uint8>(EHUDs::HS_Character));
   }
}

void AUserInput::ToggleEquipmentMenu()
{
   const int numSelectedHeroes = GetBasePlayer()->GetSelectedHeroes().Num();
   if(numSelectedHeroes > 0 && NotInMinigame())
   {
      hudManagerRef->AddHUD(static_cast<uint8>(EHUDs::HS_Equipment));
   }
}

void AUserInput::ToggleSpellbookMenu()
{
   const int numSelectedHeroes = GetBasePlayer()->GetSelectedHeroes().Num();
   if(numSelectedHeroes > 0 && NotInMinigame())
   {
      hudManagerRef->AddHUD(static_cast<uint8>(EHUDs::HS_Spellbook));
   }
}

bool AUserInput::NotInMinigame()
{
   return GetPawn() == GetCameraPawn();
}

void AUserInput::OnRep_PlayerState()
{
   Super::OnRep_PlayerState();
   basePlayer = Cast<ABasePlayer>(PlayerState);
}

void AUserInput::OnRep_Pawn()
{
   Super::OnRep_Pawn();
   cameraPawn = Cast<ARTSPawn>(GetPawn());
}
