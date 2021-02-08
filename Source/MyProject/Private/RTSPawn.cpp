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
#include "ShopNPC.h"
#include "Quests/QuestManager.h"
#include "AbilitySystemComponent.h"
#include "ActionbarInterface.h"
#include "EquipmentMenu.h"
#include "GameplayDelegateContext.h"
#include "HeroAIController.h"
#include "HeroInventory.h"
#include "ItemDelegateContext.h"
#include "ItemManager.h"
#include "ManualSpellComponent.h"
#include "PartyDelegateContext.h"
#include "SceneViewport.h"
#include "Spellbook.h"
#include "MySpell.h"
#include "StorageContainer.h"
#include "StorageInventory.h"
#include "StoreInventory.h"

#include "UIDelegateContext.h"

#include "Algo/Transform.h"
#include "GameBaseHelpers/DefaultCursorClickFunctionality.h"
#include "GameBaseHelpers/ECursorStates.h"

namespace MouseCVars
{
   bool                           bPrintMouseHover = false;
   static FAutoConsoleVariableRef CVarPrintMouseHover(TEXT("printMouseHover"), bPrintMouseHover,
                                                      TEXT("Turn on the capability to print out whatever our mouse is hovering over."));
}

namespace GameplayModifierCVars
{
   bool                           bEnableEnemyControl;
   static FAutoConsoleVariableRef CVarEnableEnemyControl(TEXT("enableEnemyControl"), bEnableEnemyControl,
                                                         TEXT("When this flag is set to 1, we change the control scheme to allow enemy controls"));
}

float const ARTSPawn::maxArmLength     = 4000.f;
float const ARTSPawn::minArmLength     = 250.f;
float const ARTSPawn::defaultArmLength = 2500.f;

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
   return controllerRef->GetBasePlayer()->GetSelectedAllies().Num() > 0;
}

void ARTSPawn::BeginPlay()
{
   if(controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()); controllerRef)
   {
      FViewport::ViewportResizedEvent.AddUObject(this, &ARTSPawn::RecalculateViewportSize);
      controllerRef->GetViewportSize(viewX, viewY);
      clickFunctionalityClass = MakeUnique<DefaultCursorClickFunctionality>(this, controllerRef);
   }
   Super::BeginPlay();
}

void ARTSPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
   Super::EndPlay(EndPlayReason);
}

void ARTSPawn::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);
   CursorHover();
}

void ARTSPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
   Super::SetupPlayerInputComponent(PlayerInputComponent);
   check(InputComponent);

   InputComponent->BindAction("Stop", IE_Pressed, this, &ARTSPawn::Stop);
   InputComponent->BindAction("LockCamera", IE_Pressed, this, &ARTSPawn::LockCamera);
   InputComponent->BindAction("RightClick", IE_Pressed, this, &ARTSPawn::RightClick);
   InputComponent->BindAction("RightClick", IE_Released, this, &ARTSPawn::RightClickReleased);
   InputComponent->BindAction("RightClickShift", IE_Pressed, this, &ARTSPawn::RightClickShift);
   InputComponent->BindAction("LeftClick", IE_Pressed, this, &ARTSPawn::LeftClick);
   InputComponent->BindAction("LeftClickReleased", IE_Released, this, &ARTSPawn::LeftClickReleased);
   InputComponent->BindAction("LeftClickShift", IE_Pressed, this, &ARTSPawn::LeftClickShift);

   InputComponent->BindAction("SelectNext", IE_Pressed, this, &ARTSPawn::TabChangeSelection);
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
   if(controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()); controllerRef)
   {
      controllerRef->GetLocalPlayer()->GetSubsystem<UUIDelegateContext>()->OnItemSlotDroppedInventoryEvent.AddDynamic(this, &ARTSPawn::OnItemSlotDroppedFromInventory);
      controllerRef->GetLocalPlayer()->GetSubsystem<UUIDelegateContext>()->OnItemSlotDroppedStorageEvent.AddDynamic(this, &ARTSPawn::OnItemSlotDroppedFromStorage);
      controllerRef->GetLocalPlayer()->GetSubsystem<UUIDelegateContext>()->OnSkillSlotDroppedEvent.AddDynamic(this, &ARTSPawn::OnSkillSlotDropped);
      controllerRef->GetLocalPlayer()->GetSubsystem<UUIDelegateContext>()->OnSkillSlotDroppedSBEvent.AddDynamic(this, &ARTSPawn::OnSkillSlotDroppedSB);
      controllerRef->GetLocalPlayer()->GetSubsystem<UUIDelegateContext>()->OnSelectionLockToggled().AddUObject(this, &ARTSPawn::ToggleSelectionLock);
      controllerRef->GetLocalPlayer()->GetSubsystem<UUIDelegateContext>()->OnQuickCastSettingToggled().AddUObject(this, &ARTSPawn::ToggleQuickCast);
      controllerRef->GetLocalPlayer()->GetSubsystem<UUIDelegateContext>()->OnStaticFormationToggled().AddUObject(this, &ARTSPawn::ToggleStayInFormation);
      controllerRef->GetLocalPlayer()->GetSubsystem<UUIDelegateContext>()->OnAutoclickToggled().AddUObject(this, &ARTSPawn::ToggleAutoClick);
      controllerRef->GetLocalPlayer()->GetSubsystem<UUIDelegateContext>()->OnEnemySkillSlotClicked().AddUObject(this, &ARTSPawn::OnSkillSlotSelected);

      GetWorld()->GetTimerManager().SetTimerForNextTick([this]() {
         controllerRef->GetHUDManager()->GetIngameHUD()->GetActionbar()->OnSlotSelected().AddUObject(this, &ARTSPawn::OnSkillSlotSelected);
         controllerRef->GetHUDManager()->GetIngameHUD()->GetEquipHUD()->OnSlotSelected().AddUObject(this, &ARTSPawn::OnEquipmentSlotSelected);
         controllerRef->GetHUDManager()->GetIngameHUD()->GetShopHUD()->OnSlotSelected().AddUObject(this, &ARTSPawn::OnShopSlotSelected);
         controllerRef->GetHUDManager()->GetIngameHUD()->GetStorageHUD()->OnStorageInventoryClosed().AddUObject(this, &ARTSPawn::OnStorageInventoryClosed);
         controllerRef->GetHUDManager()->GetIngameHUD()->GetInventoryHUD()->OnSlotSelected().AddUObject(this, &ARTSPawn::OnInventorySlotSelected);
         controllerRef->GetHUDManager()->GetIngameHUD()->GetStorageHUD()->OnSlotSelected().AddUObject(this, &ARTSPawn::OnStorageSlotSelected);
      });
   }

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

bool ARTSPawn::IsSelectionRectActive() const
{
   const FVector2D difference = startMousePos - endMousePos;
   return FMath::Sqrt(FVector2D::DotProduct(difference, difference)) > 10;
}

void ARTSPawn::SetSecondaryCursor(const ECursorStateEnum cursorType)
{
   switch(cursorType)
   {
      case ECursorStateEnum::Magic:
      case ECursorStateEnum::Item:
      case ECursorStateEnum::AttackMove:
         bHasSecondaryCursor = true;
         hitActor            = nullptr;
         ChangeCursor(cursorType);
         break;
      default:
         bHasSecondaryCursor = false;
         hitActor            = nullptr;
         break;
   }
}

void ARTSPawn::ChangeCursor(ECursorStateEnum newCursorState)
{
   if(cursorState != newCursorState)
   {
      cursorState                       = newCursorState;
      controllerRef->CurrentMouseCursor = static_cast<EMouseCursor::Type>(newCursorState);
      FSlateApplication::Get().GetPlatformCursor().Get()->SetType(static_cast<EMouseCursor::Type>(newCursorState));
   }
}

void ARTSPawn::GetHitResultClick(FHitResult& clickHitResult) const
{
   FVector worldLocation, worldDirection;
   if(controllerRef && controllerRef->DeprojectMousePositionToWorld(worldLocation, worldDirection))
   {
      const auto endPos = worldLocation + worldDirection * GetMaxArmLength() * CLICK_TRACE_LENGTH_MULTIPLIER;
      GetWorld()->LineTraceSingleByChannel(clickHitResult, worldLocation, endPos, SELECTABLE_BY_CLICK_CHANNEL);
   }
}

void ARTSPawn::GetHitResultRightClick(FHitResult& clickHitResult) const
{
   FVector worldLocation, worldDirection;
   if(controllerRef->DeprojectMousePositionToWorld(worldLocation, worldDirection))
   {
      const auto                  endPos = worldLocation + worldDirection * GetMaxArmLength() * CLICK_TRACE_LENGTH_MULTIPLIER;
      FCollisionObjectQueryParams queryParams;
      queryParams.AddObjectTypesToQuery(ECC_WorldStatic);
      queryParams.AddObjectTypesToQuery(ENEMY_CHANNEL);
      if(GameplayModifierCVars::bEnableEnemyControl)
      {
         queryParams.AddObjectTypesToQuery(FRIENDLY_CHANNEL);
      }
      GetWorld()->LineTraceSingleByObjectType(clickHitResult, worldLocation, endPos, queryParams);
   }
}

AActor* ARTSPawn::GetHitActorClick(FHitResult& clickHitResult)
{
   GetHitResultClick(clickHitResult);
   return clickHitResult.GetActor();
}

void ARTSPawn::CreateSelectionRect()
{
   if(GetCursorState() != ECursorStateEnum::UI)
   {
      float locX, locY;
      controllerRef->GetMousePosition(locX, locY);
      endMousePos = FVector2D(locX, locY);
      if(IsSelectionRectActive())
      {
         controllerRef->GetHUD()->DrawRect(GetSelectionRectColor(), startMousePos.X, startMousePos.Y, endMousePos.X - startMousePos.X, endMousePos.Y - startMousePos.Y);
         TArray<AUnit*> actorsInSelectRect;
         controllerRef->GetHUD()->GetActorsInSelectionRectangle<AUnit>(startMousePos, endMousePos, actorsInSelectRect, false, false);

         // Only update if we just overlapped a new actor (not one we already have selected)
         if(controllerRef->GetBasePlayer()->GetSelectedUnits().Num() != actorsInSelectRect.Num())
         {
            for(AUnit* unitInSelectionRect : actorsInSelectRect)
            {
               if(IsValid(unitInSelectionRect))
               {
                  if(unitInSelectionRect->IsEnabled() && !unitInSelectionRect->GetUnitSelected())
                  {
                     if(IsUnitOnScreen(unitInSelectionRect))
                     {
                        unitInSelectionRect->SetUnitSelected(true);
                        break;
                     }
                  }
               }
            }
         }
      }
   }
}

FLinearColor ARTSPawn::GetSelectionRectColor() const
{
   return FLinearColor(0.15, 0.57, 0.78, 0.4); // Blue-ish color
}

void ARTSPawn::CursorHover()
{
   if(!bHasSecondaryCursor)
   {
      if(cursorDirections.Num() > 0 && !isCamNavDisabled)
      {
         ChangeCursor(cursorDirections.Last());
         hitActor = nullptr;
         return;
      }

      GetHitResultClick(hitResult);

      if(hitResult.GetActor())
      {
         if(MouseCVars::bPrintMouseHover)
         {
            GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Red, "Hovering over " + hitResult.GetActor()->GetName());
         }

         if(hitResult.GetActor() != hitActor)
         {
            // Remove highlights from last actor
            if(hitActor && hitActor->GetClass()->IsChildOf(ACharacter::StaticClass()))
            {
               Cast<ACharacter>(hitActor)->GetMesh()->SetRenderCustomDepth(false);
            }

            hitActor = hitResult.GetActor();

            // Add highlight to new actor
            if(hitActor && hitActor->GetClass()->IsChildOf(ACharacter::StaticClass()))
            {
               Cast<ACharacter>(hitActor)->GetMesh()->SetRenderCustomDepth(true);
            }

            if(ABasePlayer* basePlayer = controllerRef->GetBasePlayer())
            {
               if(basePlayer->GetSelectedHeroes().Num() > 0)
               {
                  switch(hitResult.GetComponent()->GetCollisionObjectType())
                  {
                     case ECC_WorldStatic: ChangeCursor(ECursorStateEnum::Moving); return;
                     case INTERACTABLE_CHANNEL: ChangeCursor(ECursorStateEnum::Interact); return;
                     case NPC_CHANNEL: ChangeCursor(ECursorStateEnum::Talking); return;
                     default: break;
                  }
               }

               if(GameplayModifierCVars::bEnableEnemyControl)
               {
                  if(basePlayer->GetSelectedUnits().Num() > 0)
                  {
                     const auto selectedEnemy = controllerRef->GetBasePlayer()->GetSelectedUnits().FindByPredicate(
                         [](const AUnit* unit) { return unit->GetClass()->IsChildOf(AEnemy::StaticClass()); });

                     if(selectedEnemy)
                     {
                        if(hitResult.GetComponent()->GetCollisionObjectType() == FRIENDLY_CHANNEL)
                        {
                           ChangeCursor(ECursorStateEnum::Attack);
                           return;
                        }
                     }
                     else
                     {
                        if(hitResult.GetComponent()->GetCollisionObjectType() == ENEMY_CHANNEL)
                        {
                           ChangeCursor(ECursorStateEnum::Attack);
                           return;
                        }
                     }
                  }
               }
               else
               {
                  if(basePlayer->GetSelectedAllies().Num() > 0)
                  {
                     if(hitResult.GetComponent()->GetCollisionObjectType() == ENEMY_CHANNEL)
                     {
                        ChangeCursor(ECursorStateEnum::Attack);
                        return;
                     }
                  }
               }
            }
            ChangeCursor(ECursorStateEnum::Select);
         }
      }
   }
   else
   {
      // If we have a spell or item cursor
      controllerRef->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(SELECTABLE_BY_CLICK_CHANNEL), true, hitResult);
      if(hitResult.GetActor())
      {
         if(hitResult.GetActor() != hitActor)
         {
            if(hitActor && hitActor->GetClass()->IsChildOf(ACharacter::StaticClass()))
            {
               Cast<ACharacter>(hitActor)->GetMesh()->SetRenderCustomDepth(false);
            }
            if(hitResult.GetActor()->GetClass()->IsChildOf(ACharacter::StaticClass()))
            {
               Cast<ACharacter>(hitResult.GetActor())->GetMesh()->SetRenderCustomDepth(true);
            }
            hitActor = hitResult.GetActor();
         }
      }
   }
}

bool ARTSPawn::IsUnitOnScreen(AUnit* unitToCheck) const
{
   const FBox2D unitBoundaryScreenCoords = unitToCheck->FindBoundary();
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
   isCamNavDisabled ? URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("HelpText", "LockOn", "Camera Lock On"))
                    : URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("HelpText", "LockOff", "Camera Lock Off"));
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
   {
      SetActorLocation(GetActorTransform().TransformPosition(FVector(axisValue * baseCameraMoveSpeed * camMoveSpeedMultiplier, 0, 0)));
   }
}

FORCEINLINE void ARTSPawn::MoveY(float axisValue)
{
   if(!isCamNavDisabled)
   {
      SetActorLocation(GetActorTransform().TransformPosition(FVector(0, axisValue * baseCameraMoveSpeed * camMoveSpeedMultiplier, 0)));
   }
}

void ARTSPawn::MMBDragX(float axisValue)
{
   if(controllerRef && controllerRef->IsInputKeyDown(FKey("MiddleMouseButton")) && !controllerRef->IsInputKeyDown(FKey("LeftShift")) &&
      !controllerRef->IsInputKeyDown(FKey("RightShift")))
   {
      AddActorLocalOffset(FVector(0, axisValue * -1.f * camMoveSpeedMultiplier * baseCameraMoveSpeed, 0));
   }
}

void ARTSPawn::MMBDragY(float axisValue)
{
   if(controllerRef && controllerRef->IsInputKeyDown(FKey("MiddleMouseButton")) && !controllerRef->IsInputKeyDown(FKey("LeftShift")) &&
      !controllerRef->IsInputKeyDown(FKey("RightShift")))
   {
      AddActorLocalOffset(FVector(axisValue * -1.f * camMoveSpeedMultiplier * baseCameraMoveSpeed, 0, 0));
   }
}

void ARTSPawn::PanX(float axisValue)
{
   if(controllerRef && controllerRef->IsInputKeyDown(EKeys::MiddleMouseButton))
   {
      if(controllerRef->IsInputKeyDown(FKey("LeftShift")) || controllerRef->IsInputKeyDown(FKey("RightShift")))
      {
         AddActorLocalRotation(FQuat::MakeFromEuler(FVector(0, 0, axisValue * camMoveSpeedMultiplier)));
      }
   }
}

void ARTSPawn::PanY(float axisValue)
{
   if(controllerRef && controllerRef->IsInputKeyDown(EKeys::MiddleMouseButton))
   {
      if(controllerRef->IsInputKeyDown(FKey("LeftShift")) || controllerRef->IsInputKeyDown(FKey("RightShift")))
      {
         AddActorLocalRotation(FQuat::MakeFromEuler(FVector(0, axisValue * camMoveSpeedMultiplier, 0)));
      }
   }
}

void ARTSPawn::PanReset()
{
   SetActorRotation(FRotator(0, 180.f, 0));
}

void ARTSPawn::EdgeMovementX(float axisValue)
{
   if(!isCamNavDisabled)
   {
      float mouseX, mouseY;
      if(controllerRef && controllerRef->GetMousePosition(mouseX, mouseY))
      {
         if(mouseX / viewX < .025)
         {
            AddActorLocalOffset(FVector(0, -1 * baseCameraMoveSpeed * camMoveSpeedMultiplier, 0));
            cursorDirections.AddUnique(ECursorStateEnum::PanLeft);
         }
         else if(mouseX / viewX > .975)
         {
            AddActorLocalOffset(FVector(0, baseCameraMoveSpeed * camMoveSpeedMultiplier, 0));
            cursorDirections.AddUnique(ECursorStateEnum::PanRight);
         }
         else
         {
            cursorDirections.Remove(ECursorStateEnum::PanLeft);
            cursorDirections.Remove(ECursorStateEnum::PanRight);
         }
      }
   }
}

void ARTSPawn::EdgeMovementY(float axisValue)
{
   if(!isCamNavDisabled)
   {
      float mouseX, mouseY;
      if(controllerRef && controllerRef->GetMousePosition(mouseX, mouseY))
      {
         if(mouseY / viewY < .025)
         {
            AddActorLocalOffset(FVector(baseCameraMoveSpeed * camMoveSpeedMultiplier, 0, 0));
            cursorDirections.AddUnique(ECursorStateEnum::PanUp);
         }
         else if(mouseY / viewY > .975)
         {
            AddActorLocalOffset(FVector(-1 * baseCameraMoveSpeed * camMoveSpeedMultiplier, 0, 0));
            cursorDirections.AddUnique(ECursorStateEnum::PanDown);
         }
         else
         {
            cursorDirections.Remove(ECursorStateEnum::PanUp);
            cursorDirections.Remove(ECursorStateEnum::PanDown);
         }
      }
   }
}

void ARTSPawn::Stop()
{
   for(AUnit* selectedUnit : controllerRef->GetBasePlayer()->GetSelectedUnits())
   {
      selectedUnit->GetUnitController()->StopMovement();
   }
   HaltSelectedExceptMovement();
}

void ARTSPawn::HaltSelectedExceptMovement()
{
   for(AUnit* selectedUnit : controllerRef->GetBasePlayer()->GetSelectedUnits())
   {
      selectedUnit->GetUnitController()->StopCurrentAction();
      selectedUnit->GetUnitController()->ClearCommandQueue();
      selectedUnit->GetUnitController()->StopAutomation();
   }

   bHasSecondaryCursor = false;
   HideSpellCircle();
   hitActor = nullptr; // Set hitActor to null to make cursor hover proc
}

void ARTSPawn::RightClick()
{
   if(bAutoClick)
   {
      GetWorld()->GetTimerManager().SetTimer(
          autoClickTimerHandle, [this]() { clickFunctionalityClass->HandleRightClick(); }, 0.1f, true, 0.f);
   }
   else
   {
      clickFunctionalityClass->HandleRightClick();
   }
}

void ARTSPawn::RightClickShift()
{
   if(bAutoClick)
   {
      GetWorld()->GetTimerManager().SetTimer(
          autoClickTimerHandle, [this]() { clickFunctionalityClass->HandleShiftRightClick(); }, 0.1f, true, 0.f);
   }
   else
   {
      clickFunctionalityClass->HandleShiftRightClick();
   }
}

void ARTSPawn::RightClickReleased()
{
   GetWorld()->GetTimerManager().ClearTimer(autoClickTimerHandle);
}

void ARTSPawn::LeftClick()
{
   clickFunctionalityClass->HandleLeftClick();
}

void ARTSPawn::LeftClickReleased()
{
   clickFunctionalityClass->HandleLeftClickRelease();
}

void ARTSPawn::LeftClickShift()
{
   clickFunctionalityClass->HandleShiftLeftClick();
}

void ARTSPawn::TabChangeSelection()
{
   // If more than one ally is selected, tab through the selection (changing the focused unit and interface but not any selections)
   if(controllerRef->GetBasePlayer()->GetSelectedUnits().Num() > 1)
   {
      TabThroughGroup();
   }
   else
   {
      TabSingleSelection();
   }
}

void ARTSPawn::TabThroughGroup()
{
   if(ABasePlayer* basePlayer = controllerRef->GetBasePlayer())
   {
      if(AUnit* focusedUnit = basePlayer->GetFocusedUnit())
      {
         const int selectedUnitIndex    = basePlayer->GetSelectedUnits().Find(focusedUnit);
         const int newSelectedUnitIndex = (selectedUnitIndex + 1) % controllerRef->GetBasePlayer()->GetSelectedUnits().Num();
         AUnit*    newSelectedAlly      = basePlayer->GetSelectedUnits()[newSelectedUnitIndex];
         OnGroupTabbed().Broadcast(newSelectedAlly);
      }
      else
      {
         OnGroupTabbed().Broadcast(basePlayer->GetSelectedUnits()[0]);
      }
   }
}

void ARTSPawn::TabSingleSelection() const
{
   if(ABasePlayer* basePlayer = controllerRef->GetBasePlayer())
   {
      if(basePlayer->GetHeroes().Num() > 0)
      {
         if(AUnit* focusedUnit = basePlayer->GetFocusedUnit())
         {
            if(ABaseHero* focusedHero = Cast<ABaseHero>(focusedUnit))
            {
               focusedHero->SetUnitSelected(false);
               const int  selectedUnitIndex    = basePlayer->GetHeroes().Find(focusedHero);
               const int  newSelectedUnitIndex = (selectedUnitIndex + 1) % basePlayer->GetHeroes().Num();
               ABaseHero* newSelectedHero      = basePlayer->GetHeroes()[newSelectedUnitIndex];
               newSelectedHero->SetUnitSelected(true);
               OnGroupTabbed().Broadcast(newSelectedHero);
            }
         }
         else
         {
            // Don't have any hero selected? Select the first one available
            ABaseHero* newSelectedHero = basePlayer->GetHeroes()[0];
            newSelectedHero->SetUnitSelected(true);
            OnGroupTabbed().Broadcast(newSelectedHero);
         }
      }
   }
}

void ARTSPawn::AttackMoveInitiate()
{
   SetSecondaryCursor(ECursorStateEnum::AttackMove);
   if(!bQuickCast)
   {
      LeftClick();
   }
}

void ARTSPawn::UseAbility(int abilityIndex)
{
   controllerRef->GetLocalPlayer()->GetSubsystem<UGameplayDelegateContext>()->OnSkillActivated().Broadcast(abilityIndex);

   // TODO: Quickcast for Vector Targeting
   if(bQuickCast)
   {
      FHitResult hitRes;
      if(controllerRef->GetHitResultUnderCursor(SELECTABLE_BY_CLICK_CHANNEL, false, hitRes))
      {
         for(AUnit* unit : controllerRef->GetBasePlayer()->GetSelectedUnits())
         {
            if(UManualSpellComponent* manSpellComp = unit->GetUnitController()->FindComponentByClass<UManualSpellComponent>())
            {
               manSpellComp->OnSpellConfirmInput(hitRes);
            }
         }
      }
   }
}

void ARTSPawn::SelectControlGroup(int controlIndex)
{
   // If we're holding down a unit to follow don't let us select another group
   if(controlBeingHeldIndex == 0)
   {
      // If we are double tapping a control selection
      if(lastControlTappedIndex == controlIndex)
      {
         // Then pan over to the lead unit of the control group and continue to follow it with the camera as long as we hold down the button
         if(controlGroups[lastControlTappedIndex].Num() > 0)
         {
            controlBeingHeldIndex = lastControlTappedIndex;
            GetWorld()->GetTimerManager().SetTimer(controlDoubleTapHoldHandle, this, &ARTSPawn::ControlGroupDoubleTapHoldFollow, 0.01f, true, 0);
         }
      }

      // Clear whatever units we have selected and select the ones in the control group
      controllerRef->GetBasePlayer()->ClearSelectedUnits();

      for(auto ally : controlGroups[controlIndex])
      {
         if(ally.IsValid())
         {
            ally->SetUnitSelected(true);
         }
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
   const auto allyRef = controlGroups[controlBeingHeldIndex][0];
   if(allyRef.IsValid())
   {
      SetActorRotation(FRotator(0, 180, 0));
      const auto [x, y, z]       = allyRef->GetActorLocation();
      cameraArm->TargetArmLength = defaultArmLength + z;
      SetActorLocation(FVector(x + FMath::Tan(FMath::DegreesToRadians(40)) * cameraArm->TargetArmLength, y, z));
   }
}

void ARTSPawn::StopContolGroupFollow(int releaseIndex)
{
   // Stop the camera follow only if we released the right key
   if(releaseIndex == controlBeingHeldIndex)
   {
      GetWorld()->GetTimerManager().ClearTimer(controlDoubleTapHoldHandle);
      controlBeingHeldIndex = 0;
   }
   // Time limit for the double tap starts after we release the control button
   GetWorld()->GetTimerManager().SetTimer(ControlGroupDoupleTapHandle, this, &ARTSPawn::ControlGroupDoubleTapTimer, 0.15f, false, -1.f);
}

void ARTSPawn::MakeControlGroup(int controlGroupIndex)
{
   auto& allies = controllerRef->GetBasePlayer()->GetSelectedAllies();
   auto& cgroup = controlGroups[controlGroupIndex];

   Algo::Transform(allies, cgroup, [](AAlly* ally) { return MakeWeakObjectPtr(ally); });
}

void ARTSPawn::OnSkillSlotSelected(int skillIndex)
{
   if(const AUnit* focusedUnit = controllerRef->GetBasePlayer()->GetFocusedUnit())
   {
      if(UManualSpellComponent* manualSpellComp = focusedUnit->GetUnitController()->FindComponentByClass<UManualSpellComponent>())
      {
         manualSpellComp->PressedCastSpell(skillIndex);
      }
   }
}

void ARTSPawn::OnInventorySlotSelected(int slotIndex)
{
   if(ABaseHero* heroWithInvShown = controllerRef->GetBasePlayer()->GetHeroes()[controllerRef->GetHUDManager()->GetIngameHUD()->GetInventoryHUD()->GetHeroIndex()])
   {
      if(const FMyItem itemUsed = heroWithInvShown->GetBackpack().GetItem(slotIndex))
      {
         if(controllerRef->GetHUDManager()->IsWidgetOnScreen(EHUDs::HS_Storage))
         {
            HandleTransferStorageItems(heroWithInvShown, slotIndex, itemUsed);
         }
         else if(controllerRef->GetHUDManager()->IsWidgetOnScreen(EHUDs::HS_Shop_General))
         {
            HandleSellItemToStore(heroWithInvShown, slotIndex, itemUsed);
         }
         else
         {
            HandleInventoryItemSelected(heroWithInvShown, slotIndex, itemUsed);
         }
      }
   }
}

void ARTSPawn::HandleInventoryItemSelected(ABaseHero* heroWithInvShown, int itemUsedSlotIndex, const FMyItem itemUsed) const
{
   const FGameplayTag itemType = UItemManager::Get().GetItemInfo(itemUsed.id)->itemType;

   if(itemType.MatchesTag(UGameplayTagsManager::Get().RequestGameplayTag("Item.Equippable")))
   {
      heroWithInvShown->Equip(itemUsedSlotIndex);
   }
   else if(itemType.MatchesTag(UGameplayTagsManager::Get().RequestGameplayTag("Item.Consumeable")))
   {
      heroWithInvShown->GetHeroController()->BeginUseItem(itemUsed.id, itemUsedSlotIndex);
   }
   else
   {
      UE_LOG(LogTemp, Log, TEXT("ERROR WITH ITEM TYPE"));
   }
}

void ARTSPawn::HandleTransferStorageItems(ABaseHero* heroWithInvShown, int itemUsedSlotIndex, FMyItem itemToDeposit) const
{
   UBackpack* originalBackpack   = &heroWithInvShown->GetBackpack();
   UBackpack* transferToBackpack = Cast<AStorageContainer>(heroWithInvShown->GetCurrentInteractable())->GetBackpack();

   if(originalBackpack && transferToBackpack)
   {
      auto  itemTransferResults = transferToBackpack->TransferItems(originalBackpack, itemUsedSlotIndex);
      auto& removeResult        = itemTransferResults.Key;
      auto& addResult           = itemTransferResults.Value;
      GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnItemsTransferred().Broadcast(*originalBackpack, *transferToBackpack,
                                                                                                                            removeResult, addResult);
   }
}

void ARTSPawn::HandleSellItemToStore(ABaseHero* heroWithInvShown, int itemUsedSlotIndex, FMyItem itemToDeposit) const
{
   // TODO: Calculate price and trigger some kind of event
   //Cast<AShopNPC>(GetCurrentInteractable())->
}

void ARTSPawn::OnStorageSlotSelected(int slotIndex)
{
   if(ABaseHero* heroUsingStorage = controllerRef->GetBasePlayer()->heroInBlockingInteraction)
   {
      if(AStorageContainer* storageContainer = Cast<AStorageContainer>(heroUsingStorage->GetCurrentInteractable()))
      {
         UBackpack* originalBackpack   = Cast<AStorageContainer>(heroUsingStorage->GetCurrentInteractable())->GetBackpack();
         UBackpack* transferToBackpack = &heroUsingStorage->GetBackpack();

         auto  itemTransferResults = transferToBackpack->TransferItems(originalBackpack, slotIndex);
         auto& removeResult        = itemTransferResults.Key;
         auto& addResult           = itemTransferResults.Value;
         GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnItemsTransferred().Broadcast(*originalBackpack, *transferToBackpack,
                                                                                                                               removeResult, addResult);
      }
   }
}

void ARTSPawn::OnEquipmentSlotSelected(int slotIndex)
{
   controllerRef->GetHUDManager()->GetIngameHUD()->GetEquipHUD()->GetEquippedHero()->Unequip(slotIndex);
}

void ARTSPawn::OnShopSlotSelected(int slotIndex)
{
   Cast<AShopNPC>(controllerRef->GetBasePlayer()->heroInBlockingInteraction->GetCurrentInteractable())->OnAskToPurchaseItem(slotIndex);
}

void ARTSPawn::OnStorageInventoryClosed()
{
   controllerRef->GetBasePlayer()->heroInBlockingInteraction = nullptr;
}

void ARTSPawn::OnItemSlotDroppedFromInventory(int dragSlotIndex, int dropSlotIndex, UBackpack* dragPack, UBackpack* dropPack)
{
   UBackpack::SwapItems(dragPack, dropPack, dragSlotIndex, dropSlotIndex);
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnItemsSwapped().Broadcast(*dragPack, *dropPack, dragSlotIndex, dropSlotIndex);
}

void ARTSPawn::OnItemSlotDroppedFromStorage(int dragSlotIndex, int dropSlotIndex, UBackpack* dragPack, UBackpack* dropPack)
{
   UBackpack::SwapItems(dragPack, dropPack, dragSlotIndex, dropSlotIndex);
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnItemsSwapped().Broadcast(*dragPack, *dropPack, dragSlotIndex, dropSlotIndex);
}

void ARTSPawn::OnSkillSlotDropped(int dragSlotIndex, int dropSlotIndex)
{
   if(AUserInput* CPCRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()))
   {
      if(ABasePlayer* basePlayer = CPCRef->GetBasePlayer())
      {
         if(AUnit* focusedUnit = basePlayer->GetFocusedUnit())
         {
            URTSAbilitySystemComponent* focusedABComp = focusedUnit->GetAbilitySystemComponent();
            const TSubclassOf<UMySpell> droppedSpell  = focusedABComp->GetSpellAtSlot(dropSlotIndex);

            focusedABComp->SetSpellAtSlot(focusedABComp->GetSpellAtSlot(dragSlotIndex), dropSlotIndex);
            focusedABComp->SetSpellAtSlot(droppedSpell, dragSlotIndex);
         }
      }
   }
}

void ARTSPawn::OnSkillSlotDroppedSB(int dragSlotIndex, int dropSlotIndex)
{
   if(AUserInput* CPCRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()))
   {
      if(ABasePlayer* basePlayer = CPCRef->GetBasePlayer())
      {
         if(ABaseHero* focusedHero = Cast<ABaseHero>(basePlayer->GetFocusedUnit()))
         {
            const TSubclassOf<UMySpell> spellClass    = focusedHero->GetSpellBook()->GetSpellFromIndex(dragSlotIndex);
            URTSAbilitySystemComponent* focusedABComp = focusedHero->GetAbilitySystemComponent();

            focusedABComp->SetSpellAtSlot(spellClass, dropSlotIndex);
         }
      }
   }
}
