// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UserInput.h"
#include "RTSGameState.h"
#include "Unit.h"
#include "ResourceManager.h"
#include "State/StateMachine.h"
#include "AIController.h"
#include "UI/DamageIndicator/DIRender.h"
#include "UI/Healthbar/HealthbarComp.h"
#include "SpellSystem/MyAbilitySystemComponent.h"
#include "MyCharacterMovementComponent.h"
#include "SpellSystem/MySpell.h"
#include "Stats/MyAttributeSet.h"
#include "SpellSystem/GameplayEffects/DamageEffect.h"
#include "AbilitySystemBlueprintLibrary.h"

AUnit::AUnit(const FObjectInitializer& objectInitializer) : Super(objectInitializer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	//Setup variables
	PrimaryActorTick.bCanEverTick = true;
	combatParams.combatStyle = ECombatType::Melee;

	//--Destroy arrow component so there isn't some random arrow sticking out of our units--  
	auto components = GetComponents();
	for (auto& component : components)
	{
		if (auto arrowComponent = Cast<UArrowComponent>(component))
		{
			arrowComponent->DestroyComponent();
			break;
		}
	}

	//Setup components can only happen in the constructor
	unitSpellData.abilitySystem = CreateDefaultSubobject<UMyAbilitySystemComponent>(TEXT("AbilitySystem"));
	selectionCircleDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("CircleShadowBounds"));
	selectionCircleDecal->SetupAttachment(RootComponent);
	healthBar = CreateDefaultSubobject<UHealthbarComp>(FName("Healthbar"));
	healthBar->SetupAttachment(RootComponent);

	//Mesh needs an offset because it isn't aligned with capsule component at the beginning
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	//--Find healthbar widget to set it as our default healthbar's widget class 
	ConstructorHelpers::FClassFinder<UUserWidget> healthBarWig(TEXT("/Game/RTS_Tutorial/HUDs/Hitpoints/HealthbarWidget"));
	if (healthBarWig.Succeeded())
	{
		healthBar->SetWidgetClass(healthBar->StaticClass());
	}

	visionSphere = CreateDefaultSubobject<USphereComponent>(FName("VisionRadius"));
	visionSphere->SetupAttachment(RootComponent);
	visionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	visionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	visionSphere->bUseAttachParentBound = true;
	visionSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel5); //see this in defaultengine.ini
}

void AUnit::BeginPlay()
{
	Super::BeginPlay();

	///---Setup initial parameters---
	controller = Cast<AAIController>(GetController());
	controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
	gameState = Cast<ARTSGameState>(GetWorld()->GetGameState());

	FVector origin, extent;
	GetActorBounds(true, origin, extent);
	height = FMath::Abs(origin.Z) + extent.Z - FMath::Abs(GetActorLocation().Z); //manually setup height informatoin for other things to read it

	if (selectionCircleDecal)
	{
		selectionCircleDecal->DecalSize = FVector(GetCapsuleComponent()->GetScaledCapsuleRadius());
		selectionCircleDecal->SetRelativeRotation(FRotator(90, 0, 0));
		selectionCircleDecal->SetRelativeLocation(FVector(0, 0, -90));
	}

	///---Delegate Callback Setup---
	if (controller)
	{
		//Bind our event dispatcher with a function to change our state machine to idle when we are done moving
		controller->ReceiveMoveCompleted.AddDynamic(this, &AUnit::OnMoveCompleted);
	}

	//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Yellow, AActor::GetName() + " " + origin.ToString() + " " + extent.ToString());
	if (gameState)
	{
		gameState->UpdateGameSpeedDelegate.AddDynamic(this, &AUnit::OnUpdateGameSpeed);
	}

	//Setup abilitysystem attributes
	if (GetAbilitySystemComponent())
	{
		//Make sure owner is player controller else this won't work
		GetAbilitySystemComponent()->InitAbilityActorInfo(GetWorld()->GetFirstPlayerController(), this); //setup owner and avatar
		baseC = TUniquePtr<FBaseCharacter>(new FBaseCharacter(GetAbilitySystemComponent()->AddSet<UMyAttributeSet>()));
		GetCharacterMovement()->MaxWalkSpeed = GetMechanicAdjValue(static_cast<int>(Mechanics::MovementSpeed));

		for (TSubclassOf<UMySpell> ability : abilities)
		{
			if (ability.GetDefaultObject()) //if client tries to give himself ability assert fails
			{
				GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(ability.GetDefaultObject(), 1));
			}
		}
		//abilitySystem->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &AUnit::RemoveGameplayEffects);
	}

	visionSphere->SetSphereRadius(visionRadius);
	visionSphere->SetRelativeLocation(FVector::ZeroVector);
}

void AUnit::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	//Calculate when we can attack again 
	if (!combatParams.readyToAttack && combatParams.currentAttTime < 2 / ((GetSkillAdjValue(static_cast<int>(UnitStats::Attack_Speed)) + 100) * 0.01))
	{
		combatParams.currentAttTime += deltaSeconds * gameState->speedModifier;
	}
	else
	{
		if (combatParams.readyToAttack == false)
		{
			combatParams.currentAttTime = 0;
			combatParams.readyToAttack = true;
		}
	}

	state->Update(deltaSeconds);
}

#pragma region Accessors
EUnitState AUnit::GetState() const
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

#pragma endregion

#pragma region Helpers
bool AUnit::IsTargetInRange(float range, FVector targetLoc)
{
	FVector currentLocation = GetActorLocation();
	FVector difference = currentLocation - targetLoc;

	if (FVector::DotProduct(difference, difference) <= range * range)
		return true;
	return false;
}

bool AUnit::IsFacingTarget(FVector targetLoc)
{
	//Lets ensure the vector between our location and the target location is close to the same direction we're facing
	FVector difference = targetLoc - GetActorLocation();
	float dot = FVector::DotProduct(GetActorForwardVector(), FVector(difference.X, difference.Y, 0).GetSafeNormal());
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("%f"), dot));
	/*
	int cross =  FVector::CrossProduct(currentLocation, targetData.targetLocation);
	float angle = FMath::Atan2(cross, dot);
	*/
	if (dot > .95) //18 degrees lenient (only from right side).  
		return true;
	return false;
}

void AUnit::TurnTowardsTarget(FVector targetLoc)
{
	FVector currentLocation = GetActorLocation();
	FVector difference = targetLoc - currentLocation;
	FVector projectedDirection = FVector(difference.X, difference.Y, 0);
	SetActorRotation(FRotationMatrix::MakeFromX(FVector(projectedDirection)).Rotator());
}

bool AUnit::AdjustPosition(float range, FVector targetLoc)
{
	if (!IsTargetInRange(range, targetLoc))
	{
		//GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Orange, TEXT("MOVING TO TARGET!!!"));	
		controller->MoveToLocation(targetLoc, UPathFollowingComponent::DefaultAcceptanceRadius);
		return false;
	}
	else
	{
		controller->StopMovement();
		if (!IsFacingTarget(targetLoc))
		{
			//GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Orange, TEXT("TURNING TOWARDS TARGET!!!"));
			TurnTowardsTarget(targetLoc);
			return false;
		}
	}
	return true;
}
#pragma endregion

void AUnit::Move(FVector newLocation)
{
	if (!IsStunned())
	{
		if (GetController())
		{
			//cancel any targets we may have since our focus with this command is pure movement
			Stop();
			//shift location a little bit if we're moving multiple units so they can group together ok
			FVector shiftedLocation = newLocation - GetActorLocation().GetSafeNormal() * GetCapsuleComponent()->GetScaledCapsuleRadius() / 2;
			controller->MoveToLocation(shiftedLocation, 10, true, true, false, true);
			state->ChangeState(EUnitState::STATE_MOVING);
		}
	}
}

void AUnit::Die()
{
	//Spawn a corpse

	//Disable alive features 
	SetEnabled(false);

	SetCanTarget(false);
	combatParams.isDead = true;

	//Trigger "Death Events"
	FGameplayEventData eD = FGameplayEventData();
	eD.EventTag = FGameplayTag::RequestGameplayTag("Event.Death");
	eD.TargetData = targetData.spellTargetData;
	if (GetAbilitySystemComponent()->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("Event.Death"), &eD))
	{
		//GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Emerald, FString("Wee") + FString::FromInt(currentSpellIndex));
	}
	//Destroy?
}

void AUnit::SetEnabled(bool bEnabled)
{
	if (bEnabled)
	{
		SetCanTarget(true);
		GetCapsuleComponent()->SetVisibility(true, true);
		SetActorEnableCollision(true);
		SetActorTickEnabled(true);
		GetCapsuleComponent()->SetEnableGravity(true);
		GetCapsuleComponent()->SetSimulatePhysics(false); //can't move w/o physics
	}
	else
	{
		SetCanTarget(false);
		GetCapsuleComponent()->SetVisibility(false, true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
		GetCapsuleComponent()->SetEnableGravity(false);
		GetCapsuleComponent()->SetSimulatePhysics(true); //but will drop if physics isn't set to true
	}
}

void AUnit::CommitCast(UMySpell* spell)
{
	int spellResIndex = static_cast<int>(Vitals::Mana); //index of resource to be used to cast spell
	if (spell && spell->GetCost(GetAbilitySystemComponent()) <= GetVitalCurValue(spellResIndex)) //if we have enough mana (in the future different spells may have their own resources)
	{
#if UE_EDITOR
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString("Cost: ") + FString::FromInt(spell->GetCost(abilitySystem)));
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString("Current Mana: ") + FString::FromInt(GetVitalCurValue(spellResIndex)));
#endif
		baseC->GetVital(spellResIndex)->SetCurrValue(GetVitalCurValue(spellResIndex) - spell->GetCost(GetAbilitySystemComponent()));
	}
}

void AUnit::BeginAttack(AUnit * target)
{
	Stop(); //Stop any other actions we're doing
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString("Attack!"));
	state->ChangeState(EUnitState::STATE_ATTACKING);
	targetData.targetUnit = target;
	targetData.spellTargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(target);
}

void AUnit::PrepareAttack()
{
	if (!IsStunned() && targetData.targetUnit)
	{
		if (targetData.targetUnit->GetCanTarget())
		{
			FVector targetLoc = FVector(targetData.targetUnit->GetActorLocation().X, targetData.targetUnit->GetActorLocation().Y, 0);
			//if (unitTarget->GetIsEnemy()) //may remove this check because we may be able to hit allies
			//{
			if (AdjustPosition(GetMechanicAdjValue(static_cast<int>(Mechanics::AttackRange)), targetLoc))
			{
				Attack();
			}
		}
		else
		{
			targetData.targetUnit = nullptr;
		}
	}
}

void AUnit::Attack()
{
	//Attack Time = 60 / WeaponAttkSpd / AttkSpeed
	if (!IsStunned() && combatParams.readyToAttack) //If we're not stunned and our attack rate is filled
	{
		//play attack animation
		//Create a gameplay effect for this
		FGameplayEffectContextHandle context = GetAbilitySystemComponent()->MakeEffectContext();
		context.AddInstigator(this, this);
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("NEW SPEC MADE DAMAGE"));
		FGameplayEffectSpecHandle damageEffectHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(UDamageEffect::StaticClass(), 1, context);
		//set all the effect's custom magnitude values else it complains
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Strength"), 0);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Intelligence"), 0);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Agility"), 0);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Understanding"), 0);

		switch (combatParams.combatStyle)
		{
		case ECombatType::Melee:
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Strength"), 100);
			break;
		case ECombatType::Magic:
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Intelligence"), 100);
		case ECombatType::Ranged:
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Agility"), 100);
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Error, combatstyle tag is not what it should be!"));
			return;
		}
		//Should add weapon element here
		UAbilitySystemBlueprintLibrary::AddAssetTag(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Element.Force"));
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*damageEffectHandle.Data.Get(), targetData.targetUnit->GetAbilitySystemComponent());
		combatParams.readyToAttack = false;
	}
}

UMySpell* AUnit::GetSpellCDO(TSubclassOf<UMySpell> spellClass) const
{
#if UE_EDITOR
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::FromInt(abilitySystem->GetActivatableAbilities().Num()));
#endif
	if (spellClass)
	{
		//If we have the ability
		if (GetAbilitySystemComponent()->FindAbilitySpecFromClass(spellClass)->Ability)
			return spellClass->GetDefaultObject<UMySpell>();
	}
	return nullptr;
}

bool AUnit::CanCast(TSubclassOf<UMySpell> spellToCheck)
{
	UMySpell* spell = spellToCheck.GetDefaultObject();
	if (IsValid(spell))
	{
		if (spell->GetCost(GetAbilitySystemComponent()) <= GetVitalCurValue(static_cast<int>(Vitals::Mana))) //Enough Mana?
		{
			if (!spell->isOnCD(GetAbilitySystemComponent()) && !IsStunned() && !IsSilenced()) //Spell on CD and we aren't affected by any status that prevents us
			{
				return true;
			}
		}
	}
	return false;
}

bool AUnit::BeginCastSpell(int spellToCastIndex, FGameplayAbilityTargetDataHandle tData)
{
	UMySpell* spell = abilities[spellToCastIndex].GetDefaultObject();

	if (IsValid(spell)) //valid spell class check
	{
		if (CanCast(abilities[spellToCastIndex]))
		{
			SetCurrentSpell(abilities[spellToCastIndex]);
			
			if (spell->GetTargetting().GetTagName() == "Skill.Targetting.None") //non targetted?  Then just cast it
			{
				state->ChangeState(EUnitState::STATE_CASTING);
				PreCastChannelingCheck(GetCurrentSpell());
				return true;
			}
			else
			{
				if (tData.Num() > 0)
				{
					targetData.spellTargetData = tData;

					if (spell->GetTargetting().MatchesTag(FGameplayTag::RequestGameplayTag("Skill.Targetting.Area")))
					{
						targetData.targetLocation = UAbilitySystemBlueprintLibrary::GetTargetDataEndPoint(targetData.spellTargetData, 0);
					}
					else
					{
						targetData.targetActor = UAbilitySystemBlueprintLibrary::GetActorsFromTargetData(targetData.spellTargetData, 0)[0];
					}

					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("%f"), FVector::Dist2D(targetData.targetLocation, GetActorLocation())));
					if (targetData.targetActor == this || FVector::Dist2D(targetData.targetLocation, GetActorLocation()) < 5.f)
					{
						PreCastChannelingCheck(GetCurrentSpell());
						return true;
					}

					state->ChangeState(EUnitState::STATE_CASTING);
					return true;
				}
			}
		}
	}
	//if spell cannot be cast or is invalid
	Stop();
	return false;
}


void AUnit::PrepareCastSpell()
{
	if (CanCast(GetCurrentSpell()))
	{
		UMySpell* spell = GetCurrentSpell().GetDefaultObject();

		FVector targetLoc;
		if (spell->GetTargetting().MatchesTag(FGameplayTag::RequestGameplayTag("Skill.Targetting.Area")))
			targetLoc = targetData.targetLocation;
		else
		{
			targetLoc = targetData.targetActor->GetActorLocation();
		}
		if (AdjustPosition(spell->GetRange(GetAbilitySystemComponent()), targetLoc))
		{
			PreCastChannelingCheck(GetCurrentSpell());
		}
	}
}

bool AUnit::CastSpell(TSubclassOf<UMySpell> spellToCast)
{
	if (GetAbilitySystemComponent()->TryActivateAbilityByClass(spellToCast))
	{
		if (spellToCast.GetDefaultObject()->GetTargetting().GetTagName() != "Skill.Targetting.None")
		{
			//When spell is actually casted, trigger castclick to let the ability know it should be ready to move on...
			//May be obsolete since we don't actully activate the spell until we click anyways.  However, it does pass in target data
			FGameplayEventData eD = FGameplayEventData();
			eD.EventTag = UGameplayTagsManager::Get().RequestGameplayTag("Event.CastClick");
			eD.TargetData = targetData.spellTargetData;
			if (GetAbilitySystemComponent()->HandleGameplayEvent(UGameplayTagsManager::Get().RequestGameplayTag("Event.CastClick"), &eD))
			{
				//GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Emerald, FString("Wee") + FString::FromInt(currentSpellIndex));
			}
		}
		Stop();
		return true;
	}
	return false;
}

void AUnit::PreCastChannelingCheck(TSubclassOf<UMySpell> spellToCast)
{
	//if there isn't a cast time
	if (!GetCurrentSpell().GetDefaultObject()->GetCastTime(GetAbilitySystemComponent()))
	{
		CastSpell(GetCurrentSpell());
	}
	else
	{
		state->ChangeState(EUnitState::STATE_CHANNELING); 	//start channeling
	}
}

void AUnit::Stop()
{
	//Stop will not only stop unit in its track, but will also make it forget its target, and set its state to idle so it can transition to something else
	targetData.spellTargetData.Clear();
	SetCurrentSpell(nullptr);
	targetData.targetUnit = nullptr;
	targetData.targetActor = nullptr;
	targetData.targetLocation = FVector();
	combatParams.readyToAttack = false;
	controller->StopMovement();
	state->ChangeState(EUnitState::STATE_IDLE);
}

void AUnit::OnMoveCompleted(FAIRequestID RequestID, const EPathFollowingResult::Type Result)
{
	if (GetState() == EUnitState::STATE_MOVING)
		state->ChangeState(EUnitState::STATE_IDLE);
	//Possible move callback unusued so far
}

void AUnit::ShowDamageDealt(Damage& d)
{
	//auto attach makes the new component the root for the sucessive components
	UDIRender* tRC = NewObject<UDIRender>(this);
	if (tRC)
	{
		tRC->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		tRC->RegisterComponent();
		tRC->Text = FText::FromString(FString::FromInt(d.damage));
		if (d.crit)
			tRC->SetWorldSize(200.f);
		else
			tRC->SetWorldSize(100.f);
		tRC->SetTextRenderColor(ResourceManager::elementalMap[d.element]);
		tRC->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
		tRC->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextBottom);
		tRC->SetRelativeLocation(FVector(0, 0, FindBoundary().Max.Y - FindBoundary().Min.Y + 100));
	}
}

void AUnit::ShowDamageDealt(FText occurance)
{
	//auto attach makes the new component the root for the sucessive components
	UDIRender* tRC = NewObject<UDIRender>(this);
	if (tRC)
	{
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

void AUnit::CalculateRisk()
{

}

void AUnit::OnUpdateGameSpeed(float speedMultiplier)
{
	GetCharacterMovement()->MaxWalkSpeed = GetMechanicAdjValue(static_cast<int>(Mechanics::MovementSpeed)) * speedMultiplier;
}

FBox2D AUnit::FindBoundary()
{
	//This function can have errors due to being called during level transition before old level is completely unloaded

	FBox2D boundary = FBox2D(ForceInit);
	FVector origin, extent;
	GetActorBounds(true, origin, extent);
	FVector2D screenLocation;
	TArray<FVector2D> corners; //get 8 corners of box

	for (int i = 0; i < 8; ++i)
	{
		controllerRef->ProjectWorldLocationToScreen(origin + extent * BoundsPointMapping[i], screenLocation, true);
		corners.Add(screenLocation);
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, corners[i].ToString() + " " + boundary.GetExtent().ToString());
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
	if (category < CombatInfo::AttCount)
	{
		baseC->GetAttribute(category)->SetCurrentValue(GetAttributeAdjValue(category) + value);
	}
	else if (category >= CombatInfo::AttCount && category < CombatInfo::AttCount + CombatInfo::StatCount)
	{
		int index = category - CombatInfo::AttCount;
		baseC->GetSkill(index)->SetBuffValue(GetSkillAdjValue(index) + value);
	}
	else if (category >= CombatInfo::AttCount + CombatInfo::StatCount && category < CombatInfo::AttCount + CombatInfo::StatCount + CombatInfo::VitalCount)
	{
		int index = category - CombatInfo::AttCount - CombatInfo::StatCount;
		baseC->GetVital(index)->SetBuffValue(GetVitalAdjValue(index) + value);
	}
	else
	{
		int index = category - CombatInfo::AttCount - CombatInfo::StatCount - CombatInfo::VitalCount;
		baseC->GetMechanic(index)->SetCurrentValue(GetMechanicAdjValue(index) + value);
	}
}
