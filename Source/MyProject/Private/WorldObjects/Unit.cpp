// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"

#include "UserInput.h"
#include "RTSGameState.h"

#include "Unit.h"
#include "Enemy.h"

#include "UpResourceManager.h"

#include "State/StateMachine.h"

#include "AIStuff/AIControllers/UnitController.h"

#include "UI/DamageIndicator/DIRender.h"
#include "UI/Healthbar/HealthbarComp.h"
#include "UI/HUDManager.h"
#include "UI/UserWidgets/ActionbarInterface.h"

#include "SpellSystem/MyAbilitySystemComponent.h"
#include "SpellSystem/MySpell.h"
#include "Stats/MyAttributeSet.h"
#include "SpellSystem/GameplayEffects/DamageEffect.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "MyCharacterMovementComponent.h"

#include "BrainComponent.h"

const FName AUnit::AIMessage_AttackReady = TEXT("AttackReady");
const FName AUnit::AIMessage_SpellCasted = TEXT("SpellCasted!");
const FName AUnit::AIMessage_SpellInterrupt = TEXT("SpellInterrupted!");
const FName AUnit::AIMessage_Stunned = TEXT("Stunned!");
const FName AUnit::AIMessage_Silenced = TEXT("Silenced!");
const FName AUnit::AIMessage_TargetLoss = TEXT("TargetLoss");

const float                   AUnit::diminishParam = 1.8f;
const TFunction<float(float)> AUnit::diminishFunc = [](float x) { return UpResourceManager::DiminishFunc(x, diminishParam); };

AUnit::AUnit(const FObjectInitializer& objectInitializer) : Super(objectInitializer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
   // Setup variables
   PrimaryActorTick.bCanEverTick = true;
   AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
   combatParams.combatStyle = ECombatType::Melee;

   //--Destroy arrow component so there isn't some random arrow sticking out of our units--
   auto components = GetComponents();
   for (auto& component : components) {
      if (auto arrowComponent = Cast<UArrowComponent>(component)) {
         arrowComponent->DestroyComponent();
         break;
      }
   }

   // Setup basic collision responses universal to all unit types
   GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel7, ECollisionResponse::ECR_Block);

   // Setup components can only happen in the constructor
   unitSpellData.abilitySystem = CreateDefaultSubobject<UMyAbilitySystemComponent>(TEXT("AbilitySystem"));
   selectionCircleDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("CircleShadowBounds"));
   selectionCircleDecal->SetupAttachment(RootComponent);

   healthBar = CreateDefaultSubobject<UHealthbarComp>(FName("Healthbar"));
   healthBar->SetRelativeLocation(FVector(0, 0, 180));
   healthBar->SetPivot(FVector2D(0.5,1));
   healthBar->SetupAttachment(RootComponent);
   healthBar->SetDrawAtDesiredSize(true);

   //--Find healthbar widget to set it as our default healthbar's widget class
   ConstructorHelpers::FClassFinder<UUserWidget> healthBarWig(TEXT("/Game/RTS_Tutorial/HUDs/Hitpoints/HealthbarWidget"));
   if (healthBarWig.Succeeded()) { healthBar->SetWidgetClass(healthBarWig.Class); }

   // Mesh needs an offset because it isn't aligned with capsule component at the beginning.  Offset by the mesh size
   GetMesh()->SetRelativeLocation(FVector(0, 0, -GetMesh()->Bounds.BoxExtent.Z));
   GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

   visionSphere = CreateDefaultSubobject<USphereComponent>(FName("VisionRadius"));
   visionSphere->SetupAttachment(RootComponent);
   visionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
   visionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
   visionSphere->bUseAttachParentBound = true;
   visionSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel5); // see this in defaultengine.ini

   // Allows units to step up stairs.  The height of the stairs they can step is set in some navmesh params
   GetCharacterMovement()->SetWalkableFloorAngle(90.f);
   GetCharacterMovement()->RotationRate = FRotator(0, 300.f, 0);
   GetCharacterMovement()->bOrientRotationToMovement = true;

   // Turn this off to make sure the unit highlights when hovered over
   GetMesh()->SetRenderCustomDepth(false);
}

void AUnit::BeginPlay()
{
   /// Setup initial parameters
   controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
   gameState = Cast<ARTSGameState>(GetWorld()->GetGameState());

   FVector origin, extent;
   GetActorBounds(true, origin, extent);
   unitProperties.height = FMath::Abs(origin.Z) + extent.Z - FMath::Abs(GetActorLocation().Z); // manually setup height informatoin for other things to read it

   if (selectionCircleDecal) {
      selectionCircleDecal->DecalSize = FVector(GetCapsuleComponent()->GetScaledCapsuleRadius());
      selectionCircleDecal->SetRelativeRotation(FRotator(90, 0, 0));
      selectionCircleDecal->SetRelativeLocation(FVector(0, 0, -90));
   }

   // Delegate Callback Setup
   if (unitController) {
      // Bind our event dispatcher with a function to change our state machine to idle when we are done moving
      unitController->ReceiveMoveCompleted.AddDynamic(this, &AUnit::OnMoveCompleted);
   }
   if (gameState) { gameState->UpdateGameSpeedDelegate.AddDynamic(this, &AUnit::OnUpdateGameSpeed); }

   // Setup abilitysystem attributes
   if (GetAbilitySystemComponent()) {
      // Make sure owner is player controller else the whole ability systme fails to function
      GetAbilitySystemComponent()->InitAbilityActorInfo(GetWorld()->GetFirstPlayerController(), this); // setup owner and avatar
      baseC = TUniquePtr<FBaseCharacter>(new FBaseCharacter(GetAbilitySystemComponent()->AddSet<UMyAttributeSet>()));
      GetCharacterMovement()->MaxWalkSpeed = GetMechanicAdjValue(static_cast<int>(Mechanics::MovementSpeed));

      for (TSubclassOf<UMySpell> ability : abilities) {
         if (ability.GetDefaultObject()) // if client tries to give himself ability assert fails
         {
            GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(ability.GetDefaultObject(), 1));
         }
      }
   }

   //Every unit should have the confirm abilities
   GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(USpellManager::Get().GetSpellClass(CONFIRM_SPELL_ID)));
   GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(USpellManager::Get().GetSpellClass(CONFIRM_SPELL_TARGET_ID)));

   // Setup vision parameters
   visionSphere->SetSphereRadius(unitProperties.visionRadius);
   visionSphere->SetRelativeLocation(FVector::ZeroVector);

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

#pragma region Accessors
EUnitState     AUnit::GetState() const
{
   return state->GetCurrentState();
}

void AUnit::UpdateStats()
{
   baseC->StatUpdate();
}

UAbilitySystemComponent* AUnit::GetAbilitySystemComponent() const
{
   return unitSpellData.abilitySystem;
}

void AUnit::Die_Implementation()
{
   // TODO: Spawn a corpse

   // Disable alive features
   SetEnabled(false);
   SetCanTarget(false);
   combatParams.isDead = true;

   //If this unit's info is being watched on the actionbar, remove it for now until I find a better fix
   if (controllerRef->GetBasePlayer()->focusedUnit == this)
      controllerRef->GetHUDManager()->GetActionHUD()->DeadUnitView();

   // Trigger "Death Events" in all skills that need them like Soul Catcher
   FGameplayEventData eD = FGameplayEventData();
   eD.EventTag = FGameplayTag::RequestGameplayTag("Event.Death");
   eD.TargetData = targetData.spellTargetData;
   if (GetAbilitySystemComponent()->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("Event.Death"), &eD)) {
      // GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Emerald, FString("Wee") + FString::FromInt(currentSpellIndex));
   }

   for (auto enemy : gameState->enemyList)
   {
      if (enemy->GetTarget() == this)
      {
         enemy->GetUnitController()->Stop();
         FAIMessage msg(AUnit::AIMessage_TargetLoss, enemy);
         FAIMessage::Send(enemy->GetUnitController(), msg);
      }
   }
   for (auto ally : gameState->allyList)
   {
      if (ally->GetTarget() == this)
      {
         ally->GetUnitController()->Stop();
         FAIMessage msg(AUnit::AIMessage_TargetLoss, ally);
         FAIMessage::Send(ally->GetUnitController(), msg);
      }
   }
}

void AUnit::SetEnabled(bool bEnabled)
{
   // Basically keep this unit loaded, but remove any trace of it
   if (bEnabled) {
      SetCanTarget(true);
      SetSelected(false);
      GetCapsuleComponent()->SetVisibility(true, true);
      SetActorEnableCollision(true);
      SetActorTickEnabled(true);
      GetCapsuleComponent()->SetEnableGravity(true);
      GetCharacterMovement()->GravityScale = 1;
      GetCapsuleComponent()->SetSimulatePhysics(false); // can't move w/o physics
      bCanAffectNavigationGeneration = true;
   }
   else {
      SetCanTarget(false);     
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

void AUnit::Attack_Implementation()
{
   // If we're not stunned and our attack rate is filled
   if (!IsStunned()) {
      // Create a gameplay effect for this
      FGameplayEffectContextHandle context = GetAbilitySystemComponent()->MakeEffectContext();
      context.AddInstigator(this, this);
      FGameplayEffectSpecHandle damageEffectHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(UDamageEffect::StaticClass(), 1, context);

      // Set all the effect's custom magnitude values else error is triggered
      UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Strength"), 0);
      UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Intelligence"), 0);
      UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Agility"), 0);
      UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Understanding"), 0);
      UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Health"), 0);

      // Stat used to determine damage depends on attack style
      switch (combatParams.combatStyle) {
      case ECombatType::Melee: UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Strength"), 100); break;
      case ECombatType::Magic: UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Intelligence"), 100);
      case ECombatType::Ranged: UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Agility"), 100); break;
      default: UE_LOG(LogTemp, Warning, TEXT("Error, combatstyle tag is not what it should be!")); return;
      }

      // TODO: Should add weapon element here
      UAbilitySystemBlueprintLibrary::AddAssetTag(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Element.Force"));
      GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*damageEffectHandle.Data.Get(), targetData.targetUnit->GetAbilitySystemComponent());
      if (attackAnimation)
         PlayAnimMontage(attackAnimation, 2 / GetSkillAdjValue(static_cast<int>(UnitStats::Attack_Speed)) + 100 * 0.01);
   }

   //Remove invisibility if you attack somebody
   GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Effect.Invisibility")));
}

UMySpell* AUnit::GetSpellCDO(TSubclassOf<UMySpell> spellClass) const
{
   // GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::FromInt(abilitySystem->GetActivatableAbilities().Num()));
   if (spellClass) {
      if (GetAbilitySystemComponent()->FindAbilitySpecFromClass(spellClass)->Ability) return spellClass->GetDefaultObject<UMySpell>();
   }
   return nullptr;
}

bool AUnit::CanCast(TSubclassOf<UMySpell> spellToCheck)
{
   UMySpell* spell = spellToCheck.GetDefaultObject();
   if (IsValid(spell)) {
      // Make sure we have enough mana
      if (spell->GetCost(GetAbilitySystemComponent()) <= GetVitalCurValue(static_cast<int>(Vitals::Mana))) // Enough Mana?
      {
         // Make sure the spell isn't on CD
         if (!spell->isOnCD(GetAbilitySystemComponent()) && !IsStunned() && !IsSilenced()) // Spell on CD and we aren't affected by any status that prevents us
         {
            return true;
         }
      }
   }
   return false;
}

bool AUnit::CastSpell(TSubclassOf<UMySpell> spellToCast)
{
   //Cast the spell and send the controller's brain a message
   if (GetAbilitySystemComponent()->TryActivateAbilityByClass(spellToCast)) {
      FAIMessage msg(AUnit::AIMessage_SpellCasted, this);
      FAIMessage::Send(unitController, msg);
      //If this spell has a channeling component after it is casted, change the state to channeling
      if(!spellToCast.Get()->GetDefaultObject<UMySpell>()->AbilityTags.HasTag(FGameplayTag::RequestGameplayTag("Skill.Channeled")))
         unitController->Stop();
      else {
         unitSpellData.channelTime = spellToCast.GetDefaultObject()->GetSecondaryTime(GetAbilitySystemComponent());
         state->ChangeState(EUnitState::STATE_CHANNELING);
      }
      return true;
   }
   return false;
}

void AUnit::OnMoveCompleted(FAIRequestID RequestID, const EPathFollowingResult::Type Result)
{
   //Check to see if we need to turn towards a point or a target after we're done moving.  
   //Moving naturally rotates us to our desired point/target, but it doesn't finish if we stop moving
   if (Result == EPathFollowingResult::Success)
   {
      switch (GetState()) {
      case EUnitState::STATE_MOVING:
         if (!unitController->IsFacingTarget(targetData.targetLocation)) unitController->TurnTowardsTarget(targetData.targetLocation);
         state->ChangeState(EUnitState::STATE_IDLE);
         break;
      case EUnitState::STATE_ATTACKING:
         !unitController->IsFacingTarget(targetData.targetUnit->GetActorLocation()) ? unitController->TurnTowardsActor(targetData.targetUnit) : unitProperties.turnAction.Execute();
         break;
      case EUnitState::STATE_CASTING:
         if (!targetData.targetActor)
            !unitController->IsFacingTarget(targetData.targetLocation) ? unitController->TurnTowardsTarget(targetData.targetLocation) : unitProperties.turnAction.Execute();
         else
            !unitController->IsFacingTarget(targetData.targetUnit->GetActorLocation()) ? unitController->TurnTowardsTarget(targetData.targetLocation) : unitProperties.turnAction.Execute();
         break;
      case EUnitState::STATE_INTERACTING:
         !unitController->IsFacingTarget(targetData.targetLocation) ? unitController->TurnTowardsTarget(targetData.targetLocation) : unitProperties.turnAction.Execute();
         break;
      case EUnitState::STATE_CHASING:
         if (unitController->ChasingQuit())
         {
            unitController->Stop();
            FAIMessage msg(AUnit::AIMessage_TargetLoss, this);
            FAIMessage::Send(unitController, msg);
         }      
         else
            unitController->Chase();
         break;
      case EUnitState::STATE_ATTACK_MOVE:
         unitController->Stop();
         break;
      default: break;
      }
   }
}

void AUnit::ShowDamageDealt(Damage& d)
{
   //Auto attach makes the new component the root for the sucessive components
   UDIRender* tRC = NewObject<UDIRender>(this);
   if (tRC) {
      tRC->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
      tRC->RegisterComponent();
      tRC->Text = FText::FromString(FString::FromInt(d.damage));
      if (d.crit)
         tRC->SetWorldSize(200.f);
      else
         tRC->SetWorldSize(100.f);
      tRC->SetTextRenderColor(UpResourceManager::elementalMap[d.element]);
      tRC->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
      tRC->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextBottom);
      tRC->SetRelativeLocation(FVector(0, 0, FindBoundary().Max.Y - FindBoundary().Min.Y + 100));
   }
}

void AUnit::ShowDamageDealt(FText occurance)
{
   //Auto attach makes the new component the root for the sucessive components
   UDIRender* tRC = NewObject<UDIRender>(this);
   if (tRC) {
      tRC->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
      tRC->RegisterComponent();
      tRC->Text = occurance;
      tRC->SetXScale(1.5f);
      tRC->SetYScale(1.5f);
      tRC->SetTextRenderColor(FColor::White);
      tRC->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
      tRC->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextBottom);
      tRC->SetRelativeLocation(FVector(0, 0, FindBoundary().Max.Y - FindBoundary().Min.Y + 100));
   }
}

float AUnit::GetDPS(float timespan)
{
   return combatParams.GetDPS(timespan, GetWorld()->GetTimeSeconds());
}

float AUnit::GetDamageRecievedPerSecond(float timespan)
{
   return combatParams.GetDamageRecievedPerSecond(timespan, GetWorld()->GetTimeSeconds());
}

float AUnit::GetHealingPerSecond(float timespan)
{
   return combatParams.GetHealingPerSecond(timespan, GetWorld()->GetTimeSeconds());
}

float AUnit::GetHealingRecievedPerSecond(float timespan)
{
   return combatParams.GetHealingRecievedPerSecond(timespan, GetWorld()->GetTimeSeconds());
}

float AUnit::CalculateRisk()
{
   //Calculate score based on how much health is missing
   const float currentHealth = GetVitalCurValue(static_cast<uint8>(Vitals::Health));
   const float totalHealth = GetVitalAdjValue(static_cast<uint8>(Vitals::Health));
   const float healthPer = (totalHealth - currentHealth) / totalHealth;

   //Calculate score based on how many debuffs we have
   FGameplayEffectQuery debuffTagQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Effect.Debuff")));
   int                  debuffNum = GetAbilitySystemComponent()->GetActiveEffects(debuffTagQuery).Num();
   const float          debuffRiskValue = FMath::Clamp(diminishFunc(debuffNum), 0.f, 1.f);

   //Calculate score based on how many units are targetting us
   const float targetRiskValue = CalculateTargetRisk();

   //Calculate score based on how much damage we are taking per second
   const float damageIntake = GetDamageRecievedPerSecond(1), healingIntake = GetHealingRecievedPerSecond(1);
   const float netIntake = (damageIntake - healingIntake) / currentHealth;

   GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Silver, FString::Printf(TEXT("Target, Debuff, NetIntake: %f, %f, %f"), targetRiskValue, debuffRiskValue, netIntake));
   return healthPer + targetRiskValue + debuffRiskValue + netIntake;
}

float AUnit::CalculateThreat()
{
   const float damageDealt = GetDPS(1), healingDealt = GetHealingPerSecond(1);
   // Calculate score based on how much damage we're dealing
   // Should be done differently for enemies and allies float avgDPS = 0, avgHealing = 0;
   for (AAlly* ally : controllerRef->GetBasePlayer()->allies) {
      ally->GetDPS(1);
   }

   //Calculate score based on how many buffs we have
   FGameplayEffectQuery buffTagQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Effect.Buff")));
   int                  buffNum = GetAbilitySystemComponent()->GetActiveEffects(buffTagQuery).Num();
   const float          buffThreatValue = FMath::Clamp(diminishFunc(buffNum), 0.f, 1.f);

   //Calculate score based on how many castable spells we have left (we might not have the mana to cast them though...)
   int numAvailableSpells = 0;
   for (TSubclassOf<UMySpell> ability : abilities) {
      if (CanCast(ability)) ++numAvailableSpells;
   }
   const float availableSpellsThreatValue = FMath::Clamp(diminishFunc(numAvailableSpells), 0.f, 1.f);

   return buffThreatValue + availableSpellsThreatValue;
}

void AUnit::OnUpdateGameSpeed(float speedMultiplier)
{
   GetCharacterMovement()->MaxWalkSpeed = GetMechanicAdjValue(static_cast<int>(Mechanics::MovementSpeed)) * speedMultiplier;
}

FBox2D AUnit::FindBoundary()
{
   // This function can have errors due to being called during level transition before old level is completely unloaded
   FBox2D  boundary = FBox2D(ForceInit);
   FVector origin, extent;
   GetActorBounds(true, origin, extent);
   FVector2D         screenLocation;
   TArray<FVector2D> corners; // get 8 corners of box

   for (int i = 0; i < 8; ++i) {
      controllerRef->ProjectWorldLocationToScreen(origin + extent * UpResourceManager::BoundsPointMapping[i], screenLocation, true);
      corners.Add(screenLocation);
      // GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, corners[i].ToString() + " " + boundary.GetExtent().ToString());
      boundary += corners[i];
   }
   return boundary;
}

bool AUnit::IsStunned() const
{
   return GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Debuff.Stunned")) ? true : false;
}

bool AUnit::IsSilenced() const
{
   return GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Debuff.Silenced")) ? true : false;
}

void AUnit::ApplyBonuses(int category, int value)
{
   if (category < CombatInfo::AttCount) {
      baseC->GetAttribute(category)->SetCurrentValue(GetAttributeAdjValue(category) + value);
   }
   else if (category >= CombatInfo::AttCount && category < CombatInfo::AttCount + CombatInfo::StatCount) {
      int index = category - CombatInfo::AttCount;
      baseC->GetSkill(index)->SetBuffValue(GetSkillAdjValue(index) + value);
   }
   else if (category >= CombatInfo::AttCount + CombatInfo::StatCount && category < CombatInfo::AttCount + CombatInfo::StatCount + CombatInfo::VitalCount) {
      int index = category - CombatInfo::AttCount - CombatInfo::StatCount;
      baseC->GetVital(index)->SetBuffValue(GetVitalAdjValue(index) + value);
   }
   else {
      int index = category - CombatInfo::AttCount - CombatInfo::StatCount - CombatInfo::VitalCount;
      baseC->GetMechanic(index)->SetCurrentValue(GetMechanicAdjValue(index) + value);
   }
}
