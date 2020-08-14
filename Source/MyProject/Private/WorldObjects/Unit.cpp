// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"

#include "UserInput.h"
#include "RTSGameState.h"
#include "RTSPawn.h"

#include "Unit.h"
#include "Enemy.h"

#include "UpResourceManager.h"

#include "State/RTSStateMachine.h"

#include "AIStuff/AIControllers/UnitController.h"

#include "NullAttackAnim.h"

#include "UI/DamageIndicator/DIRender.h"
#include "UI/Healthbar/HealthbarComp.h"
#include "UI/HUDManager.h"
#include "UI/UserWidgets/ActionbarInterface.h"

#include "SpellSystem/RTSAbilitySystemComponent.h"
#include "SpellSystem/MySpell.h"
#include "Stats/MyAttributeSet.h"
#include "SpellSystem/GameplayEffects/RTSDamageEffect.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BasePlayer.h"

#include "MyCharacterMovementComponent.h"

#include "BrainComponent.h"
#include "CombatParameters.h"

#include "TargetComponent.h"
#include "UpStatComponent.h"

const float                   AUnit::diminishParam = 1.8f;
const TFunction<float(float)> AUnit::diminishFunc  = [](float x) { return UpResourceManager::DiminishFunc(x, diminishParam); };

void AUnit::SetupHealthbarComponent()
{
   healthBar = CreateDefaultSubobject<UHealthbarComp>("Healthbar");
   healthBar->SetRelativeLocation(FVector(0, 0, 180));
   healthBar->SetPivot(FVector2D(0.5, 1));
   healthBar->SetupAttachment(RootComponent);
   healthBar->SetDrawAtDesiredSize(true);

   ConstructorHelpers::FClassFinder<UUserWidget> healthBarWig(TEXT("/Game/RTS_Tutorial/HUDs/ActionUI/Hitpoints/HealthbarWidget"));
   if(healthBarWig.Succeeded()) {
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

void AUnit::RemoveArrowComponent() const
{
   // Destroy arrow component so there isn't some random arrow sticking out of our units
   GetComponentByClass(UArrowComponent::StaticClass())->DestroyComponent();
}

AUnit::AUnit(const FObjectInitializer& objectInitializer) :
    Super(objectInitializer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
   PrimaryActorTick.bCanEverTick = true;
   AutoPossessAI                 = EAutoPossessAI::PlacedInWorldOrSpawned;
   combatParams                  = MakeUnique<UPAICombatParameters>();
   combatParams->combatStyle     = ECombatType::Melee;

   RemoveArrowComponent();

   abilitySystemComponent = CreateDefaultSubobject<URTSAbilitySystemComponent>(TEXT("AbilitySystem"));

   visionComponent = CreateDefaultSubobject<URTSVisionComponent>(FName("VisionRadius"));
   visionComponent->SetupAttachment(RootComponent);

   selectionCircleDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("CircleShadowBounds"));
   selectionCircleDecal->SetupAttachment(RootComponent);

   attackAnimClass = UNullAttackAnim::StaticClass();

   SetupHealthbarComponent();

   SetupCharacterCollision();

   SetupMovementComponent();

   // Turn this off to make sure the unit highlights when hovered over
   GetMesh()->SetRenderCustomDepth(false);
}

void AUnit::SetupAbilitiesAndStats()
{
   if(GetAbilitySystemComponent()) {
      // Make sure owner is player controller else the whole ability system fails to function (maybe it should be set to RTSPawn I'll have to double check)
      GetAbilitySystemComponent()->InitAbilityActorInfo(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController(), this); // setup owner and avatar
      GetCharacterMovement()->MaxWalkSpeed = statComponent->GetMechanicAdjValue(EMechanics::MovementSpeed);

      for(TSubclassOf<UMySpell> ability : abilitySystemComponent->abilities) {
         if(ability.GetDefaultObject()) // if client tries to give himself ability assert fails
         {
            GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(ability.GetDefaultObject(), 1));
         }
      }

      // Every unit should have the confirm abilities
      GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(USpellManager::Get().GetSpellClass(CONFIRM_SPELL_ID)));
      GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(USpellManager::Get().GetSpellClass(CONFIRM_SPELL_TARGET_ID)));
   }
}

void AUnit::AlignSelectionCircleWithGround() const
{
   if(selectionCircleDecal) {
      selectionCircleDecal->DecalSize = FVector(GetCapsuleComponent()->GetScaledCapsuleRadius());
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
   if(UNLIKELY(!IsValid(controllerRef)))
      controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());

   auto gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState());

   StoreUnitHeight();

   AlignSelectionCircleWithGround();

   gameStateRef->UpdateGameSpeedDelegate.AddDynamic(this, &AUnit::OnUpdateGameSpeed);

   SetupAbilitiesAndStats();

   visionComponent->SetRelativeLocation(FVector::ZeroVector);

   attackAnim = TUniquePtr<IAttackAnim>(NewObject<IAttackAnim>(this, attackAnimClass));

   Super::BeginPlay();
}

void AUnit::PossessedBy(AController* newController)
{
   Super::PossessedBy(newController);
   unitController = Cast<AUnitController>(newController);
}

void AUnit::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);
   state->Update(deltaSeconds);
}

EUnitState AUnit::GetState() const
{
   return state->GetCurrentState();
}

void AUnit::SetEnabled(bool bEnabled)
{
   if(bEnabled) {
      SetSelected(false);
      GetCapsuleComponent()->SetVisibility(true, true);
      SetActorEnableCollision(true);
      SetActorTickEnabled(true);
      GetCapsuleComponent()->SetEnableGravity(true);
      GetCharacterMovement()->GravityScale = 1;
      GetCapsuleComponent()->SetSimulatePhysics(false); // can't move w/o physics
      bCanAffectNavigationGeneration = true;
   } else {
      GetCapsuleComponent()->SetVisibility(false, true);
      SetActorEnableCollision(false);
      SetActorTickEnabled(false);
      GetCapsuleComponent()->SetEnableGravity(false);
      GetCharacterMovement()->GravityScale = 0;
      GetCapsuleComponent()->SetSimulatePhysics(true); // but will drop if physics isn't set to true
      GetCapsuleComponent()->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
      bCanAffectNavigationGeneration = false;
   }
}

void AUnit::OnUpdateGameSpeed(float speedMultiplier)
{
   GetCharacterMovement()->MaxWalkSpeed = statComponent->GetMechanicAdjValue(EMechanics::MovementSpeed) * speedMultiplier;
}

bool AUnit::GetIsEnemy() const
{
   return combatParams->isEnemy;
}

bool AUnit::GetIsDead() const
{
   return combatParams->isDead;
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

   for(int i = 0; i < CORNER_COUNT; ++i) {
      controllerRef->ProjectWorldLocationToScreen(origin + extent * UpResourceManager::BoundsPointMapping[i], screenLocation, true);
      corners.Add(screenLocation);
      boundary += corners[i];
   }
   return boundary;
}
