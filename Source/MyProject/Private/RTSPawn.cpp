// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSPawn.h"
#include "MySceneCapture2D.h"
#include "UserInput.h"
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
#include "SceneViewport.h"

float const ARTSPawn::maxArmLength     = 4000.f;
float const ARTSPawn::minArmLength     = 250.f;
float const ARTSPawn::defaultArmLength = 2500.f;

// Sets default values
ARTSPawn::ARTSPawn()
{
   // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
   PrimaryActorTick.bCanEverTick = true;
   scene                         = CreateDefaultSubobject<USceneComponent>(FName("Scene"));
   SetRootComponent(scene);

   cameraArm = CreateDefaultSubobject<USpringArmComponent>(FName("CameraArm"));
   cameraArm->SetupAttachment(scene);
   cameraArm->TargetArmLength = defaultArmLength;

   camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
   camera->SetupAttachment(cameraArm);

   mapArm = CreateDefaultSubobject<USpringArmComponent>(FName("MapArm"));
   mapArm->SetupAttachment(scene);

   mapCamera = CreateDefaultSubobject<UChildActorComponent>(FName("Map"));
   mapCamera->SetChildActorClass(AMySceneCapture2D::StaticClass());
   mapCamera->SetupAttachment(mapArm);

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
}

// Called when the game starts or when spawned
void ARTSPawn::BeginPlay()
{
   mapCamera->CreateChildActor();
   AMySceneCapture2D* sceneCapture                      = Cast<AMySceneCapture2D>(mapCamera->GetChildActor());
   sceneCapture->GetCaptureComponent2D()->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
   sceneCapture->GetCaptureComponent2D()->CompositeMode = ESceneCaptureCompositeMode::SCCM_Overwrite;
   controllerRef                                        = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
   FViewport::ViewportResizedEvent.AddUObject(this, &ARTSPawn::RecalculateViewportSize);
   controllerRef->GetViewportSize(viewX, viewY);
   Super::BeginPlay();
}

// Called every frame
void ARTSPawn::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);
   CursorHover();
}

// Called to bind functionality to input
void ARTSPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
   Super::SetupPlayerInputComponent(PlayerInputComponent);
   check(InputComponent);

   InputComponent->BindAction("Stop", IE_Pressed, this, &ARTSPawn::StopSelectedAllyCommands);
   InputComponent->BindAction("PrevFlightPath", IE_Pressed, this, &ARTSPawn::PrevFlight);
   InputComponent->BindAction("NextFlightPath", IE_Pressed, this, &ARTSPawn::NextFlight);
   InputComponent->BindAction("OpenInventory", IE_Pressed, this, &ARTSPawn::Inventory);
   InputComponent->BindAction("OpenCharacterSheet", IE_Pressed, this, &ARTSPawn::Character);
   InputComponent->BindAction("Equipment", IE_Pressed, this, &ARTSPawn::Equipment);
   InputComponent->BindAction("Spellbook", IE_Pressed, this, &ARTSPawn::Spellbook);
   InputComponent->BindAction("RightClick", IE_Pressed, this, &ARTSPawn::RightClick);
   InputComponent->BindAction("QuestKey", IE_Pressed, this, &ARTSPawn::QuestJournal);
   InputComponent->BindAction("ToggleQuestWidget", IE_Pressed, this, &ARTSPawn::QuestList);
   InputComponent->BindAction("SelectNext", IE_Pressed, this, &ARTSPawn::TabNextAlly);
   InputComponent->BindAction<FAbilityUseDelegate>("UseAbility1", IE_Pressed, this, &ARTSPawn::UseAbility, 0);
   InputComponent->BindAction<FAbilityUseDelegate>("UseAbility2", IE_Pressed, this, &ARTSPawn::UseAbility, 1);
   InputComponent->BindAction<FAbilityUseDelegate>("UseAbility3", IE_Pressed, this, &ARTSPawn::UseAbility, 2);
   InputComponent->BindAction<FAbilityUseDelegate>("UseAbility4", IE_Pressed, this, &ARTSPawn::UseAbility, 3);
   InputComponent->BindAction<FAbilityUseDelegate>("UseAbility5", IE_Pressed, this, &ARTSPawn::UseAbility, 4);
   InputComponent->BindAction<FAbilityUseDelegate>("UseAbility6", IE_Pressed, this, &ARTSPawn::UseAbility, 5);
   InputComponent->BindAction("CameraSpeedup", IE_Pressed, this, &ARTSPawn::CameraSpeedOn);
   InputComponent->BindAction("CameraSpeedup", IE_Released, this, &ARTSPawn::CameraSpeedOff);
   InputComponent->BindAxis("MoveForward", this, &ARTSPawn::MoveX);
   InputComponent->BindAxis("MoveRight", this, &ARTSPawn::MoveY);
   InputComponent->BindAxis("EdgeMoveX", this, &ARTSPawn::EdgeMovementX);
   InputComponent->BindAxis("EdgeMoveY", this, &ARTSPawn::EdgeMovementY);
   InputComponent->BindAxis("EdgeMoveX", this, &ARTSPawn::MMBDragX);
   InputComponent->BindAxis("EdgeMoveY", this, &ARTSPawn::MMBDragY);
   InputComponent->BindAxis("EdgeMoveX", this, &ARTSPawn::PanX);
   InputComponent->BindAxis("EdgeMoveY", this, &ARTSPawn::PanY);
   InputComponent->BindAction("PanReset", IE_Pressed, this, &ARTSPawn::PanReset);
   InputComponent->BindAction("Zoom In", IE_Pressed, this, &ARTSPawn::ZoomIn);
   InputComponent->BindAction("Zoom Out", IE_Pressed, this, &ARTSPawn::ZoomOut);
}

void ARTSPawn::PossessedBy(AController* newController)
{
   Super::PossessedBy(newController);
   PrimaryActorTick.bCanEverTick = true;
   EnableInput(controllerRef);
}

void ARTSPawn::UnPossessed()
{
   Super::UnPossessed();
   PrimaryActorTick.bCanEverTick = false;
   DisableInput(controllerRef);
}

void ARTSPawn::SetSecondaryCursor(ECursorStateEnum cursorType)
{
   if (cursorType == ECursorStateEnum::Magic || cursorType == ECursorStateEnum::Item) {
      hasSecondaryCursor = true;
      hitActor           = nullptr;
      ChangeCursor(cursorType);
   } else {
      hasSecondaryCursor = false;
   }
}

void ARTSPawn::ChangeCursor(ECursorStateEnum newCursorState)
{
   if (cursorState != newCursorState) {
      cursorState                       = newCursorState;
      controllerRef->CurrentMouseCursor = static_cast<EMouseCursor::Type>(newCursorState);
      FSlateApplication::Get().GetPlatformCursor().Get()->SetType(static_cast<EMouseCursor::Type>(newCursorState));
   }
}

void ARTSPawn::CursorHover()
{
   if (controllerRef->GetHUDManager() && controllerRef->GetHUDManager()->numWidgetsBlocking > 0) {
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

      if (controllerRef->GetBasePlayer() && controllerRef->GetBasePlayer()->selectedAllies.Num() > 0) {
         controllerRef->GetHitResultUnderCursorForObjects(leftClickQueryObjects, true, hitResult);
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

bool ARTSPawn::IsUnitOnScreen(AUnit* unitToCheck)
{
   FBox2D unitBoundaryScreenCoords = unitToCheck->FindBoundary();
   return unitBoundaryScreenCoords.Max.X < viewX || unitBoundaryScreenCoords.Max.Y < viewY;
}

void ARTSPawn::SetCameraArmLength(float newLength)
{
   cameraArm->TargetArmLength = FMath::Clamp(newLength + cameraArm->TargetArmLength, minArmLength, maxArmLength);
}

void ARTSPawn::RecalculateViewportSize(FViewport* viewport, uint32 newSize)
{
   GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("RECALC VIEWPORT"));
   controllerRef->GetViewportSize(viewX, viewY);
}

void ARTSPawn::ZoomIn()
{
   SetCameraArmLength(camMoveSpeedMultiplier * baseCameraMoveSpeed * zoomMultiplier * -1);
}

void ARTSPawn::ZoomOut()
{
   SetCameraArmLength(camMoveSpeedMultiplier * baseCameraMoveSpeed * zoomMultiplier);
}

void ARTSPawn::ZoomReset()
{
   cameraArm->TargetArmLength = defaultArmLength;
}

void ARTSPawn::LockCamera()
{
   isCamNavDisabled = isCamNavDisabled ? false : true;
}

void ARTSPawn::CameraSpeedOn()
{
   camMoveSpeedMultiplier = 2.f;
}

void ARTSPawn::CameraSpeedOff()
{
   camMoveSpeedMultiplier = 1.f;
}

FORCEINLINE void ARTSPawn::MoveX(float axisValue)
{
   if (!isCamNavDisabled) SetActorLocation(GetActorTransform().TransformPosition(FVector(axisValue * baseCameraMoveSpeed * camMoveSpeedMultiplier, 0, 0)));
}

FORCEINLINE void ARTSPawn::MoveY(float axisValue)
{
   if (!isCamNavDisabled) SetActorLocation(GetActorTransform().TransformPosition(FVector(0, axisValue * baseCameraMoveSpeed * camMoveSpeedMultiplier, 0)));
}

void ARTSPawn::MMBDragX(float axisValue)
{
   if (controllerRef->IsInputKeyDown(FKey("MiddleMouseButton")) && !controllerRef->IsInputKeyDown(FKey("LeftShift")) && !controllerRef->IsInputKeyDown(FKey("RightShift"))) {
      AddActorLocalOffset(FVector(0, axisValue * -1.f * camMoveSpeedMultiplier * baseCameraMoveSpeed, 0));
   }
}

void ARTSPawn::MMBDragY(float axisValue)
{
   if (controllerRef->IsInputKeyDown(FKey("MiddleMouseButton")) && !controllerRef->IsInputKeyDown(FKey("LeftShift")) && !controllerRef->IsInputKeyDown(FKey("RightShift"))) {
      AddActorLocalOffset(FVector(axisValue * -1.f * camMoveSpeedMultiplier * baseCameraMoveSpeed, 0, 0));
   }
}

void ARTSPawn::PanX(float axisValue)
{
   if (controllerRef->IsInputKeyDown(FKey("LeftShift")) || controllerRef->IsInputKeyDown(FKey("RightShift")) && controllerRef->IsInputKeyDown(FKey("MiddleMouseButton"))) {
      AddActorLocalRotation(FRotator(0, axisValue * camMoveSpeedMultiplier * baseCameraMoveSpeed, 0));
   }
}

void ARTSPawn::PanY(float axisValue)
{
   if (controllerRef->IsInputKeyDown(FKey("LeftShift")) || controllerRef->IsInputKeyDown(FKey("RightShift")) && controllerRef->IsInputKeyDown(FKey("MiddleMouseButton"))) {
      AddActorLocalRotation(FRotator(0, 0, axisValue * camMoveSpeedMultiplier * baseCameraMoveSpeed));
   }
}

void ARTSPawn::PanReset()
{
   SetActorRotation(FRotator(0, 0, -180.f));
}

void ARTSPawn::EdgeMovementX(float axisValue)
{
   if (!isCamNavDisabled) {
      float mouseX, mouseY;
      if (controllerRef->GetMousePosition(mouseX, mouseY)) {
         if (mouseX / viewX < .025) {
            AddActorLocalOffset(FVector(0, -1 * baseCameraMoveSpeed * camMoveSpeedMultiplier, 0));
            cursorDirections.AddUnique(ECursorStateEnum::PanLeft);
         } else if (mouseX / viewX > .975) {
            AddActorLocalOffset(FVector(0, baseCameraMoveSpeed * camMoveSpeedMultiplier, 0));
            cursorDirections.AddUnique(ECursorStateEnum::PanRight);
         } else {
            cursorDirections.Remove(ECursorStateEnum::PanLeft);
            cursorDirections.Remove(ECursorStateEnum::PanRight);
         }
      }
   }
}

void ARTSPawn::EdgeMovementY(float axisValue)
{
   if (!isCamNavDisabled) {
      float mouseX, mouseY;
      if (controllerRef->GetMousePosition(mouseX, mouseY)) {
         if (mouseY / viewY < .025) {
            AddActorLocalOffset(FVector(baseCameraMoveSpeed * camMoveSpeedMultiplier, 0, 0));
            cursorDirections.AddUnique(ECursorStateEnum::PanUp);
         } else if (mouseY / viewY > .975) {
            AddActorLocalOffset(FVector(-1 * baseCameraMoveSpeed * camMoveSpeedMultiplier, 0, 0));
            cursorDirections.AddUnique(ECursorStateEnum::PanDown);
         } else {
            cursorDirections.Remove(ECursorStateEnum::PanUp);
            cursorDirections.Remove(ECursorStateEnum::PanDown);
         }
      }
   }
}

void ARTSPawn::StopSelectedAllyCommands()
{
   for (int i = 0; i < controllerRef->GetBasePlayer()->selectedAllies.Num(); i++) {
      if (controllerRef->GetBasePlayer()->selectedAllies[i]->GetSelected()) {
         controllerRef->GetBasePlayer()->selectedAllies[i]->Stop();
         controllerRef->GetBasePlayer()->selectedAllies[i]->ClearCommandQueue();
      }
   }

   hasSecondaryCursor = false;
   HideSpellCircle();
}

void ARTSPawn::PrevFlight()
{
   for (int i = 0; i < controllerRef->GetBasePlayer()->selectedAllies.Num(); i++) {
      Cast<UFlyComponent>(controllerRef->GetBasePlayer()->heroes[i]->GetComponentByClass(UFlyComponent::StaticClass()))->PreviousFlightPathSelected();
   }
}

void ARTSPawn::NextFlight()
{
   for (int i = 0; i < controllerRef->GetBasePlayer()->selectedAllies.Num(); i++) {
      Cast<UFlyComponent>(controllerRef->GetBasePlayer()->heroes[i]->GetComponentByClass(UFlyComponent::StaticClass()))->NextFlightPathSelected();
   }
}

void ARTSPawn::Inventory()
{
   int numSelectedHeroes = controllerRef->GetBasePlayer()->selectedHeroes.Num();
   if (numSelectedHeroes > 0) { controllerRef->GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_Inventory)); }
}

void ARTSPawn::QuestJournal()
{
   controllerRef->GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_QuestJournal));
}

void ARTSPawn::QuestList()
{
   controllerRef->GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_QuestList));
}

void ARTSPawn::Character()
{
   int numSelectedHeroes = controllerRef->GetBasePlayer()->selectedHeroes.Num();
   if (numSelectedHeroes > 0 || controllerRef->GetHUDManager()->widgetReferences[static_cast<uint8>(HUDs::HS_Character)]->IsVisible()) {
      controllerRef->GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_Character));
   }
}

void ARTSPawn::Equipment()
{
   int numSelectedHeroes = controllerRef->GetBasePlayer()->selectedHeroes.Num();
   if (numSelectedHeroes > 0) { controllerRef->GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_Equipment)); }
}

void ARTSPawn::Spellbook()
{
   int numSelectedHeroes = controllerRef->GetBasePlayer()->selectedHeroes.Num();
   if (numSelectedHeroes > 0) { controllerRef->GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_Spellbook)); }
}

void ARTSPawn::RightClick()
{
   if (cursorState == ECursorStateEnum::Magic) {
      StopSelectedAllyCommands();
      return;
   }

   if (cursorState != ECursorStateEnum::UI) {
      if (cursorState != ECursorStateEnum::Attack) {
         if (controllerRef->GetHitResultUnderCursorForObjects(rightClickQueryObjects, false, hitResult)) {
            // last time when I didn't make this temporary, we tried making our lambda automatically capture location and it failed miserably
            FVector location = hitResult.Location;

            // create a little decal where we clicked
            CreateClickVisual(location);

            if (controllerRef->IsInputKeyDown(EKeys::LeftShift)) {
               GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, hitResult.Location.ToString());
               for (AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies) {
                  ally->QueueAction(TFunction<void()>([ally, location]() { ally->Move(location); }));
               }
            } else {
               StopSelectedAllyCommands();
               for (AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies) {
                  ally->Move(FVector(location));
               }
            }
         }
      } else {
         if (controllerRef->GetHitResultUnderCursorForObjects(leftClickQueryObjects, false, hitResult)) {
            AEnemy* enemy = Cast<AEnemy>(hitResult.GetActor());
            if (controllerRef->IsInputKeyDown(EKeys::LeftShift)) {
               for (AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies) {
                  ally->QueueAction(TFunction<void()>([ally, enemy]() { ally->BeginAttack(enemy); }));
               }
            } else {
               StopSelectedAllyCommands();
               for (AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies) {
                  ally->BeginAttack(enemy);
               }
            }
         }
      }
   }
}

void ARTSPawn::TabNextAlly()
{
   int selectedHeroIndex = -1;
   if (controllerRef->GetBasePlayer()->selectedAllies.Num() > 1) {
      controllerRef->GetBasePlayer()->unitIndex = (controllerRef->GetBasePlayer()->unitIndex + 1) % controllerRef->GetBasePlayer()->selectedAllies.Num();
      // Make sure any other selected heroes are actually in hero array.  If a hero is on the map, it better be in our party else spawn the NPC version of it
      controllerRef->GetHUDManager()->GetActionHUD()->SingleAllyViewIndexFree(controllerRef->GetBasePlayer()->selectedAllies[controllerRef->GetBasePlayer()->unitIndex]);
   } else {
      // tab through heroes if one/zero allies are selected
      if (controllerRef->GetBasePlayer()->selectedAllies.Num() > 0) {
         selectedHeroIndex = controllerRef->GetBasePlayer()->selectedHeroes[0]->heroIndex;
         controllerRef->GetBasePlayer()->heroes[selectedHeroIndex]->SetSelected(false);
      }
      GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Purple, FString::FromInt(controllerRef->GetBasePlayer()->heroes.Num()));
      controllerRef->GetBasePlayer()->heroes[(selectedHeroIndex + 1) % controllerRef->GetBasePlayer()->heroes.Num()]->SetSelected(true);
      controllerRef->OnAllySelectedDelegate.Broadcast();
   }
}

void ARTSPawn::UseAbility(int abilityIndex)
{
   controllerRef->GetHUDManager()->GetActionHUD()->UseSkill(abilityIndex);
}

void ARTSPawn::ClearSelectedAllies()
{
   while (controllerRef->GetBasePlayer()->selectedAllies.Num() > 0)
      controllerRef->GetBasePlayer()->selectedAllies[0]->SetSelected(false);

   controllerRef->GetBasePlayer()->focusedUnit = nullptr;
   controllerRef->OnAllyDeselectedDelegate.Broadcast();
}
