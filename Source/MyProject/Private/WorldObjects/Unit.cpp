                      // Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSGameState.h"
#include "Unit.h"
#include "MyGameInstance.h"
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
#include "SpellSystem/Calcs/DamageCalculation.h"
#include "AbilitySystemBlueprintLibrary.h"

AUnit::AUnit(const FObjectInitializer& objectInitializer) : Super(objectInitializer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	//Setup variables
	PrimaryActorTick.bCanEverTick = true;
	name = FText::FromString(GetName()); //use this for now.
	combatStyle = FGameplayTag::RequestGameplayTag("Combat.Style.Melee");                      

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
	abilitySystem = CreateDefaultSubobject<UMyAbilitySystemComponent>(TEXT("AbilitySystem"));
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
}

void AUnit::BeginPlay()
{
	Super::BeginPlay();

	///---Setup initial parameters---
	controller = Cast<AAIController>(GetController());	
	gameState = Cast<ARTSGameState>(GetWorld()->GetGameState());
	gameInstance = Cast<UMyGameInstance>(GetGameInstance());

	FVector origin, extent;
	GetActorBounds(true, origin, extent);
	height = origin.Z + extent.Z; //manually setup height informatoin for other things to read it


	//Runtime component configuration
	if(healthBar)
	{
		if (!healthBar->IsRegistered()) //Before calling any functions register the component.  Widget for widgetcomponent is created during register
			healthBar->RegisterComponent();

		healthBar->SetWorldLocation(FVector::ZeroVector);
		healthBar->SetRelativeLocation(healthBar->GetComponentLocation() + FVector(0, 0, height/3));	
	}

	if(selectionCircleDecal)
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
	if (abilitySystem)
	{
		//Make sure owner is player controller else this won't work
		abilitySystem->InitAbilityActorInfo(GetWorld()->GetFirstPlayerController(), this); //setup owner and avatar
		baseC = TUniquePtr<FBaseCharacter>(new FBaseCharacter(abilitySystem->AddSet<UMyAttributeSet>()));
		GetCharacterMovement()->MaxWalkSpeed = GetMechanicAdjValue(static_cast<int>(Mechanics::MovementSpeed));
		//abilitySystem->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &AUnit::RemoveGameplayEffects);
	}
}

void AUnit::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	//Calculate when we can attack again 
	if (!readyToAttack && currentAttTime < 2 / GetSkillAdjValue(static_cast<int>(UnitStats::Attack_Speed)))
	{
		currentAttTime += deltaSeconds * gameState->speedModifier;
	}
	else
	{
		if (readyToAttack == false)
		{
			currentAttTime = 0;
			readyToAttack = true;
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
	return abilitySystem;
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
	int cross =  FVector::CrossProduct(currentLocation, targetLocation);
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
	if(!IsTargetInRange(range, targetLoc))
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
			//shift location a little bit if we're moving multiple units so they can group together ok
			FVector shiftedLocation = newLocation - GetActorLocation().GetSafeNormal() * GetCapsuleComponent()->GetScaledCapsuleRadius() / 2;
			controller->MoveToLocation(shiftedLocation, 10);
			state->ChangeState(EUnitState::STATE_MOVING);
		}
	}
}

void AUnit::Die()
{
	//Spawn a corpse

	//Disable alive features 
	GetRootComponent()->SetVisibility(false, true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	SetCanTarget(false);
	isDead = true;

	//Trigger "Death Events"
	FGameplayEventData eD = FGameplayEventData();
	eD.EventTag = UGameplayTagsManager::Get().RequestGameplayTag("Event.Death");
	eD.TargetData = targetData;
	if (abilitySystem->HandleGameplayEvent(UGameplayTagsManager::Get().RequestGameplayTag("Event.Death"), &eD))
	{
		//GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Emerald, FString("Wee") + FString::FromInt(currentSpellIndex));
	}

	//Destroy?
}

void AUnit::CommitCast(UMySpell* spell)
{
	int spellResIndex = static_cast<int>(Vitals::Mana); //index of resource to be used to cast spell
	if (spell && spell->GetCost(abilitySystem) <= GetVitalCurValue(spellResIndex)) //if we have enough mana (in the future different spells may have their own resources)
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
	targetUnit = target;
	targetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(target);
}

void AUnit::PrepareAttack()
{
	if (!IsStunned() && targetUnit)
	{
		if (targetUnit->GetCanTarget())
		{
			FVector targetLoc = FVector(targetUnit->GetActorLocation().X, targetUnit->GetActorLocation().Y, 0);
			//if (unitTarget->GetIsEnemy()) //may remove this check because we may be able to hit allies
			//{
			if (AdjustPosition(GetMechanicAdjValue(static_cast<int>(Mechanics::AttackRange)), targetLoc))
			{
				Attack();
			}
		}
		else
		{
			targetUnit = nullptr;
		}
	}
}

void AUnit::Attack()
{
	//Attack Time = 60 / WeaponAttkSpd / AttkSpeed
	if (!IsStunned() && readyToAttack) //If we're not stunned and our attack rate is filled
	{
		//play attack animation
		//Create a gameplay effect for this
		FGameplayEffectContextHandle context = GetAbilitySystemComponent()->MakeEffectContext();
		context.AddInstigator(this, this);
		FGameplayEffectSpecHandle damageEffectHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(UDamageEffect::StaticClass(), 1, context);
		//set all the effect's custom magnitude values else it complains
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Strength"), 0);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Intelligence"), 0);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Agility"), 0);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Understanding"), 0);

		if(combatStyle == FGameplayTag::RequestGameplayTag("Combat.Style.Melee"))
		{
			UAbilitySystemBlueprintLibrary::AssignSetByCallerMagnitude(damageEffectHandle, "Strength", 100);
		}
		else if(combatStyle == FGameplayTag::RequestGameplayTag("Combat.Style.Magic"))
		{
			UAbilitySystemBlueprintLibrary::AssignSetByCallerMagnitude(damageEffectHandle, "Intelligence", 100);
		}
		else if(combatStyle == FGameplayTag::RequestGameplayTag("Combat.Style.Ranged"))
		{
			UAbilitySystemBlueprintLibrary::AssignSetByCallerMagnitude(damageEffectHandle, "Agility", 100);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Error, combatstyle tag is not what it should be!"));
			return;
		}
		//Should add weapon element here
		UAbilitySystemBlueprintLibrary::AddAssetTag(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Element.Force"));
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*damageEffectHandle.Data.Get(), targetUnit->abilitySystem);
		readyToAttack = false;
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
		if (spell->GetCost(abilitySystem) <= GetVitalCurValue(static_cast<int>(Vitals::Mana))) //Enough Mana?
		{
			if(!spell->isOnCD(GetAbilitySystemComponent()) && !IsStunned() && !IsSilenced()) //Spell on CD and we aren't affected by any status that prevents us
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
			currentSpell = abilities[spellToCastIndex];

			if (spell->GetTargetting().GetTagName() == "Skill.Targetting.None") //non targetted?  Then just cast it
			{
				state->ChangeState(EUnitState::STATE_CASTING);
				PreCastChannelingCheck(currentSpell);
				return true;
			}
			else
			{
				if (tData.Num() > 0)
				{
					targetData = tData;
					//here error
					if (spell->GetTargetting().MatchesTag(FGameplayTag::RequestGameplayTag("Skill.Targetting.Area")))
					{
						targetLocation = UAbilitySystemBlueprintLibrary::GetTargetDataEndPoint(targetData, 0);
					}
					else
					{
						targetActor = UAbilitySystemBlueprintLibrary::GetActorsFromTargetData(targetData, 0)[0];
					}

					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("%f"), FVector::Dist2D(targetLocation, GetActorLocation())));
					if (targetActor == this || FVector::Dist2D(targetLocation, GetActorLocation()) < 5.f)
					{
						PreCastChannelingCheck(currentSpell);
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
	if (CanCast(currentSpell))
	{
		UMySpell* spell = currentSpell.GetDefaultObject();

		FVector targetLoc;
		if (spell->GetTargetting().MatchesTag(FGameplayTag::RequestGameplayTag("Skill.Targetting.Area")))
			targetLoc = targetLocation;
		else
		{
			targetLoc = targetActor->GetActorLocation();
		}
		if (AdjustPosition(spell->GetRange(GetAbilitySystemComponent()), targetLoc))
		{
			PreCastChannelingCheck(currentSpell);
		}
	}
}

bool AUnit::CastSpell(TSubclassOf<UMySpell> spellToCast)
{
	if (abilitySystem->TryActivateAbilityByClass(spellToCast))
	{
		if (spellToCast.GetDefaultObject()->GetTargetting().GetTagName() != "Skill.Targetting.None")
		{
			//When spell is actually casted, trigger castclick to let the ability know it should be ready to move on...
			//May be obsolete since we don't actully activate the spell until we click anyways.  However, it does pass in target data
			FGameplayEventData eD = FGameplayEventData();
			eD.EventTag = UGameplayTagsManager::Get().RequestGameplayTag("Event.CastClick");
			eD.TargetData = targetData;
			if (abilitySystem->HandleGameplayEvent(UGameplayTagsManager::Get().RequestGameplayTag("Event.CastClick"), &eD))
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
	if(!currentSpell.GetDefaultObject()->GetCastTime(GetAbilitySystemComponent()))
	{
		CastSpell(currentSpell);
	}
	else
	{
		state->ChangeState(EUnitState::STATE_CHANNELING); 	//start channeling
	}
}

void AUnit::Stop()
{
	//Stop will not only stop unit in its track, but will also make it forget its target, and set its state to idle so it can transition to something else
	targetData.Clear();
	currentSpell = nullptr;
	targetUnit = nullptr;
	targetActor = nullptr;
	targetLocation = FVector();
	readyToAttack = false;
	controller->StopMovement();
	state->ChangeState(EUnitState::STATE_IDLE);
}

//Relic of when I did this manually
// void AUnit::ApplyGameplayEffects(const FGameplayEffectSpec& effect)
// {
// 	for (FGameplayEffectModifiedAttribute modifier : effect.ModifiedAttributes)
// 	{
// #if UE_EDITOR
// 		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString("Numeric Attribute Value") + FString::FromInt(modifier.TotalMagnitude) + FString(" Attribute Index") + FString::FromInt(GetAbilitySystemComponent()->GetNumericAttributeBase(modifier.Attribute) - modifier.TotalMagnitude));
// 		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString ("Numeric Attributes") + FString::FromInt(GetAbilitySystemComponent()->GetNumericAttributeBase(modifier.Attribute)));
// #endif
// 		//Crappy fix but we negate the modification on our attributeset since we have our own stats system but need the base value to be the same to continue to index.  
// 		//GetAbilitySystemComponent()->SetNumericAttributeBase(modifier.Attribute, GetAbilitySystemComponent()->GetNumericAttributeBase(modifier.Attribute) - modifier.TotalMagnitude);
// 		int index = GetAbilitySystemComponent()->GetNumericAttributeBase(modifier.Attribute);
// 		if (index < CombatInfo::AttCount)
// 		{
// 			//baseC->GetAttribute(GetAbilitySystemComponent()->GetNumericAttributeBase(modifier.Attribute))->SetBuffValue(modifier.TotalMagnitude);
// 		}
// 		else if (index < CombatInfo::AttCount + CombatInfo::StatCount)
// 		{
// 			index = index - CombatInfo::AttCount;
// 			baseC->GetSkill(index)->SetBuffValue(modifier.TotalMagnitude);
// 		}
// 		else if(index < CombatInfo::AttCount + CombatInfo::StatCount + CombatInfo::VitalCount)
// 		{
// 			index = index - (CombatInfo::AttCount + CombatInfo::StatCount);
// 			baseC->GetVital(index)->SetBuffValue(modifier.TotalMagnitude);
// 		}
// 		else if(index < CombatInfo::AttCount + CombatInfo::StatCount + CombatInfo::VitalCount + CombatInfo::MechanicCount)
// 		{
// 			index = index - (CombatInfo::AttCount + CombatInfo::StatCount + CombatInfo::VitalCount);
// 			baseC->GetMechanic(index)->SetCurrentValue(modifier.TotalMagnitude);
// 		}
// 		else
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("ERROR WITH EFFECT INDICES"));
// 		}
// 	}
// }

void AUnit::OnMoveCompleted(FAIRequestID RequestID, const EPathFollowingResult::Type Result)
{
	if(GetState() == EUnitState::STATE_MOVING)
		state->ChangeState(EUnitState::STATE_IDLE);
	//Possible move callback unusued so far
}

// void AUnit::RemoveGameplayEffects(const FActiveGameplayEffect& effect) //two ways of doing this, we can either bind it to an active event when we activate the ability, 
// //else we can do a cast everytime to check but then we don't have to worry about adding the bindings every time
// {
// 	FGameplayTagContainer tagContainer;
// 	effect.Spec.GetAllAssetTags(tagContainer);
// #if UE_EDITOR
// 	for(FGameplayTag tag : tagContainer)
// 	{

// 		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, tag.GetTagName().ToString());
// 	}
// #endif
// 	if(tagContainer.HasTag(UGameplayTagsManager::Get().RequestGameplayTag("Skill.Effect.StatChange")))
// 	{ 
// 		for (FGameplayEffectModifiedAttribute modifier : effect.Spec.ModifiedAttributes)
// 		{
// 			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::FromInt(GetAbilitySystemComponent()->GetNumericAttributeBase(modifier.Attribute)));
// 			int index = GetAbilitySystemComponent()->GetNumericAttributeBase(modifier.Attribute);
// 			if (index < CombatInfo::AttCount)
// 			{
// 				//Attribute* att = baseC->GetAttribute(index);
// 				//att->SetBuffValue(att->GetBuffValue() - modifier.TotalMagnitude);
// 			}
// 			else if (index < CombatInfo::AttCount + CombatInfo::StatCount)
// 			{
// 				index = index - CombatInfo::AttCount;
// 				Stat* stat = baseC->GetSkill(index);
// 				stat->SetBuffValue(stat->GetBuffValue() - modifier.TotalMagnitude);
// 			}
// 			else if (index < CombatInfo::AttCount + CombatInfo::StatCount + CombatInfo::VitalCount)
// 			{
// 				index = index - (CombatInfo::AttCount + CombatInfo::StatCount);
// 				Vital* vital = baseC->GetVital(index);
// 				vital->SetBuffValue(vital->GetBuffValue() - modifier.TotalMagnitude);
// 			}
// 			else if (index < CombatInfo::AttCount + CombatInfo::StatCount + CombatInfo::VitalCount + CombatInfo::MechanicCount)
// 			{
// 				index = index - (CombatInfo::AttCount + CombatInfo::StatCount + CombatInfo::VitalCount);
// 				FGameplayAttributeData* mech = baseC->GetMechanic(index);
// 				mech->SetCurrentValue(mech->GetCurrentValue() - modifier.TotalMagnitude);
// 			}
// 			else
// 			{
// 				UE_LOG(LogTemp, Warning, TEXT("ERROR WITH EFFECT INDICES"));
// 				//error 
// 			}
// 		}
// 	}
// }

void AUnit::ShowDamageDealt(Damage& d)
{
	FTimerHandle unusedHandle;
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
		tRC->SetRelativeLocation(FVector(0,0,FindBoundary().Max.Y - FindBoundary().Min.Y + 100));	
		tRC->ready = true;
		//GetWorldTimerManager().SetTimer(unusedHandle, this, &AUnit::deleteDI, 2.f, false);
	}
}

void AUnit::ShowDamageDealt(FText occurance)
{
	//display dodged 
}

void AUnit::OnUpdateGameSpeed(float speedMultiplier)
{
	GetCharacterMovement()->MaxWalkSpeed = GetMechanicAdjValue(static_cast<int>(Mechanics::MovementSpeed)) * speedMultiplier;
}

void AUnit::deleteDI()
{
	TSubclassOf<UTextRenderComponent> dIClass;
	UTextRenderComponent* comp = FindComponentByClass<UTextRenderComponent>();
	if(comp)
		comp->DestroyComponent(false);
}

FBox2D AUnit::FindBoundary()
{
	FBox2D boundary = FBox2D(ForceInit);
	FVector origin, extent;
	GetActorBounds(true, origin, extent);
	FVector2D screenLocation;
	TArray<FVector2D> corners; //get 8 corners of box

	for(int i = 0; i < 8; ++i)
	{
		GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(origin + extent * BoundsPointMapping[i], screenLocation, true);
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
	return GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Debuff.Silenced")) ?  true : false;
}
