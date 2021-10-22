#include "MyProject.h"

#include "UserInput.h"
#include "RTSGameState.h"
#include "RTSPawn.h"

#include "Unit.h"

#include "UpResourceManager.h"

#include "State/RTSStateMachine.h"

#include "AIStuff/AIControllers/UnitController.h"

#include "UI/Healthbar/HealthbarComp.h"
#include "UI/HUDManager.h"
#include "UI/UserWidgets/RTSDamageNumberContainer.h"

#include "SpellSystem/RTSAbilitySystemComponent.h"
#include "SpellSystem/MySpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BasePlayer.h"
#include "DamageNumberWidgetComponent.h"

#include "MyCharacterMovementComponent.h"
#include "PartyDelegateContext.h"
#include "RTSGlobalCVars.h"

#include "RTSStateComponent.h"
#include "RTSVisionComponent.h"

#include "UpStatComponent.h"
#include "TargetComponent.h"

DEFINE_LOG_CATEGORY(Up_Log_Combat);

AUnit::AUnit(const FObjectInitializer& objectInitializer) :
    Super(objectInitializer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(CharacterMovementComponentName))
{
   PrimaryActorTick.bCanEverTick = true;
   AutoPossessAI                 = EAutoPossessAI::PlacedInWorldOrSpawned;

   RemoveArrowComponent();

   abilitySystemComponent = CreateDefaultSubobject<URTSAbilitySystemComponent>(TEXT("AbilitySystem"));

   statComponent   = CreateDefaultSubobject<UUpStatComponent>(TEXT("Stats"));
   targetComponent = CreateDefaultSubobject<UTargetComponent>(TEXT("Targeting"));

   visionComponent = CreateDefaultSubobject<URTSVisionComponent>(TEXT("VisionRadius"));
   visionComponent->SetupAttachment(RootComponent);
   visionComponent->SetRelativeLocation(FVector::ZeroVector);
   visionComponent->bUseAttachParentBound = true;

   selectionCircleDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("CircleShadowBounds"));
   selectionCircleDecal->SetupAttachment(RootComponent);
   selectionCircleDecal->bUseAttachParentBound = true;

   SetupSelectionCircle();

   SetupHealthbarComponent();

   SetupCharacterCollision();

   SetupMovementComponent();

   SetupDamageInidicatorContainerWidget();

   combatInfo = MakeUnique<UpCombatInfo>();

   // Turn this off to make sure the unit highlights when hovered over
   GetMesh()->SetRenderCustomDepth(false);
   GetMesh()->bUseAttachParentBound = true;
}

void AUnit::SetupHealthbarComponent()
{
   healthBar = CreateDefaultSubobject<UHealthbarComp>("Healthbar");
   healthBar->SetRelativeLocation(FVector(0, 0, 180));
   healthBar->SetPivot(FVector2D(0.5, 1));
   healthBar->SetupAttachment(RootComponent);
   healthBar->SetDrawAtDesiredSize(true);
   healthBar->SetCanEverAffectNavigation(false);
   healthBar->bUseAttachParentBound = true;
   ConstructorHelpers::FClassFinder<UUserWidget> healthBarWig(TEXT("/Game/RTS_Tutorial/HUDs/ActionUI/Hitpoints/Up_W_HealthbarWidget"));
   if(healthBarWig.Succeeded())
   {
      healthBar->SetWidgetClass(healthBarWig.Class);
   }
}

void AUnit::SetupCharacterCollision() const
{
   // Mesh needs an offset because it isn't aligned with capsule component at the beginning.  Offset by the mesh size
   GetMesh()->SetRelativeLocation(FVector(0, 0, GetDefaultHalfHeight() * 2));
   GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

   GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
   GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
   GetCapsuleComponent()->SetCollisionResponseToChannel(SELECTABLE_BY_CLICK_CHANNEL, ECR_Block);
   GetCapsuleComponent()->SetSimulatePhysics(false); // can't move w/o physics
   // GetCapsuleComponent()->SetCanEverAffectNavigation(true);

   GetCharacterMovement()->bEnablePhysicsInteraction = false;
   GetCharacterMovement()->bPushForceScaledToMass    = false;
   GetCharacterMovement()->PushForceFactor           = 0;
}

void AUnit::SetupMovementComponent() const
{
   // Allows units to step up stairs.  The height of the stairs they can step is set in some navmesh params
   GetCharacterMovement()->SetWalkableFloorAngle(90.f);
   GetCharacterMovement()->RotationRate              = FRotator(0, 300.f, 0);
   GetCharacterMovement()->bOrientRotationToMovement = true;
   GetCharacterMovement()->JumpOffJumpZFactor        = 0;
}

void AUnit::SetupDamageInidicatorContainerWidget()
{
   damageIndicatorWidget = CreateDefaultSubobject<UDamageNumberWidgetComponent>("DamageNumbers");
   damageIndicatorWidget->SetDrawAtDesiredSize(true);
   damageIndicatorWidget->SetRelativeLocation(FVector(0, 0, 180));
   damageIndicatorWidget->SetupAttachment(RootComponent);
   damageIndicatorWidget->SetInitialSharedLayerName("DamageIndicatorLayer");
   damageIndicatorWidget->SetInitialLayerZOrder(1);
   damageIndicatorWidget->bUseAttachParentBound = true;
   damageIndicatorWidget->SetCanEverAffectNavigation(false);
}

void AUnit::RemoveArrowComponent() const
{
   // Destroy arrow component so there isn't some random arrow sticking out of our units
   if(UArrowComponent* arrowComp = FindComponentByClass<UArrowComponent>())
   {
      arrowComp->DestroyComponent();
   }
}

void AUnit::SetupSelectionCircle() const
{
   if(selectionCircleDecal)
   {
      const float ScaledCapsuleRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
      selectionCircleDecal->DecalSize = FVector(30.f, ScaledCapsuleRadius, ScaledCapsuleRadius);
      selectionCircleDecal->SetUsingAbsoluteScale(false);
      selectionCircleDecal->SetRelativeScale3D(FVector::OneVector);
      selectionCircleDecal->SetRelativeRotation(FRotator(90, 0, 0));
      selectionCircleDecal->SetUsingAbsoluteLocation(false);
      selectionCircleDecal->SetRelativeLocation(FVector(0, 0, -90));
      selectionCircleDecal->bUseAttachParentBound = true;
      selectionCircleDecal->SetComponentTickEnabled(false);
   }
}

void AUnit::StoreUnitHeight()
{
   FVector origin, extent;
   GetActorBounds(true, origin, extent);
   unitProperties.height = FMath::Abs(origin.Z) + extent.Z - FMath::Abs(GetActorLocation().Z);
}

void AUnit::BeginPlay()
{
   Super::BeginPlay();
   // GetCapsuleComponent()->SetCanEverAffectNavigation(true);

   if((!HasAuthority() || !IsRunningDedicatedServer()) && UNLIKELY(!IsValid(controllerRef)))
   {
      controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
   }

   if(HasAuthority() || !IsRunningDedicatedServer())
   {
      GetWorld()->GetTimerManager().SetTimerForNextTick(
          [this]()
          {
             damageIndicatorWidget->SetWidgetClass(controllerRef->GetHUDManager()->damageIndicatorContainerClass);
             URTSDamageNumberContainer* damageNumberContainerWidget = Cast<URTSDamageNumberContainer>(damageIndicatorWidget->GetUserWidgetObject());
             damageIndicatorWidget->SetOwnerPlayer(controllerRef->GetLocalPlayer());
             if(damageNumberContainerWidget)
             {
                damageNumberContainerWidget->SetOwningUnit(this);
             }
             damageIndicatorWidget->SetRelativeLocation(FVector::ZeroVector);
          });
   }

   StoreUnitHeight();

   if(const ARTSGameState* gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState()))
   {
      gameStateRef->OnGameSpeedUpdated().AddDynamic(this, &AUnit::OnUpdateGameSpeed);
   }

   GetCharacterMovement()->MaxWalkSpeed = statComponent->GetMechanicAdjValue(EMechanics::MovementSpeed);
   damageIndicatorWidget->SetPivot(FVector2D(0.5f, 1.f + 0.25f * GetSimpleCollisionHalfHeight() / 88.f));
   visionComponent->SetRelativeLocation(FVector::ZeroVector);
}

void AUnit::PossessedBy(AController* newController)
{
   Super::PossessedBy(newController);
   unitController = Cast<AUnitController>(newController);

   if(!IsEnabled())
   {
      // Delay since BasePlayer's Unit registration delegates may not be set
      GetWorld()->GetTimerManager().SetTimerForNextTick(
          [this]()
          {
             SetEnabled(false);
          });
   }
}

#if WITH_EDITOR
void AUnit::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
   Super::PostEditChangeProperty(PropertyChangedEvent);
   if(PropertyChangedEvent.Property != nullptr && PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(AUnit, unitControllerClass))
   {
      AIControllerClass = unitControllerClass;
   }
}
#endif

void AUnit::HideInvisibleUnits()
{
   if(LIKELY(!GameplayModifierCVars::bShowEnemyPerspective))
   {
      if(GetIsEnemy())
      {
         if(GetIsUnitHidden())
         {
            SetActorHiddenInGame(true);
            GetCapsuleComponent()->SetCollisionResponseToChannel(SELECTABLE_BY_CLICK_CHANNEL, ECR_Ignore);
         }
         else
         {
            SetActorHiddenInGame(false);
            GetCapsuleComponent()->SetCollisionResponseToChannel(SELECTABLE_BY_CLICK_CHANNEL, ECR_Block);
         }
      }
   }
   else
   {
      if(!GetIsEnemy())
      {
         if(GetIsUnitHidden())
         {
            SetActorHiddenInGame(true);
            GetCapsuleComponent()->SetCollisionResponseToChannel(SELECTABLE_BY_CLICK_CHANNEL, ECR_Ignore);
         }
         else
         {
            SetActorHiddenInGame(false);
            GetCapsuleComponent()->SetCollisionResponseToChannel(SELECTABLE_BY_CLICK_CHANNEL, ECR_Block);
         }
      }
   }
}

void AUnit::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);
   HideInvisibleUnits();
}

void AUnit::SetEnabled(bool bEnabled)
{
   if(bEnabled)
   {
      if(GetUnitSelected())
      {
         SetUnitSelected(false);
      }
      GetCapsuleComponent()->SetVisibility(true, true);
      SetActorEnableCollision(true);
      SetActorTickEnabled(true);
      GetCapsuleComponent()->SetEnableGravity(true);
      GetCharacterMovement()->GravityScale = 1;
      bCanAffectNavigationGeneration       = true;
      unitProperties.bIsEnabled            = true;
      damageIndicatorWidget->SetActive(true);
   }
   else
   {
      if(AUnitController* UnitController = GetUnitController())
      {
         UnitController->StopCurrentAction();
         UnitController->StopAutomation();
         GetCapsuleComponent()->SetVisibility(false, true);
         SetActorEnableCollision(false);
         SetActorTickEnabled(false);
         GetCapsuleComponent()->SetEnableGravity(false);
         GetCharacterMovement()->GravityScale = 0;
         GetCapsuleComponent()->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
         bCanAffectNavigationGeneration = false;
         unitProperties.bIsEnabled      = false;
         damageIndicatorWidget->SetActive(false);
      }
   }
}

void AUnit::OnUpdateGameSpeed(float speedMultiplier)
{
   // GetCharacterMovement()->MaxWalkSpeed = statComponent->GetMechanicAdjValue(EMechanics::MovementSpeed) * speedMultiplier;
}

void AUnit::SetUnitSelected(bool value)
{
   unitProperties.isSelected = value;
   if(value)
   {
      if(controllerRef->GetBasePlayer()->GetSelectedUnits().Num() < 16)
      {
         controllerRef->GetBasePlayer()->AddSelectedUnit(this);
         controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnUnitSelectedDelegate.Broadcast();
      }
   }
   else
   {
      controllerRef->GetBasePlayer()->RemoveSelectedUnit(this);
      controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnUnitDeselectedDelegate.Broadcast(this);
   }
}

bool AUnit::GetIsDead() const
{
   return combatInfo->isDead;
}

FBox2D AUnit::FindBoundary() const
{
   // This function can have errors due to being called during level transition before old level is completely unloaded
   FBox2D  boundary = FBox2D(ForceInit);
   FVector origin, extent;
   GetActorBounds(true, origin, extent);
   int sizeX, sizeY;
   controllerRef->GetViewportSize(sizeX, sizeY);
   static const int                                 CORNER_COUNT = 8;
   TArray<FVector2D, TFixedAllocator<CORNER_COUNT>> corners; // Get 8 corners of box

   for(int i = 0; i < CORNER_COUNT; ++i)
   {
      FVector2D screenLocation = FVector2D::ZeroVector;
      controllerRef->ProjectWorldLocationToScreen(origin + extent * UpResourceManager::BoundsPointMapping[i], screenLocation, false);

      if(screenLocation.X > 0 && screenLocation.X < sizeX && screenLocation.Y > 0 && screenLocation.Y < sizeY)
      {
         corners.Add(screenLocation);
         boundary += screenLocation;
      }
   }
   return boundary;
}
