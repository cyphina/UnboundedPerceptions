// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Ally.h"
#include "SpellSystem/MySpell.h"
#include "RTSGameState.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "UI/HUDManager.h"
#include "UI/UserWidgets/ActionbarInterface.h"
#include "UI/UserWidgets/MainWidget.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AIStuff/AIControllers/AllyAIController.h"
#include "Enemy.h"
#include "InteractableBase.h"

FText const AAlly::notEnoughManaText = NSLOCTEXT("HelpMessages", "Mana", "Not Enough Mana!");
FText const AAlly::invalidTargetText = NSLOCTEXT("HelpMessages", "Target", "Invalid Target!");
FText const AAlly::onCooldownText = NSLOCTEXT("HelpMessages", "CD", "Spell is on Cooldown!");
FText const AAlly::stunnedText = NSLOCTEXT("HelpMessages", "Stun", "Currently Stunned!");
FText const AAlly::filledQueueText = NSLOCTEXT("HelpMessages", "Queue", "Command Queue Filled!");

AAlly::AAlly(const FObjectInitializer& oI) : AUnit(oI)
{
	state = TUniquePtr<StateMachine>(new StateMachine(this));
	abilities.SetNum(MAX_NUM_SPELLS); //size of abilities that can be used on actionbar
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel9);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel10, ECollisionResponse::ECR_Block); //traceAllyOnly
	visionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAlly::OnVisionSphereOverlap);
	visionSphere->OnComponentEndOverlap.AddDynamic(this, &AAlly::OnVisionSphereEndOverlap);
	visionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap); //Enemy
#if UE_EDITOR
	visionSphere->SetHiddenInGame(false);
#endif
}

void AAlly::BeginPlay()
{
	Super::BeginPlay();

	Cast<ABasePlayer>(controllerRef->PlayerState)->allies.Add(this);
	Cast<ARTSGameState>(GetWorld()->GetGameState())->allyList.Add(this);
	queryParamVision.AddObjectTypesToQuery(ECC_GameTraceChannel6); //VisionBlockers
}

void AAlly::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	if (!controllerRef->IsInputKeyDown(EKeys::LeftShift) && GetState() == EUnitState::STATE_IDLE && !commandQueue.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString(TEXT("Command Dequeued")));
		TFunction<void()> command;
		commandQueue.Dequeue(command);
		--queueCount;
		command();
	}
}

void AAlly::Destroyed()
{
	Super::Destroyed();
	//controllerRef->basePlayer->allies.Remove(this);
}

void AAlly::PossessedBy(AController* newAllyControllerRef)
{
	Super::PossessedBy(newAllyControllerRef);
	allyControllerRef = Cast<AAllyAIController>(newAllyControllerRef);
}

void AAlly::SetSelected(bool value)
{
	Super::SetSelected(value);
	if (value)
	{
		controllerRef->GetBasePlayer()->selectedAllies.AddUnique(this);
	}
	else
	{
		controllerRef->GetBasePlayer()->selectedAllies.Remove(this);
	}
}

void AAlly::SetSpellIndex(int index)
{
#if UE_EDITOR
	check(index >= 0 && index < MAX_NUM_SPELLS)
#endif
		spellIndex = index;
}

UGameplayAbility* AAlly::GetSpellInstance(TSubclassOf<UMySpell> spellClass) const
{
	if (spellClass)
	{
		if (GetAbilitySystemComponent()->FindAbilitySpecFromClass(spellClass)->Ability)
			return GetAbilitySystemComponent()->FindAbilitySpecFromClass(spellClass)->GetAbilityInstances()[0];
	}
	return nullptr;
}

bool AAlly::PressedCastSpell(TSubclassOf<UMySpell> spellToCast)
{
	UMySpell* spell = spellToCast.GetDefaultObject();
	if (IsValid(spell)) //valid spell class check
	{
		if (!spell->isOnCD(GetAbilitySystemComponent()))
		{
			if (spell->GetCost(GetAbilitySystemComponent()) <= GetVitalCurValue(static_cast<int>(Vitals::Mana)))
			{
				if (GetCurrentSpell() == spellToCast) //if already selected
				{
					SetCurrentSpell(nullptr); //deselect
					controllerRef->HideSpellCircle();
					controllerRef->SetSecondaryCursor(ECursorStateEnum::Select);
					return true;
				}

				//set our current spell to spellToCast for recording purposes
				SetCurrentSpell(spellToCast);

				if (spell->GetTargetting().GetTagName() == "Skill.Targetting.None") //non targetted?  Then just cast it
				{
					state->ChangeState(EUnitState::STATE_CASTING);
					PreCastChannelingCheck(GetCurrentSpell());
				}
				else
				{
					//set wand targetting cursor
					controllerRef->SetSecondaryCursor(ECursorStateEnum::Magic);
				}

				if (spell->GetTargetting().MatchesTag(FGameplayTag::RequestGameplayTag("Skill.Targetting.Area")))
				{
					//TODO: depending on the spell area targetting, use different indicators
					//if it's an AOE spell show the targetting indicator
					controllerRef->ShowSpellCircle(spell->GetAOE(GetAbilitySystemComponent()));
				}
				else
				{
					controllerRef->HideSpellCircle();
				}

				return true;
			}
			else //if not enough mana 
			{
				controllerRef->GetHUDManager()->GetMainHUD()->DisplayHelpText(notEnoughManaText);
				return false;
			}

		}
		else //if spell still not ready
		{
			controllerRef->GetHUDManager()->GetMainHUD()->DisplayHelpText(onCooldownText);
			return false;
		}
	}
	return false;
}

bool AAlly::BeginCastSpell(int spellToCastIndex, FGameplayAbilityTargetDataHandle tData)
{
	SetSpellIndex(spellToCastIndex);
	if (Super::BeginCastSpell(spellToCastIndex, tData))
	{
		return true;
	}
	return false;
}

void AAlly::Stop()
{
	Super::Stop();
}

void AAlly::QueueAction(TFunction<void()> actionToQueue)
{
	if (queueCount < 20)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Emerald, TEXT("SUCESSFUL QUEUE"));
		commandQueue.Enqueue(actionToQueue);
		++queueCount;
	}
	else
		controllerRef->GetHUDManager()->GetMainHUD()->DisplayHelpText(filledQueueText);
}

bool AAlly::CastSpell(TSubclassOf<UMySpell> spellToCast)
{
	if (Super::CastSpell(spellToCast))
	{
		if (controllerRef->GetBasePlayer()->focusedUnit == this)
			controllerRef->GetHUDManager()->GetActionHUD()->ShowSkillVisualCD(spellIndex);
		return true;
	}
	return false;
}

bool AAlly::GetOverlappingObjects(TArray<FHitResult>& hits)
{
	//For debugging trace
	const FName TraceTag("MyTraceTag");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	//Sweep from capsule around hero to camera location
	FVector start = GetActorLocation() + FVector(0, 0, GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation().Z);
	FVector end = GetActorLocation();
	//FCollisionQueryParams params;
	//params.TraceTag = TraceTag;
	UCapsuleComponent* capsule = GetCapsuleComponent();
	if (capsule)
		GetWorld()->SweepMultiByObjectType(hits, start, end, FQuat::Identity, queryParamVision, FCollisionShape::MakeCapsule(capsule->GetScaledCapsuleRadius(), capsule->GetScaledCapsuleHalfHeight() * 2));
	if (!hits.Num())
		return false;
	return true;
}

bool AAlly::SetupSpellTargetting(FHitResult result, TSubclassOf<UMySpell> spellClass)
{
	UMySpell* spell = spellClass.GetDefaultObject();
	FName spellTargetting = spell->GetTargetting().GetTagName();

	if (IsValid(spell))
	{
		if (spellTargetting == "Skill.Targetting.Area") // we can target anything and the area around it will be affected
		{
			targetData.spellTargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(result);
			targetData.targetLocation = UAbilitySystemBlueprintLibrary::GetTargetDataEndPoint(targetData.spellTargetData, 0);
		}
		else //we need to check to see if we targetted the right kind of actor
		{
			if (IsValid(result.Actor.Get()))
			{
				if (result.Actor->GetClass()->IsChildOf(AUnit::StaticClass()))
				{
					if (spellTargetting != "Skill.Targetting.Single.Interactable")
					{
						AUnit* unit = Cast<AUnit>(result.Actor.Get());
						if (unit->GetIsEnemy())
						{
							if (spellTargetting == "Skill.Targetting.Single.Friendly")
							{
								controllerRef->GetHUDManager()->GetMainHUD()->DisplayHelpText(invalidTargetText);
								return false;
							}
						}
						else //if not enemy
						{
							if (spellTargetting == "Skill.Targetting.Single.Enemy")
							{
								controllerRef->GetHUDManager()->GetMainHUD()->DisplayHelpText(invalidTargetText);
								return false;
							}
						}

						targetData.spellTargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(result);
						targetData.targetActor = unit;
						targetData.targetUnit = unit;

						//If casting on ourselves, then we can just instantly cast
						if (targetData.targetUnit == this || FVector::Dist2D(targetData.targetLocation, GetActorLocation()) < 5.f)
						{
							PreCastChannelingCheck(spellClass);
							controllerRef->ChangeCursor(ECursorStateEnum::Select);
							return true;
						}
					}
				}
				else if (result.Actor->GetClass()->IsChildOf(AInteractableBase::StaticClass()))
				{
					if (spellTargetting != "Skill.Targetting.Single.Interactable" && spellTargetting != "Skill.Targetting.Single.AllUnitsAndInteractables")
					{
						controllerRef->GetHUDManager()->GetMainHUD()->DisplayHelpText(invalidTargetText);
						return false;
					}

					targetData.targetActor = Cast<AInteractableBase>(result.Actor);
					targetData.spellTargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(result);
				}
				else
				{
					controllerRef->GetHUDManager()->GetMainHUD()->DisplayHelpText(invalidTargetText);
					return false;
				}
			}
			else
			{
				controllerRef->GetHUDManager()->GetMainHUD()->DisplayHelpText(invalidTargetText);
				return false;
			}
		}

		//Stop any queued commands since we're interrupting them to cast a spell
		ClearCommandQueue();

		controllerRef->ChangeCursor(ECursorStateEnum::Select); //just turn it back to select so the loop will quickly change the cursor back to normal after spell casted
		state->ChangeState(EUnitState::STATE_CASTING);
		return true;
	}
	return false;
}

void AAlly::OnVisionSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int otherBodyIndex, bool fromSweep, const FHitResult& sweepRes)
{
	if (otherActor->IsA(AEnemy::StaticClass()))
	{
		AEnemy* enemy = Cast<AEnemy>(otherActor);
		//TODO: Check for invisibility
		possibleEnemiesInRadius.Add(enemy);
		enemy->IncVisionCount();
	}
}

void AAlly::OnVisionSphereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	AEnemy* enemy = Cast<AEnemy>(otherActor);
	if (IsValid(enemy))
	{
		possibleEnemiesInRadius.Remove(enemy);
		enemy->DecVisionCount();
		if (!enemy->GetVisionCount())
		{
			enemy->GetCapsuleComponent()->SetVisibility(false, true);
			gameState->visibleEnemies.Remove(enemy);
		}
	}
}
