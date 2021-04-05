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
<<<<<<< HEAD
=======
#include "UI/UserWidgets/RTSDamageNumberContainer.h"
>>>>>>> componentrefactor

#include "SpellSystem/RTSAbilitySystemComponent.h"
#include "SpellSystem/MySpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BasePlayer.h"

#include "MyCharacterMovementComponent.h"
#include "PartyDelegateContext.h"
#include "RTSGlobalCVars.h"

<<<<<<< HEAD
#include "BrainComponent.h"
=======
>>>>>>> componentrefactor
#include "RTSStateComponent.h"
#include "RTSVisionComponent.h"

#include "UpStatComponent.h"
<<<<<<< HEAD
=======
#include "TargetComponent.h"
>>>>>>> componentrefactor

AUnit::AUnit(const FObjectInitializer& objectInitializer) :
    Super(objectInitializer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(CharacterMovementComponentName))
{
   PrimaryActorTick.bCanEverTick = true;
   AutoPossessAI                 = EAutoPossessAI::PlacedInWorldOrSpawned;

   RemoveArrowComponent();
<<<<<<< HEAD

   abilitySystemComponent = CreateDefaultSubobject<URTSAbilitySystemComponent>(TEXT("AbilitySystem"));

   visionComponent = CreateDefaultSubobject<URTSVisionComponent>(FName("VisionRadius"));
   visionComponent->SetupAttachment(RootComponent);
=======

   abilitySystemComponent = CreateDefaultSubobject<URTSAbilitySystemComponent>(TEXT("AbilitySystem"));

   statComponent   = CreateDefaultSubobject<UUpStatComponent>(TEXT("Stats"));
   targetComponent = CreateDefaultSubobject<UTargetComponent>(TEXT("Targeting"));

   visionComponent = CreateDefaultSubobject<URTSVisionComponent>(TEXT("VisionRadius"));
   visionComponent->SetupAttachment(RootComponent);
   visionComponent->SetRelativeLocation(FVector::ZeroVector);
   visionComponent->bUseAttachParentBound = true;
>>>>>>> componentrefactor

   selectionCircleDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("CircleShadowBounds"));
   selectionCircleDecal->SetupAttachment(RootComponent);
   selectionCircleDecal->bUseAttachParentBound = true;

   SetupHealthbarComponent();

   SetupCharacterCollision();

   SetupMovementComponent();

   SetupDamageInidicatorContainerWidget();

<<<<<<< HEAD
   SetupHealthbarComponent();

   SetupCharacterCollision();

   SetupMovementComponent();

   combatInfo                  = MakeUnique<UpCombatInfo>();
   combatInfo->combatStyle     = ECombatType::Melee;
   
   // Turn this off to make sure the unit highlights when hovered over
   GetMesh()->SetRenderCustomDepth(false);
=======
   combatInfo              = MakeUnique<UpCombatInfo>();
   combatInfo->combatStyle = ECombatType::Melee;

   // Turn this off to make sure the unit highlights when hovered over
   GetMesh()->SetRenderCustomDepth(false);
   GetMesh()->bUseAttachParentBound = true;
>>>>>>> componentrefactor
}

AUnit::~AUnit() = default;

void AUnit::SetupHealthbarComponent()
{
   healthBar = CreateDefaultSubobject<UHealthbarComp>("Healthbar");
   healthBar->SetRelativeLocation(FVector(0, 0, 180));
   healthBar->SetPivot(FVector2D(0.5, 1));
   healthBar->SetupAttachment(RootComponent);
   healthBar->SetDrawAtDesiredSize(true);
<<<<<<< HEAD

   ConstructorHelpers::FClassFinder<UUserWidget> healthBarWig(TEXT("/Game/RTS_Tutorial/HUDs/ActionUI/Hitpoints/HealthbarWidget"));
   if(healthBarWig.Succeeded()) { healthBar->SetWidgetClass(healthBarWig.Class); }
=======
   healthBar->SetCanEverAffectNavigation(false);
   healthBar->bUseAttachParentBound = true;
   ConstructorHelpers::FClassFinder<UUserWidget> healthBarWig(TEXT("/Game/RTS_Tutorial/HUDs/ActionUI/Hitpoints/Up_W_HealthbarWidget"));
   if(healthBarWig.Succeeded())
   {
      healthBar->SetWidgetClass(healthBarWig.Class);
   }
>>>>>>> componentrefactor
}

void AUnit::SetupCharacterCollision() const
{
   // Mesh needs an offset because it isn't aligned with capsule component at the beginning.  Offset by the mesh size
<<<<<<< HEAD
   GetMesh()->SetRelativeLocation(FVector(0, 0, -GetMesh()->Bounds.BoxExtent.Z));
   GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
   GetCapsuleComponent()->SetCollisionResponseToChannel(SELECTABLE_BY_CLICK_CHANNEL, ECR_Block);
}

=======
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

>>>>>>> componentrefactor
void AUnit::SetupMovementComponent() const
{
   // Allows units to step up stairs.  The height of the stairs they can step is set in some navmesh params
   GetCharacterMovement()->SetWalkableFloorAngle(90.f);
   GetCharacterMovement()->RotationRate              = FRotator(0, 300.f, 0);
   GetCharacterMovement()->bOrientRotationToMovement = true;
<<<<<<< HEAD
}

void AUnit::RemoveArrowComponent() const
{
   // Destroy arrow component so there isn't some random arrow sticking out of our units
   GetComponentByClass(UArrowComponent::StaticClass())->DestroyComponent();
}

void AUnit::SetupAbilitiesAndStats()
{
   if(GetAbilitySystemComponent()) {
      // ! Make sure owner is player controller else the whole ability system fails to function (maybe it should be set to RTSPawn I'll have to double check)
      GetAbilitySystemComponent()->InitAbilityActorInfo(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController(), this); // setup owner and avatar
      GetCharacterMovement()->MaxWalkSpeed = statComponent->GetMechanicAdjValue(EMechanics::MovementSpeed);

      for(TSubclassOf<UMySpell> ability : GetAbilitySystemComponent()->GetAbilities()) {
         if(ability.GetDefaultObject()) // if client tries to give himself ability assert fails
         {
            GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(ability.GetDefaultObject(), 1));
         }
      }
   }
}

void AUnit::AlignSelectionCircleWithGround() const
{
   if(selectionCircleDecal) {
=======
   GetCharacterMovement()->JumpOffJumpZFactor        = 0;
}

void AUnit::SetupDamageInidicatorContainerWidget()
{
   damageIndicatorWidget = CreateDefaultSubobject<UWidgetComponent>("DamageNumbers");
   damageIndicatorWidget->SetWidgetSpace(EWidgetSpace::Screen);
   damageIndicatorWidget->SetDrawSize(FVector2D(200, 200));
   damageIndicatorWidget->SetPivot(FVector2D(0.5f, 0.5f));
   damageIndicatorWidget->SetManuallyRedraw(false);
   damageIndicatorWidget->SetRedrawTime(0.f);
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
>>>>>>> componentrefactor
      selectionCircleDecal->DecalSize = FVector(GetCapsuleComponent()->GetScaledCapsuleRadius());
      selectionCircleDecal->SetUsingAbsoluteScale(true);
      selectionCircleDecal->SetWorldScale3D(FVector::OneVector);
      selectionCircleDecal->SetRelativeRotation(FRotator(90, 0, 0));
      selectionCircleDecal->SetRelativeLocation(FVector(0, 0, -90));
      selectionCircleDecal->bUseAttachParentBound = true;
   }
}

void AUnit::StoreUnitHeight()
{
   FVector origin, extent;
   GetActorBounds(true, origin, extent);
   unitProperties.height = FMath::Abs(origin.Z) + extent.Z - FMath::Abs(GetActorLocation().Z);
}
<<<<<<< HEAD

void AUnit::BeginPlay()
{
   if(UNLIKELY(!IsValid(controllerRef))) controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());

   StoreUnitHeight();

   AlignSelectionCircleWithGround();

   if(auto gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState())) { gameStateRef->OnGameSpeedUpdated().AddDynamic(this, &AUnit::OnUpdateGameSpeed); }
   
   SetupAbilitiesAndStats();
=======

void AUnit::BeginPlay()
{
   Super::BeginPlay();
   // GetCapsuleComponent()->SetCanEverAffectNavigation(true);

   if(UNLIKELY(!IsValid(controllerRef)))
   {
      controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
   }

   GetWorld()->GetTimerManager().SetTimerForNextTick([this]() {
      damageIndicatorWidget->SetWidgetClass(controllerRef->GetHUDManager()->damageIndicatorContainerClass);
      URTSDamageNumberContainer* damageNumberContainerWidget = Cast<URTSDamageNumberContainer>(damageIndicatorWidget->GetUserWidgetObject());
      damageIndicatorWidget->SetOwnerPlayer(controllerRef->GetLocalPlayer());
      if(damageNumberContainerWidget)
      {
         damageNumberContainerWidget->SetOwningUnit(this);
      }
      damageIndicatorWidget->SetRelativeLocation(FVector::ZeroVector);
   });

   StoreUnitHeight();
   SetupSelectionCircle();

   if(const ARTSGameState* gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState()))
   {
      gameStateRef->OnGameSpeedUpdated().AddDynamic(this, &AUnit::OnUpdateGameSpeed);
   }

   GetCharacterMovement()->MaxWalkSpeed = statComponent->GetMechanicAdjValue(EMechanics::MovementSpeed);
>>>>>>> componentrefactor

   visionComponent->SetRelativeLocation(FVector::ZeroVector);

   if(!IsEnabled())
   {
      SetEnabled(false);
   }
}

void AUnit::PossessedBy(AController* newController)
{
   Super::PossessedBy(newController);
   unitController = Cast<AUnitController>(newController);
}

<<<<<<< HEAD
void AUnit::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);
}

EUnitState AUnit::GetState() const
{
   return FindComponentByClass<URTSStateComponent>()->GetState();
=======
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
>>>>>>> componentrefactor
}

void AUnit::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);
   HideInvisibleUnits();
}

void AUnit::SetEnabled(bool bEnabled)
{
<<<<<<< HEAD
   if(bEnabled) {
      SetSelected(false);
=======
   if(bEnabled)
   {
      if(GetUnitSelected())
      {
         SetUnitSelected(false);
      }
>>>>>>> componentrefactor
      GetCapsuleComponent()->SetVisibility(true, true);
      SetActorEnableCollision(true);
      SetActorTickEnabled(true);
      GetCapsuleComponent()->SetEnableGravity(true);
      GetCharacterMovement()->GravityScale = 1;
<<<<<<< HEAD
      GetCapsuleComponent()->SetSimulatePhysics(false); // can't move w/o physics
      bCanAffectNavigationGeneration = true;
   } else {
      GetUnitController()->Stop();
=======
      // bCanAffectNavigationGeneration       = true;
      unitProperties.bIsEnabled = true;
      damageIndicatorWidget->SetActive(true);
   }
   else
   {
      GetUnitController()->StopCurrentAction();
>>>>>>> componentrefactor
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

void AUnit::OnUpdateGameSpeed(float speedMultiplier)
{
   GetCharacterMovement()->MaxWalkSpeed = statComponent->GetMechanicAdjValue(EMechanics::MovementSpeed) * speedMultiplier;
}

<<<<<<< HEAD
=======
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
      controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnUnitDeselectedDelegate.Broadcast();
   }
}

>>>>>>> componentrefactor
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
<<<<<<< HEAD

   FVector2D                                        screenLocation;
=======
   int sizeX, sizeY;
   controllerRef->GetViewportSize(sizeX, sizeY);
>>>>>>> componentrefactor
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
