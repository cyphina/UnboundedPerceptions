// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"

#include "RTSPawn.h"
#include "MyGameInstance.h"
#include "UserInput.h"
#include "BasePlayer.h"

#include "Extras/FlyComponent.h"

#include "AIStuff/AIControllers/UnitController.h"
#include "AIStuff/AIControllers/AllyAIController.h"

#include "WorldObjects/Ally.h"
#include "WorldObjects/BaseHero.h"
#include "WorldObjects/Enemies/Enemy.h"

#include "UI/HUDManager.h"
#include "UI/UserWidgets/RTSIngameWidget.h"
#include "Quests/QuestManager.h"
#include "AbilitySystemComponent.h"
#include "ActionbarInterface.h"
#include "SceneViewport.h"

#include "Algo/Transform.h"
#include "GameBaseHelpers/DefaultCursorClickFunctionality.h"

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

   // ObjectTypeQueries can be seen in enum list ObjectTypeQuery in the blueprints
   // We use this to define what kind of objects can be affected by this raycast
   leftClickQueryObjects.Add(WORLD_STATIC_OBJECT_QUERY);
   leftClickQueryObjects.Add(ENEMY_OBJECT_QUERY);
   leftClickQueryObjects.Add(INTERACTABLE_OBJECT_QUERY);
   leftClickQueryObjects.Add(NPC_OBJECT_QUERY);
   // Don't hit buildings for these click traces
   // leftClickQueryObjects.Add(TRIGGER_OBJECT_QUERY);
   // leftClickQueryObjects.Add(VISION_BLOCKER_OBJECT_QUERY);
   leftClickQueryObjects.Add(FRIENDLY_OBJECT_QUERY);

   rightClickQueryObjects.Add(WORLD_STATIC_OBJECT_QUERY);
   rightClickQueryObjects.Add(ENEMY_OBJECT_QUERY);

   controlGroups.AddDefaulted(5);

   FCollisionQueryParams collisionQueryParams;
   collisionQueryParams.bTraceComplex = true;
}

bool ARTSPawn::IsAnyAllySelected() const
{
   return controllerRef->GetBasePlayer()->selectedAllies.Num() > 0;
}

// Called when the game starts or when spawned
void ARTSPawn::BeginPlay()
{
   controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
   // Stores new current viewport size in viewX and viewY if playing window resized
   FViewport::ViewportResizedEvent.AddUObject(this, &ARTSPawn::RecalculateViewportSize);
   // Stores current viewport size
   controllerRef->GetViewportSize(viewX, viewY);
   clickFunctionalityClass = MakeUnique<UDefaultCursorClickFunctionality>(this, controllerRef);
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
   InputComponent->BindAction("LockCamera", IE_Pressed, this, &ARTSPawn::LockCamera);
   InputComponent->BindAction("PrevFlightPath", IE_Pressed, this, &ARTSPawn::PrevFlight);
   InputComponent->BindAction("NextFlightPath", IE_Pressed, this, &ARTSPawn::NextFlight);
   InputComponent->BindAction("RightClick", IE_Pressed, this, &ARTSPawn::RightClick);
   InputComponent->BindAction("RightClickShift", IE_Pressed, this, &ARTSPawn::RightClickShift);
   InputComponent->BindAction("LeftClick", IE_Pressed, this, &ARTSPawn::LeftClick);
   InputComponent->BindAction("LeftClickShift", IE_Pressed, this, &ARTSPawn::LeftClickShift);

   InputComponent->BindAction("SelectNext", IE_Pressed, this, &ARTSPawn::TabNextAlly);
   InputComponent->BindAction("AttackMove", IE_Pressed, this, &ARTSPawn::AttackMoveInitiate);

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

   InputComponent->BindAction<FSelectControlGroupDelegate>("Control1", IE_Pressed, this, &ARTSPawn::SelectControlGroup, 1);
   InputComponent->BindAction<FSelectControlGroupDelegate>("Control2", IE_Pressed, this, &ARTSPawn::SelectControlGroup, 2);
   InputComponent->BindAction<FSelectControlGroupDelegate>("Control3", IE_Pressed, this, &ARTSPawn::SelectControlGroup, 3);
   InputComponent->BindAction<FSelectControlGroupDelegate>("Control4", IE_Pressed, this, &ARTSPawn::SelectControlGroup, 4);
   InputComponent->BindAction<FSelectControlGroupDelegate>("Control5", IE_Pressed, this, &ARTSPawn::SelectControlGroup, 5);

   InputComponent->BindAction<FSelectControlGroupDelegate>("Control1", IE_Released, this, &ARTSPawn::StopContolGroupFollow, 1);
   InputComponent->BindAction<FSelectControlGroupDelegate>("Control2", IE_Released, this, &ARTSPawn::StopContolGroupFollow, 2);
   InputComponent->BindAction<FSelectControlGroupDelegate>("Control3", IE_Released, this, &ARTSPawn::StopContolGroupFollow, 3);
   InputComponent->BindAction<FSelectControlGroupDelegate>("Control4", IE_Released, this, &ARTSPawn::StopContolGroupFollow, 4);
   InputComponent->BindAction<FSelectControlGroupDelegate>("Control5", IE_Released, this, &ARTSPawn::StopContolGroupFollow, 5);

   InputComponent->BindAction<FMakeControlGroupDelegate>("CreateControl1", IE_Pressed, this, &ARTSPawn::MakeControlGroup, 1);
   InputComponent->BindAction<FMakeControlGroupDelegate>("CreateControl2", IE_Pressed, this, &ARTSPawn::MakeControlGroup, 2);
   InputComponent->BindAction<FMakeControlGroupDelegate>("CreateControl3", IE_Pressed, this, &ARTSPawn::MakeControlGroup, 3);
   InputComponent->BindAction<FMakeControlGroupDelegate>("CreateControl4", IE_Pressed, this, &ARTSPawn::MakeControlGroup, 4);
   InputComponent->BindAction<FMakeControlGroupDelegate>("CreateControl5", IE_Pressed, this, &ARTSPawn::MakeControlGroup, 5);
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

void ARTSPawn::DisableInput(APlayerController* PlayerController)
{
   Super::DisableInput(PlayerController);
   hitActor = nullptr;
}

bool ARTSPawn::isSelectionRectActive() const
{
   const FVector2D difference = startMousePos - endMousePos;
   return FMath::Sqrt(FVector2D::DotProduct(difference, difference)) > 10;
}

void ARTSPawn::SetSecondaryCursor(ECursorStateEnum cursorType)
{
   switch(cursorType) {
      case ECursorStateEnum::Magic:
      case ECursorStateEnum::Item:
      case ECursorStateEnum::AttackMove:
         hasSecondaryCursor = true;
         hitActor           = nullptr;
         ChangeCursor(cursorType);
         break;
      default:
         hasSecondaryCursor = false;
         hitActor           = nullptr;
         break;
   }
}

void ARTSPawn::ChangeCursor(ECursorStateEnum newCursorState)
{
   if(cursorState != newCursorState) {
      cursorState                       = newCursorState;
      controllerRef->CurrentMouseCursor = static_cast<EMouseCursor::Type>(newCursorState);
      FSlateApplication::Get().GetPlatformCursor().Get()->SetType(static_cast<EMouseCursor::Type>(newCursorState));
   }
}

void ARTSPawn::GetHitResultClick(FHitResult& clickHitResult) const
{
   FVector worldLocation, worldDirection;
   if(controllerRef->DeprojectMousePositionToWorld(worldLocation, worldDirection)) {
      const auto               endPos = worldLocation + worldDirection * GetMaxArmLength() * CLICK_TRACE_LENGTH_MULTIPLIER;
      FCollisionResponseParams f;
      GetWorld()->LineTraceSingleByChannel(clickHitResult, worldLocation, endPos, SELECTABLE_BY_CLICK_CHANNEL, clickCollisionQueryParams);
   }
}

AActor* ARTSPawn::GetHitActorClick(FHitResult& clickHitResult)
{
   GetHitResultClick(clickHitResult);
   return clickHitResult.GetActor();
}

void ARTSPawn::CreateSelectionRect()
{
   if(GetCursorState() != ECursorStateEnum::UI) {
      float locX, locY;
      controllerRef->GetMousePosition(locX, locY);
      endMousePos = FVector2D(locX, locY);
      if(isSelectionRectActive()) {
         controllerRef->GetHUD()->DrawRect(GetSelectionRectColor(), startMousePos.X, startMousePos.Y, endMousePos.X - startMousePos.X, endMousePos.Y - startMousePos.Y);
         TArray<AAlly*> actorsInSelectRect;
         controllerRef->GetHUD()->GetActorsInSelectionRectangle<AAlly>(startMousePos, endMousePos, actorsInSelectRect, false, false);
         // Only update if we just overlapped a new actor (not one we already have selected)
         if(controllerRef->GetBasePlayer()->selectedAllies.Num() != actorsInSelectRect.Num()) {
            for(AAlly* allyInSelectRect : actorsInSelectRect) {
               if(!allyInSelectRect->GetSelected()) {
                  if(allyInSelectRect->GetCanTarget() && IsUnitOnScreen(allyInSelectRect)) {
                     allyInSelectRect->SetSelected(true);
                     OnAllySelectedDelegate.Broadcast(true);
                     break;
                  }
               }
            }
         }
      }
   }
}

FLinearColor ARTSPawn::GetSelectionRectColor() const
{
   return FLinearColor(0.15,0.57,0.78,0.4); // Blue-ish color
}

void ARTSPawn::CursorHover()
{
   if(!hasSecondaryCursor) {
      if(cursorDirections.Num() > 0 && !isCamNavDisabled) {
         ChangeCursor(cursorDirections.Last());
         hitActor = nullptr;
         return;
      }

      GetHitResultClick(hitResult);

      if(hitResult.GetActor()) {
         if(hitResult.GetActor() != hitActor) {
            // Remove higlights from last actor
            if(hitActor && hitActor->GetClass()->IsChildOf(ACharacter::StaticClass()))
               Cast<ACharacter>(hitActor)->GetMesh()->SetRenderCustomDepth(false);

            hitActor = hitResult.GetActor();

            // Add highlight to new actor
            if(hitActor && hitActor->GetClass()->IsChildOf(ACharacter::StaticClass()))
               Cast<ACharacter>(hitActor)->GetMesh()->SetRenderCustomDepth(true);

            if(controllerRef->GetBasePlayer() && controllerRef->GetBasePlayer()->selectedAllies.Num() > 0) {
               switch(hitResult.GetComponent()->GetCollisionObjectType()) {
                  case ECC_WorldStatic: ChangeCursor(ECursorStateEnum::Moving); return;
                  case INTERACTABLE_CHANNEL: ChangeCursor(ECursorStateEnum::Interact); return;
                  case NPC_CHANNEL: ChangeCursor(ECursorStateEnum::Talking); return;
                  case ENEMY_CHANNEL: ChangeCursor(ECursorStateEnum::Attack); return;
                  default: ChangeCursor(ECursorStateEnum::Select); return;
               }
            } else
               ChangeCursor(ECursorStateEnum::Select);
         }
      }
   } else { // If we have a spell or item cursor
      controllerRef->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(SELECTABLE_BY_CLICK_CHANNEL), true, hitResult);
      if(hitResult.GetActor()) {
         if(hitResult.GetActor() != hitActor) {
            if(hitActor && hitActor->GetClass()->IsChildOf(ACharacter::StaticClass()))
               Cast<ACharacter>(hitActor)->GetMesh()->SetRenderCustomDepth(false);
            if(hitResult.GetActor()->GetClass()->IsChildOf(ACharacter::StaticClass()))
               Cast<ACharacter>(hitResult.GetActor())->GetMesh()->SetRenderCustomDepth(true);
            hitActor = hitResult.GetActor();
         }
      }
   }
}

bool ARTSPawn::IsUnitOnScreen(AUnit* unitToCheck) const
{
   FBox2D unitBoundaryScreenCoords = unitToCheck->FindBoundary();
   return unitBoundaryScreenCoords.Max.X < viewX || unitBoundaryScreenCoords.Max.Y < viewY;
}

void ARTSPawn::SetCameraArmLength(float newLength) const
{
   cameraArm->TargetArmLength = FMath::Clamp(newLength + cameraArm->TargetArmLength, minArmLength, maxArmLength);
}

void ARTSPawn::RecalculateViewportSize(FViewport* viewport, uint32 newSize)
{
   controllerRef->GetViewportSize(viewX, viewY);
   GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("RECALC VIEWPORT with new size: %d %d"), viewX, viewY));
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
   isCamNavDisabled ? controllerRef->GetHUDManager()->GetIngameHUD()->DisplayHelpText(NSLOCTEXT("HelpText", "LockOn", "Camera Lock On"))
                    : controllerRef->GetHUDManager()->GetIngameHUD()->DisplayHelpText(NSLOCTEXT("HelpText", "LockOff", "Camera Lock Off"));
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
   if(!isCamNavDisabled)
      SetActorLocation(GetActorTransform().TransformPosition(FVector(axisValue * baseCameraMoveSpeed * camMoveSpeedMultiplier, 0, 0)));
}

FORCEINLINE void ARTSPawn::MoveY(float axisValue)
{
   if(!isCamNavDisabled)
      SetActorLocation(GetActorTransform().TransformPosition(FVector(0, axisValue * baseCameraMoveSpeed * camMoveSpeedMultiplier, 0)));
}

void ARTSPawn::MMBDragX(float axisValue)
{
   if(controllerRef->IsInputKeyDown(FKey("MiddleMouseButton")) && !controllerRef->IsInputKeyDown(FKey("LeftShift")) &&
      !controllerRef->IsInputKeyDown(FKey("RightShift"))) {
      AddActorLocalOffset(FVector(0, axisValue * -1.f * camMoveSpeedMultiplier * baseCameraMoveSpeed, 0));
   }
}

void ARTSPawn::MMBDragY(float axisValue)
{
   if(controllerRef->IsInputKeyDown(FKey("MiddleMouseButton")) && !controllerRef->IsInputKeyDown(FKey("LeftShift")) &&
      !controllerRef->IsInputKeyDown(FKey("RightShift"))) {
      AddActorLocalOffset(FVector(axisValue * -1.f * camMoveSpeedMultiplier * baseCameraMoveSpeed, 0, 0));
   }
}

void ARTSPawn::PanX(float axisValue)
{
   if(controllerRef->IsInputKeyDown(EKeys::MiddleMouseButton))
      if(controllerRef->IsInputKeyDown(FKey("LeftShift")) || controllerRef->IsInputKeyDown(FKey("RightShift"))) {
         AddActorLocalRotation(FQuat::MakeFromEuler(FVector(0, 0, axisValue * camMoveSpeedMultiplier)));
      }
}

void ARTSPawn::PanY(float axisValue)
{
   if(controllerRef->IsInputKeyDown(EKeys::MiddleMouseButton))
      if(controllerRef->IsInputKeyDown(FKey("LeftShift")) || controllerRef->IsInputKeyDown(FKey("RightShift"))) {
         AddActorLocalRotation(FQuat::MakeFromEuler(FVector(0, axisValue * camMoveSpeedMultiplier, 0)));
      }
}

void ARTSPawn::PanReset()
{
   SetActorRotation(FRotator(0, 180.f, 0));
}

void ARTSPawn::EdgeMovementX(float axisValue)
{
   if(!isCamNavDisabled) {
      float mouseX, mouseY;
      if(controllerRef->GetMousePosition(mouseX, mouseY)) {
         if(mouseX / viewX < .025) {
            AddActorLocalOffset(FVector(0, -1 * baseCameraMoveSpeed * camMoveSpeedMultiplier, 0));
            cursorDirections.AddUnique(ECursorStateEnum::PanLeft);
         } else if(mouseX / viewX > .975) {
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
   if(!isCamNavDisabled) {
      float mouseX, mouseY;
      if(controllerRef->GetMousePosition(mouseX, mouseY)) {
         if(mouseY / viewY < .025) {
            AddActorLocalOffset(FVector(baseCameraMoveSpeed * camMoveSpeedMultiplier, 0, 0));
            cursorDirections.AddUnique(ECursorStateEnum::PanUp);
         } else if(mouseY / viewY > .975) {
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
   for(int i = 0; i < controllerRef->GetBasePlayer()->selectedAllies.Num(); i++) {
      controllerRef->GetBasePlayer()->selectedAllies[i]->GetUnitController()->Stop();
      controllerRef->GetBasePlayer()->selectedAllies[i]->ClearCommandQueue();
   }

   // Cancel whatever secondary cursors we have since we're stopping our action and set hitActor to null to make cursor hover proc
   hasSecondaryCursor = false;
   hitActor           = nullptr;

   for(auto ally : controllerRef->GetBasePlayer()->selectedAllies)
      ally->GetAllyAIController()->StopAutomation();

   HideSpellCircle();
}

void ARTSPawn::PrevFlight()
{
   for(int i = 0; i < controllerRef->GetBasePlayer()->selectedAllies.Num(); i++) {
      Cast<UFlyComponent>(controllerRef->GetBasePlayer()->heroes[i]->GetComponentByClass(UFlyComponent::StaticClass()))->PreviousFlightPathSelected();
   }
}

void ARTSPawn::NextFlight()
{
   for(int i = 0; i < controllerRef->GetBasePlayer()->selectedAllies.Num(); i++) {
      Cast<UFlyComponent>(controllerRef->GetBasePlayer()->heroes[i]->GetComponentByClass(UFlyComponent::StaticClass()))->NextFlightPathSelected();
   }
}

void ARTSPawn::RightClick()
{
   clickFunctionalityClass->HandleRightClick();
}

void ARTSPawn::RightClickShift()
{
   clickFunctionalityClass->HandleShiftRightClick();
}

void ARTSPawn::LeftClick()
{
   clickFunctionalityClass->HandleLeftClick();
}

void ARTSPawn::LeftClickShift()
{
   clickFunctionalityClass->HandleShiftLeftClick();
}

void ARTSPawn::TabNextAlly()
{
   int selectedHeroIndex = 0;
   // If more than one ally is selected, tab through the selection (changing the focused unit and interface but not any selections)
   if(controllerRef->GetBasePlayer()->selectedAllies.Num() > 1) {
      controllerRef->GetBasePlayer()->unitIndex = (controllerRef->GetBasePlayer()->unitIndex + 1) % controllerRef->GetBasePlayer()->selectedAllies.Num();
      // Make sure any other selected heroes are actually in hero array.  If a hero is on the map, it better be in our party else spawn the NPC version of it
      controllerRef->GetHUDManager()->GetActionHUD()->SingleAllyViewIndexFree(controllerRef->GetBasePlayer()->selectedAllies[controllerRef->GetBasePlayer()->unitIndex]);
   } else {
      // Tab through heroes if one/zero allies are selected
      if(controllerRef->GetBasePlayer()->heroes.Num()) {
         if(controllerRef->GetBasePlayer()->selectedHeroes.Num() == 1) { // If we have a hero already selected
            // If a unit dies, the hero index is rearranged in hero->SetSelected() so don't worry about out of bounds
            int prevSelectedHeroIndex = controllerRef->GetBasePlayer()->selectedHeroes[0]->heroIndex;
            // Deselect old unit and get next one in the contiguous hero index vals
            controllerRef->GetBasePlayer()->heroes[prevSelectedHeroIndex]->SetSelected(false);
            selectedHeroIndex = (prevSelectedHeroIndex + 1) % controllerRef->GetBasePlayer()->heroes.Num();
            controllerRef->GetBasePlayer()->heroes[selectedHeroIndex]->SetSelected(true);
            OnAllySelectedDelegate.Broadcast(false);
         } else { // Don't have any hero selected? Select the first one available
            controllerRef->GetBasePlayer()->heroes[selectedHeroIndex]->SetSelected(true);
            OnAllySelectedDelegate.Broadcast(false);
         }
      }
   }
}

void ARTSPawn::AttackMoveInitiate()
{
   if(!controllerRef->GetMyGameInstance()->playerQuickCast)
      SetSecondaryCursor(ECursorStateEnum::AttackMove);
   else {
      FHitResult hitRes;
      if(controllerRef->GetHitResultUnderCursor(SELECTABLE_BY_CLICK_CHANNEL, false, hitRes))
         AttackMoveConfirm(hitRes.Location);
   }
}

void ARTSPawn::UseAbility(int abilityIndex)
{
   controllerRef->GetHUDManager()->GetActionHUD()->UseSkill(abilityIndex);
   //Quick cast enabled
   if(controllerRef->GetMyGameInstance()->playerQuickCast) {
      FHitResult hitRes;
      if(controllerRef->GetHitResultUnderCursor(SELECTABLE_BY_CLICK_CHANNEL, false, hitRes))
         for(AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies) {
            ally->GetAllyAIController()->SetupSpellTargetting(hitRes, ally->abilities[abilityIndex]);
         }
   }
}

void ARTSPawn::AttackMoveConfirm(FVector moveLocation)
{
   if(controllerRef->IsInputKeyDown(EKeys::LeftShift)) {
      for(AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies) {
         ally->QueueAction(TFunction<void()>([ally, moveLocation]() { ally->GetUnitController()->AttackMove(moveLocation); }));
      }
   } else {
      for(AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies) {
         ally->GetUnitController()->AttackMove(moveLocation);
      }
   }
   CreateClickVisual(moveLocation);
   SetSecondaryCursor();
}

void ARTSPawn::SelectControlGroup(int controlIndex)
{
   // If we're holding down a unit to follow don't let us select another group
   if(controlBeingHeldIndex == 0) {
      // If we are double tapping a control selection
      if(lastControlTappedIndex == controlIndex) {
         // Then pan over to the lead unit of the control group and continue to follow it with the camera as long as we hold down the button
         if(controlGroups[lastControlTappedIndex].Num() > 0) {
            controlBeingHeldIndex = lastControlTappedIndex;
            GetWorld()->GetTimerManager().SetTimer(controlDoubleTapHoldHandle, this, &ARTSPawn::ControlGroupDoubleTapHoldFollow, 0.01f, true, 0);
         }
      }

      // Clear whatever units we have selected and select the ones in the control group
      controllerRef->GetBasePlayer()->ClearSelectedAllies();

      for(auto ally : controlGroups[controlIndex]) {
         if(ally.IsValid())
            ally->SetSelected(true);
      }
   }

   // Mark the last control tapped for the double tap check
   lastControlTappedIndex = controlIndex;
}

void ARTSPawn::ControlGroupDoubleTapTimer()
{
   lastControlTappedIndex = 0;
}

void ARTSPawn::ControlGroupDoubleTapHoldFollow()
{
   // Follow the leader of the control group for whatever control group key we're pressing down
   auto allyRef = controlGroups[controlBeingHeldIndex][0];
   if(allyRef.IsValid()) {
      SetActorRotation(FRotator(0, 180, 0));
      const auto [x, y, z]       = allyRef->GetActorLocation();
      cameraArm->TargetArmLength = defaultArmLength + z;
      SetActorLocation(FVector(x + FMath::Tan(FMath::DegreesToRadians(40)) * cameraArm->TargetArmLength, y, z));
   }
}

void ARTSPawn::StopContolGroupFollow(int releaseIndex)
{
   // Stop the camera follow only if we released the right key
   if(releaseIndex == controlBeingHeldIndex) {
      GetWorld()->GetTimerManager().ClearTimer(controlDoubleTapHoldHandle);
      controlBeingHeldIndex = 0;
   }
   // Time limit for the double tap starts after we release the control button
   GetWorld()->GetTimerManager().SetTimer(ControlGroupDoupleTapHandle, this, &ARTSPawn::ControlGroupDoubleTapTimer, 0.15f, false, -1.f);
}

void ARTSPawn::MakeControlGroup(int controlGroupIndex)
{
   auto& allies = controllerRef->GetBasePlayer()->selectedAllies;
   auto& cgroup = controlGroups[controlGroupIndex];

   Algo::Transform(allies, cgroup, [](AAlly* ally) { return MakeWeakObjectPtr(ally); });
}
