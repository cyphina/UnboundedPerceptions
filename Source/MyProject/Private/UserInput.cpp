// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UserInput.h"
#include "MyGameInstance.h"
#include "RTSGameMode.h"
#include "RTSGameState.h"
#include "RTSCameraPawn.h"
#include "Engine.h"
#include "SceneViewport.h"
#include "Extras/FlyComponent.h"
#include "BasePlayer.h"
#include "WorldObjects/Ally.h"
#include "WorldObjects/BaseHero.h"
#include "WorldObjects/Enemies/Enemy.h"
#include "UI/HUDManager.h"
#include "Quests/QuestManager.h"
#include "UI/UserWidgets/MainWidget.h"
#include "AbilitySystemComponent.h"
#include "SpellSystem/MySpell.h"
#include "ActionbarInterface.h"
#include "MyCheatManager.h"

AUserInput::AUserInput()
{
   // Initialization of variables
   PrimaryActorTick.bCanEverTick = false;
   basePlayer                    = nullptr;
   gameInstance                  = nullptr;
   bShowMouseCursor              = true;
   DefaultMouseCursor            = EMouseCursor::Type::GrabHandClosed;

   // ObjectTypeQueries can be seen in enum list ObjectTypeQuery in the blueprints
   leftClickQueryObjects.Add(EObjectTypeQuery::ObjectTypeQuery1); // WorldStatic
   leftClickQueryObjects.Add(EObjectTypeQuery::ObjectTypeQuery7); // Enemy
   leftClickQueryObjects.Add(EObjectTypeQuery::ObjectTypeQuery8); // Interactable
   leftClickQueryObjects.Add(EObjectTypeQuery::ObjectTypeQuery9); // NPC
   // leftClickQueryObjects.Add(EObjectTypeQuery::ObjectTypeQuery10); don't hit buildings for these click traces
   // leftClickQueryObjects.Add(EObjectTypeQuery::ObjectTypeQuery11); //VisionBlocker
   leftClickQueryObjects.Add(EObjectTypeQuery::ObjectTypeQuery12); // Friendly

   rightClickQueryObjects.Add(EObjectTypeQuery::ObjectTypeQuery1); // WorldStatic
   rightClickQueryObjects.Add(EObjectTypeQuery::ObjectTypeQuery7); // Enemy

   // HitResultTraceDistance = 100000.f; set in parent
   CheatClass = UMyCheatManager::StaticClass();
}

void AUserInput::BeginPlay()
{
   gameInstance = Cast<UMyGameInstance>(GetGameInstance());
   gameMode     = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode());
   gameState    = Cast<ARTSGameState>(GetWorld()->GetGameState());
   cameraPawn   = Cast<ARTSCameraPawn>(GetPawn());

   // There should only be one player
   basePlayer = Cast<ABasePlayer>(PlayerState);

   // if (basePlayer)
   //{
   //	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Input component here"));
   //	TArray<ABaseHero*> heroesInWorld;

   //	for (TActorIterator<ABaseHero> heroItr(GetWorld()); heroItr; ++heroItr)
   //	{
   //		heroesInWorld.Add(*heroItr);
   //	}

   //	if (heroesInWorld.Num() > 0)
   //		basePlayer->UpdateParty(heroesInWorld);
   //}
   // Super Beginplay calls the blueprint BeginPlay

   hudManager = GetWorld()->SpawnActor<AHUDManager>(AHUDManager::StaticClass(), FTransform(), FActorSpawnParameters());
   Super::BeginPlay();
}

void AUserInput::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);
   CursorHover();
}

void AUserInput::SetupInputComponent()
{
   Super::SetupInputComponent();
   check(InputComponent);
   InputComponent->BindAction("Stop", IE_Pressed, this, &AUserInput::StopSelectedAllyCommands);
   InputComponent->BindAction("PrevFlightPath", IE_Pressed, this, &AUserInput::PrevFlight);
   InputComponent->BindAction("NextFlightPath", IE_Pressed, this, &AUserInput::NextFlight);
   InputComponent->BindAction("OpenInventory", IE_Pressed, this, &AUserInput::Inventory);
   InputComponent->BindAction("OpenCharacterSheet", IE_Pressed, this, &AUserInput::Character);
   InputComponent->BindAction("Equipment", IE_Pressed, this, &AUserInput::Equipment);
   InputComponent->BindAction("Spellbook", IE_Pressed, this, &AUserInput::Spellbook);
   InputComponent->BindAction("RightClick", IE_Pressed, this, &AUserInput::RightClick);
   InputComponent->BindAction("QuestKey", IE_Pressed, this, &AUserInput::QuestJournal);
   InputComponent->BindAction("ToggleQuestWidget", IE_Pressed, this, &AUserInput::QuestList);
   InputComponent->BindAction("Break", IE_Pressed, this, &AUserInput::ToggleBreakMenu);
   InputComponent->BindAction("SelectNext", IE_Pressed, this, &AUserInput::TabNextAlly);
}

void AUserInput::StopSelectedAllyCommands()
{
   for (int i = 0; i < basePlayer->selectedAllies.Num(); i++) {
      if (basePlayer->selectedAllies[i]->GetSelected()) {
         basePlayer->selectedAllies[i]->Stop();
         basePlayer->selectedAllies[i]->ClearCommandQueue();
      }
   }

   hasSecondaryCursor = false;
   HideSpellCircle();
}

void AUserInput::PrevFlight()
{
   for (int i = 0; i < basePlayer->selectedAllies.Num(); i++) {
      Cast<UFlyComponent>(basePlayer->heroes[i]->GetComponentByClass(UFlyComponent::StaticClass()))->PreviousFlightPathSelected();
   }
}

void AUserInput::NextFlight()
{
   for (int i = 0; i < basePlayer->selectedAllies.Num(); i++) {
      Cast<UFlyComponent>(basePlayer->heroes[i]->GetComponentByClass(UFlyComponent::StaticClass()))->NextFlightPathSelected();
   }
}

void AUserInput::Inventory()
{
   int numSelectedHeroes = basePlayer->selectedHeroes.Num();
   if (numSelectedHeroes > 0) { GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_Inventory)); }
}

void AUserInput::QuestJournal()
{
   GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_QuestJournal));
}

void AUserInput::QuestList()
{
   GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_QuestList));
}

void AUserInput::Character()
{
   int numSelectedHeroes = basePlayer->selectedHeroes.Num();
   if (numSelectedHeroes > 0 || GetHUDManager()->widgetReferences[static_cast<uint8>(HUDs::HS_Character)]->IsVisible()) { GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_Character)); }
}

void AUserInput::Equipment()
{
   int numSelectedHeroes = basePlayer->selectedHeroes.Num();
   if (numSelectedHeroes > 0) { GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_Equipment)); }
}

void AUserInput::Spellbook()
{
   int numSelectedHeroes = basePlayer->selectedHeroes.Num();
   if (numSelectedHeroes > 0) { GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_Spellbook)); }
}

void AUserInput::ToggleBreakMenu()
{
   GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_Break));
}

void AUserInput::TabNextAlly()
{
   int selectedHeroIndex = -1;
   if (basePlayer->selectedAllies.Num() > 1) {
      basePlayer->unitIndex = (basePlayer->unitIndex + 1) % basePlayer->selectedAllies.Num();
      // Make sure any other selected heroes are actually in hero array.  If a hero is on the map, it better be in our party else spawn the NPC version of it
      hudManager->GetActionHUD()->SingleAllyViewIndexFree(basePlayer->selectedAllies[basePlayer->unitIndex]);
   } else {
      // tab through heroes if one/zero allies are selected
      if (basePlayer->selectedAllies.Num() > 0) {
         selectedHeroIndex = basePlayer->selectedHeroes[0]->heroIndex;
         basePlayer->heroes[selectedHeroIndex]->SetSelected(false);
      }
      GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Purple, FString::FromInt(basePlayer->heroes.Num()));
      basePlayer->heroes[(selectedHeroIndex + 1) % basePlayer->heroes.Num()]->SetSelected(true);
      OnAllySelectedDelegate.Broadcast();
   }
}

void AUserInput::ClearSelectedAllies()
{
   while (basePlayer->selectedAllies.Num() > 0)
      basePlayer->selectedAllies[0]->SetSelected(false);

   basePlayer->focusedUnit = nullptr;
   OnAllyDeselectedDelegate.Broadcast();
}

bool AUserInput::IsUnitOnScreen(AUnit* unitToCheck)
{
   FBox2D unitBoundaryScreenCoords = unitToCheck->FindBoundary();
   int    sizeX, sizeY;
   GetViewportSize(sizeX, sizeY);
   return unitBoundaryScreenCoords.Max.X < sizeX || unitBoundaryScreenCoords.Max.Y < sizeY;
}

FORCEINLINE void AUserInput::MoveX(float axisValue)
{
   if (!isCamNavDisabled) cameraPawn->SetActorLocation(cameraPawn->GetActorTransform().TransformPosition(FVector(axisValue * baseCameraMoveSpeed * camMoveSpeedMultiplier, 0, 0)));
}

FORCEINLINE void AUserInput::MoveY(float axisValue)
{
   if (!isCamNavDisabled) cameraPawn->SetActorLocation(cameraPawn->GetActorTransform().TransformPosition(FVector(0, axisValue * baseCameraMoveSpeed * camMoveSpeedMultiplier, 0)));
}

void AUserInput::EdgeMovementX()
{
   if (!isCamNavDisabled) {
      int   viewX, viewY;
      float mouseX, mouseY;
      GetViewportSize(viewX, viewY);
      if (GetMousePosition(mouseX, mouseY)) {
         if (mouseX / viewX < .025) {
            cameraPawn->AddActorLocalOffset(FVector(0, -1 * baseCameraMoveSpeed * camMoveSpeedMultiplier, 0));
            cursorDirections.AddUnique(ECursorStateEnum::PanLeft);
         } else if (mouseX / viewX > .975) {
            cameraPawn->AddActorLocalOffset(FVector(0, baseCameraMoveSpeed * camMoveSpeedMultiplier, 0));
            cursorDirections.AddUnique(ECursorStateEnum::PanRight);
         } else {
            cursorDirections.Remove(ECursorStateEnum::PanLeft);
            cursorDirections.Remove(ECursorStateEnum::PanRight);
         }
      }
   }
}

void AUserInput::EdgeMovementY()
{
   if (!isCamNavDisabled) {
      int   viewX, viewY;
      float mouseX, mouseY;
      GetViewportSize(viewX, viewY);
      if (GetMousePosition(mouseX, mouseY)) {
         if (mouseY / viewY < .025) {
            cameraPawn->AddActorLocalOffset(FVector(baseCameraMoveSpeed * camMoveSpeedMultiplier, 0, 0));
            cursorDirections.AddUnique(ECursorStateEnum::PanUp);
         } else if (mouseY / viewY > .975) {
            cameraPawn->AddActorLocalOffset(FVector(-1 * baseCameraMoveSpeed * camMoveSpeedMultiplier, 0, 0));
            cursorDirections.AddUnique(ECursorStateEnum::PanDown);
         } else {
            cursorDirections.Remove(ECursorStateEnum::PanUp);
            cursorDirections.Remove(ECursorStateEnum::PanDown);
         }
      }
   }
}

void AUserInput::RightClick()
{
   if (cursorState == ECursorStateEnum::Magic) {
      StopSelectedAllyCommands();
      return;
   }

   if (cursorState != ECursorStateEnum::UI) {
      if (cursorState != ECursorStateEnum::Attack) {
         if (GetHitResultUnderCursorForObjects(rightClickQueryObjects, false, hitResult)) {
            // last time when I didn't make this temporary, we tried making our lambda automatically capture location and it failed miserably
            FVector location = hitResult.Location;

            // create a little decal where we clicked
            CreateClickVisual(location);

            if (IsInputKeyDown(EKeys::LeftShift)) {
               GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, hitResult.Location.ToString());
               for (AAlly* ally : basePlayer->selectedAllies) {
                  ally->QueueAction(TFunction<void()>([ally, location]() { ally->Move(location); }));
               }
            } else {
               StopSelectedAllyCommands();
               for (AAlly* ally : basePlayer->selectedAllies) {
                  ally->Move(FVector(location));
               }
            }
         }
      } else {
         if (GetHitResultUnderCursorForObjects(leftClickQueryObjects, false, hitResult)) {
            AEnemy* enemy = Cast<AEnemy>(hitResult.GetActor());
            if (IsInputKeyDown(EKeys::LeftShift)) {
               for (AAlly* ally : basePlayer->selectedAllies) {
                  ally->QueueAction(TFunction<void()>([ally, enemy]() { ally->BeginAttack(enemy); }));
               }
            } else {
               StopSelectedAllyCommands();
               for (AAlly* ally : basePlayer->selectedAllies) {
                  ally->BeginAttack(enemy);
               }
            }
         }
      }
   }
}

void AUserInput::SetSecondaryCursor(ECursorStateEnum cursorType)
{
   if (cursorType == ECursorStateEnum::Magic || cursorType == ECursorStateEnum::Item) {
      hasSecondaryCursor = true;
      hitActor           = nullptr;
      ChangeCursor(cursorType);
   } else {
      hasSecondaryCursor = false;
   }
}

void AUserInput::ChangeCursor(ECursorStateEnum newCursorState)
{
   if (cursorState != newCursorState) {
      cursorState        = newCursorState;
      CurrentMouseCursor = static_cast<EMouseCursor::Type>(newCursorState);
      FSlateApplication::Get().GetPlatformCursor().Get()->SetType(static_cast<EMouseCursor::Type>(newCursorState));
   }
}

void AUserInput::CursorHover()
{
   if (GetHUDManager() && GetHUDManager()->numWidgetsBlocking > 0) {
      ChangeCursor(ECursorStateEnum::UI);
      hitActor = nullptr;
      return;
   }

   if (!hasSecondaryCursor) {
      if (cursorDirections.Num() > 0 && !isCamNavDisabled) {
         ChangeCursor(cursorDirections.Last());
         hitActor = nullptr;
         return;
      }

      if (basePlayer && basePlayer->selectedAllies.Num() > 0) {
         GetHitResultUnderCursorForObjects(leftClickQueryObjects, true, hitResult);
         if (hitResult.GetActor()) {
            if (hitResult.GetActor() != hitActor) {
               hitActor = hitResult.GetActor();
               switch (hitResult.GetComponent()->GetCollisionObjectType()) {
                  case ECollisionChannel::ECC_WorldStatic: ChangeCursor(ECursorStateEnum::Moving); break;
                  case ECollisionChannel::ECC_GameTraceChannel3: ChangeCursor(ECursorStateEnum::Interact); break;
                  case ECollisionChannel::ECC_GameTraceChannel4: ChangeCursor(ECursorStateEnum::Talking); break;
                  case ECollisionChannel::ECC_GameTraceChannel2: ChangeCursor(ECursorStateEnum::Attack); break;
                  default: ChangeCursor(ECursorStateEnum::Select); break;
               }
            }
         }
      } else {
         ChangeCursor(ECursorStateEnum::Select);
      }
   }
}
