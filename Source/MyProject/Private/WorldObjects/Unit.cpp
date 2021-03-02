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

#include "MyCharacterMovementComponent.h"
#include "PartyDelegateContext.h"

#include "RTSStateComponent.h"
#include "RTSVisionComponent.h"

#include "UpStatComponent.h"
#include "TargetComponent.h"

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

   selectionCircleDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("CircleShadowBounds"));
   selectionCircleDecal->SetupAttachment(RootComponent);

   SetupHealthbarComponent();

   SetupCharacterCollision();

   SetupMovementComponent();

   SetupDamageInidicatorContainerWidget();

   combatInfo              = MakeUnique<UpCombatInfo>();
   combatInfo->combatStyle = ECombatType::Melee;

   // Turn this off to make sure the unit highlights when hovered over
   GetMesh()->SetRenderCustomDepth(false);
}

AUnit::~AUnit() = default;

void AUnit::SetupHealthbarComponent()
{
   healthBar = CreateDefaultSubobject<UHealthbarComp>("Healthbar");
   healthBar->SetRelativeLocation(FVector(0, 0, 180));
   healthBar->SetPivot(FVector2D(0.5, 1));
   healthBar->SetupAttachment(RootComponent);
   healthBar->SetDrawAtDesiredSize(true);

   ConstructorHelpers::FClassFinder<UUserWidget> healthBarWig(TEXT("/Game/RTS_Tutorial/HUDs/ActionUI/Hitpoints/HealthbarWidget"));
   if(healthBarWig.Succeeded())
   {
      healthBar->SetWidgetClass(healthBarWig.Class);
   }
}

void AUnit::SetupCharacterCollision() const
{
   // Mesh needs an offset because it isn't aligned with capsule component at the beginning.  Offset by the mesh size
   GetMesh()->SetRelativeLocation(FVector(0, 0, -GetMesh()->Bounds.BoxExtent.Z));
   GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
   GetCapsuleComponent()->SetCollisionResponseToChannel(SELECTABLE_BY_CLICK_CHANNEL, ECR_Block);
}

void AUnit::SetupMovementComponent() const
{
   // Allows units to step up stairs.  The height of the stairs they can step is set in some navmesh params
   GetCharacterMovement()->SetWalkableFloorAngle(90.f);
   GetCharacterMovement()->RotationRate              = FRotator(0, 300.f, 0);
   GetCharacterMovement()->bOrientRotationToMovement = true;
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
}

void AUnit::RemoveArrowComponent() const
{
   // Destroy arrow component so there isn't some random arrow sticking out of our units
   if(UArrowComponent* arrowComp = FindComponentByClass<UArrowComponent>())
   {
      arrowComp->DestroyComponent();
   }
}

void AUnit::SetupAbilitiesAndStats()
{
   if(GetAbilitySystemComponent())
   {
      // ! Make sure owner is player controller else the whole ability system fails to function (maybe it should be set to RTSPawn I'll have to double check)
      // This sets up the owner and avatar actors for our ability component.
      GetAbilitySystemComponent()->InitAbilityActorInfo(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController(), this);
      GetCharacterMovement()->MaxWalkSpeed = statComponent->GetMechanicAdjValue(EMechanics::MovementSpeed);

      for(TSubclassOf<UMySpell> ability : GetAbilitySystemComponent()->GetAbilities())
      {
         if(ability.GetDefaultObject())
         {
            // If a client tries to give himself ability assert fails
            GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(ability.GetDefaultObject(), 1));
         }
      }
   }
}

void AUnit::SetupSelectionCircle() const
{
   if(selectionCircleDecal)
   {
      selectionCircleDecal->DecalSize = FVector(GetCapsuleComponent()->GetScaledCapsuleRadius());
      selectionCircleDecal->SetUsingAbsoluteScale(true);
      selectionCircleDecal->SetWorldScale3D(FVector::OneVector);
      selectionCircleDecal->SetRelativeRotation(FRotator(90, 0, 0));
      selectionCircleDecal->SetRelativeLocation(FVector(0, 0, -90));
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

   if(UNLIKELY(!IsValid(controllerRef)))
   {
      controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
   }

   GetWorld()->GetTimerManager().SetTimerForNextTick([this]() {
      damageIndicatorWidget->SetWidgetClass(controllerRef->GetHUDManager()->damageIndicatorContainerClass);
      Cast<URTSDamageNumberContainer>(damageIndicatorWidget->GetUserWidgetObject())->SetOwningUnit(this);
   });

   StoreUnitHeight();
   SetupSelectionCircle();

   if(const ARTSGameState* gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState()))
   {
      gameStateRef->OnGameSpeedUpdated().AddDynamic(this, &AUnit::OnUpdateGameSpeed);
   }

   SetupAbilitiesAndStats();

   visionComponent->SetRelativeLocation(FVector::ZeroVector);
   damageIndicatorWidget->SetRelativeLocation(FVector::ZeroVector);
}

void AUnit::PossessedBy(AController* newController)
{
   Super::PossessedBy(newController);
   unitController = Cast<AUnitController>(newController);
}

void AUnit::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);
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

void AUnit::SetEnabled(bool bEnabled)
{
   if(bEnabled)
   {
      SetUnitSelected(false);
      GetCapsuleComponent()->SetVisibility(true, true);
      SetActorEnableCollision(true);
      SetActorTickEnabled(true);
      GetCapsuleComponent()->SetEnableGravity(true);
      GetCharacterMovement()->GravityScale = 1;
      GetCapsuleComponent()->SetSimulatePhysics(false); // can't move w/o physics
      bCanAffectNavigationGeneration = true;
      unitProperties.bIsEnabled      = true;
      damageIndicatorWidget->RegisterComponent();
   }
   else
   {
      GetUnitController()->StopCurrentAction();
      GetCapsuleComponent()->SetVisibility(false, true);
      SetActorEnableCollision(false);
      SetActorTickEnabled(false);
      GetCapsuleComponent()->SetEnableGravity(false);
      GetCharacterMovement()->GravityScale = 0;
      GetCapsuleComponent()->SetSimulatePhysics(true); // but will drop if physics isn't set to true
      GetCapsuleComponent()->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
      bCanAffectNavigationGeneration = false;
      unitProperties.bIsEnabled      = false;
      damageIndicatorWidget->UnregisterComponent();
   }
}

void AUnit::OnUpdateGameSpeed(float speedMultiplier)
{
   GetCharacterMovement()->MaxWalkSpeed = statComponent->GetMechanicAdjValue(EMechanics::MovementSpeed) * speedMultiplier;
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
      controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnUnitDeselectedDelegate.Broadcast();
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

   FVector2D                                        screenLocation;
   static const int                                 CORNER_COUNT = 8;
   TArray<FVector2D, TFixedAllocator<CORNER_COUNT>> corners; // Get 8 corners of box

   for(int i = 0; i < CORNER_COUNT; ++i)
   {
      controllerRef->ProjectWorldLocationToScreen(origin + extent * UpResourceManager::BoundsPointMapping[i], screenLocation, true);
      corners.Add(screenLocation);
      boundary += corners[i];
   }
   return boundary;
}
