// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Ally.h"
#include "SpellSystem/MySpell.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "UI/HUDManager.h"
#include "UI/UserWidgets/ActionbarInterface.h"
#include "UI/UserWidgets/MainWidget.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AIStuff/AIControllers/AllyAIController.h"

FText const AAlly::notEnoughManaText = NSLOCTEXT("HelpMessages", "Mana", "Not Enough Mana!");
FText const AAlly::invalidTargetText = NSLOCTEXT("HelpMessages", "Target", "Invalid Target!");
FText const AAlly::onCooldownText = NSLOCTEXT("HelpMessages", "CD", "Spell is on Cooldown!");
FText const AAlly::stunnedText = NSLOCTEXT("HelpMessages", "Stun", "Currently Stunned!");
FText const AAlly::filledQueueText = NSLOCTEXT("HelpMessages", "Queue", "Command Queue Filled!");

AAlly::AAlly(const FObjectInitializer& oI) : AUnit(oI)
{
	/* ABILITY SETUP!!! */
	state = TUniquePtr<StateMachine>(new StateMachine(this));
	abilities.SetNum(MAX_NUM_SPELLS); //size of abilities that can be used on actionbar
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel9); 
}

void AAlly::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UMySpell> ability : abilities)
	{
		if (ability.GetDefaultObject()) //if client tries to give himself ability assert fails
		{
			GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(ability.GetDefaultObject(), 1));
		}
	}

	controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
	Cast<ABasePlayer>(controllerRef->PlayerState)->allies.Add(this);
	queryParamVision.AddObjectTypesToQuery(ECC_GameTraceChannel6); //VisionBlockers
}

void AAlly::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	if(!controllerRef->IsInputKeyDown(EKeys::LeftShift) && GetState() == EUnitState::STATE_IDLE && !commandQueue.IsEmpty())
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
	if(value)
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
				if (currentSpell == spellToCast) //if already selected
				{
					currentSpell = nullptr; //deselect
					controllerRef->HideSpellCircle();
					controllerRef->SetSecondaryCursor(ECursorStateEnum::Select);
					return true;
				}

				//set our current spell to spellToCast for recording purposes
				currentSpell = spellToCast;

				if (spell->GetTargetting().GetTagName() == "Skill.Targetting.None") //non targetted?  Then just cast it
				{
					state->ChangeState(EUnitState::STATE_CASTING);
					PreCastChannelingCheck(currentSpell);
				}
				else
				{
					//set wand targetting cursor
					controllerRef->SetSecondaryCursor(ECursorStateEnum::Magic);
				}

				if (spell->GetTargetting().GetTagName() == "Skill.Targetting.Area")
				{
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
	if(Super::BeginCastSpell(spellToCastIndex,tData))
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
	if(Super::CastSpell(spellToCast))
	{
		if(controllerRef->GetBasePlayer()->focusedUnit == this)
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
	if (IsValid(spell))
	{
		if (spell->GetTargetting().GetTagName() == "Skill.Targetting.Area") // we can target anything and the area around it will be affected
		{	
			targetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(result);
			targetLocation = UAbilitySystemBlueprintLibrary::GetTargetDataEndPoint(targetData,0);
		}
		else //we need to check to see if we targetted the right kind of actor
		{
			if (result.Actor->GetClass()->IsChildOf(AUnit::StaticClass()))
			{
				AUnit* unit = Cast<AUnit>(result.Actor.Get());
				if (unit->GetIsEnemy())
				{
					if (spell->GetTargetting().GetTagName() == "Skill.Targetting.Friendly")
					{
						controllerRef->GetHUDManager()->GetMainHUD()->DisplayHelpText(invalidTargetText);
						return false;
					}
				}
				else //if not enemy
				{
					if (spell->GetTargetting().GetTagName() == "Skill.Targetting.Enemy")
					{
						controllerRef->GetHUDManager()->GetMainHUD()->DisplayHelpText(invalidTargetText);
						return false;
					}
				}

				targetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(result);
				targetUnit = unit;

				//Stop any queued commands since we're interrupting them to cast a spell
				ClearCommandQueue();
				
				//If casting on ourselves, then we can just instantly cast
				if (targetUnit == this || FVector::Dist2D(targetLocation, GetActorLocation()) < 5.f)
				{
					PreCastChannelingCheck(spellClass);
					controllerRef->ChangeCursor(ECursorStateEnum::Select);
					return true;
				}

			}
			else
			{
				controllerRef->GetHUDManager()->GetMainHUD()->DisplayHelpText(invalidTargetText);
				return false;
			}
		}
		controllerRef->ChangeCursor(ECursorStateEnum::Select); //just turn it back to select so the loop will quickly change the cursor back to normal after spell casted
		state->ChangeState(EUnitState::STATE_CASTING);
		return true;
	}
	return false;
}
