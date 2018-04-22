// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BaseHero.h"
#include "Interactables/Interactable.h"
#include "BasePlayer.h"
#include "UserInput.h"
#include "State/StateMachine.h"
#include "Items/Weapon.h"
#include "Items/Consumable.h"
#include "AbilitySystemComponent.h"
#include "SpellSystem/MySpell.h"
#include "Materials/MaterialInstanceDynamic.h"

const float ABaseHero::nextExpMultiplier = 1.5f;

//---Object Fundamentals---
#pragma region Fundamentals
ABaseHero::ABaseHero(const FObjectInitializer& oI) : AAlly(oI)
{	
	isEnemy = false;
	backpack = CreateDefaultSubobject<UBackpack>(FName("Backpack"));

	ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> materialPC(TEXT("/Game/RTS_Tutorial/Materials/CelShade/LightSource"));
	if (materialPC.Object)
	{
		lightSource = materialPC.Object;
	}
}

// Called when the game starts or when spawned
void ABaseHero::BeginPlay()
{
	Super::BeginPlay();
	//Setup player reference
	player = Cast<ABasePlayer>(controllerRef->PlayerState);

	//Setup dynamic material instance and vector parameter references
	if (GetMesh() != nullptr)
	{
		material = GetMesh()->GetMaterial(0);
		dMat = UMaterialInstanceDynamic::Create(material, this);
		if (material != nullptr)
		{
			FLinearColor initDirection;
#if UE_BUILD_DEBUG
			if (!material->GetVectorParameterValue("BodyColor", baseColor))
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Failed to get material color"));
			else
#endif
				currentColor = baseColor;
#if UE_BUILD_DEBUG
			if (!material->GetVectorParameterValue("InstDirectLight", initDirection))
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::Printf(TEXT("Failed to get light direction for %s"), *name.ToString()));
#endif
			GetMesh()->SetMaterial(0, dMat);
		}
	}

	//Setup initial states
	currentInteractable = nullptr;
	//Set light direction parameters
	lightDirectionPV = GetWorld()->GetParameterCollectionInstance(lightSource);
	//Default backpack size is 40 items
	backpack->SetItemMax(40);
	//this is the size of a hero's equipment 
	equips.InsertDefaulted(0,10);
	
#if UE_EDITOR
	for(int i = 0; i < 70; i++)
	{
		baseC->LevelUp();
	}
#endif
}

// Called every frame
void ABaseHero::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);
	//if (dMat)
	//	CheckShadows();
}

void ABaseHero::Destroyed()
{
	Super::Destroyed();
	if (player)
	{
		player->heroes.RemoveAt(player->heroes.Find(this));
		Die();
	}
}

void ABaseHero::EndPlay(EEndPlayReason::Type epr)
{
	Super::EndPlay(EEndPlayReason::Quit);
}
#pragma endregion

//--EXP Functions--
#pragma region Acessors
int ABaseHero::GetCurrentExp() const
{
	return currentExp;
}

int ABaseHero::GetExpToLevel() const
{
	return expForLevel;
}

void ABaseHero::SetCurrentExp(int amount)
{
	currentExp += amount;
	while(currentExp >= expForLevel)
	{
		currentExp = currentExp - expForLevel;
		expForLevel *= nextExpMultiplier; //
		LevelUp();
	}
}

UObject* ABaseHero::GetCurrentInteractable() const
{
	return Cast<UObject>(currentInteractable);
}

void ABaseHero::SetCurrentInteractable(AActor* interactable)
{
	IInteractable* interactor = Cast<IInteractable>(interactable);
	if (interactor)
		currentInteractable = interactor;
	else
		currentInteractable = nullptr;
}

#pragma endregion

#pragma region BaseCStuff
void ABaseHero::LevelUp_Implementation()
{
	attPoints += 5;
	skillPoints += 1;
	baseC->LevelUp();
}

void ABaseHero::ChangeAttribute(Attributes att, bool isIncrementing)
{
	if (isIncrementing)
	{
		GetAbilitySystemComponent()->SetNumericAttributeBase(UMyAttributeSet::IndexAtts(static_cast<int>(att)), baseC->GetAttribute(static_cast<int>(att))->GetBaseValue() + 1);	
		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::FromInt(baseC->GetAttribute(static_cast<int>(att))->GetBaseValue()));
		--attPoints;
		baseC->StatUpdate();
	}
	else
	{
		GetAbilitySystemComponent()->SetNumericAttributeBase(UMyAttributeSet::IndexAtts(static_cast<int>(att)), baseC->GetAttribute(static_cast<int>(att))->GetBaseValue() - 1);	
		++attPoints;
		baseC->StatUpdate();
	}
}
#pragma endregion

#pragma region actions

void ABaseHero::BeginInteract(AActor* interactor)
{
	if (IInteractable* intractable = Cast<IInteractable>(interactor))
	{
		state.ChangeState(EUnitState::STATE_INTERACTING);
		currentInteractable = intractable;
	}
}

void ABaseHero::BeginUseItem(UConsumable* itemToUse)
{
	SetCurrentItem(itemToUse);
	state.ChangeState(EUnitState::STATE_ITEM);
	PressedCastSpell(itemToUse->ability);
	if (itemToUse->ability.GetDefaultObject()->GetTargetting() != FGameplayTag::RequestGameplayTag("Skill.Targetting.None"))
		controllerRef->SetSecondaryCursor(ECursorStateEnum::Item);
}

void ABaseHero::PrepareInteract()
{
	if (!IsStunned() && currentInteractable)
	{
		FVector targetLoc = currentInteractable->GetInteractableLocation_Implementation();
		if (AdjustPosition(interactRange, targetLocation))
		{
			AActor* interactor = Cast<AActor>(currentInteractable);
			currentInteractable->Execute_Interact(interactor, this);
			Stop();
		}
	}
}

void ABaseHero::UseItem()
{
	if (currentItem)
	{
		PreCastChannelingCheck(currentItem->ability);
	}
}
#pragma endregion

#pragma region equipment
void ABaseHero::SetupBonuses(UEquip* e, bool isEquip)
{
	for (int i = 0; i < e->GetBonuses().Num(); i++)
	{
		if (e->GetBonuses()[i] < CombatInfo::AttCount)
		{
			baseC->GetAttribute(e->GetBonuses()[i])->SetBaseValue(GetAttributeBaseValue(e->GetBonuses()[i]) + (2*isEquip-1)*e->GetBonusValues()[i]);
		}
		else if (e->GetBonuses()[i] >= CombatInfo::AttCount && e->GetBonuses()[i] < CombatInfo::AttCount + CombatInfo::StatCount)
		{
			int index = e->GetBonuses()[i] - CombatInfo::AttCount;
			baseC->GetSkill(index)->SetBaseValue(GetSkillBaseValue(index) + (2 * isEquip - 1)*e->GetBonusValues()[i]);
		}
		else if (e->GetBonuses()[i] >= CombatInfo::AttCount + CombatInfo::StatCount && e->GetBonuses()[i] < CombatInfo::AttCount + CombatInfo::StatCount + CombatInfo::VitalCount)
		{
			int index = e->GetBonuses()[i] - CombatInfo::AttCount - CombatInfo::StatCount;
			baseC->GetVital(index)->SetBaseValue(GetVitalBaseValue(index) + (2 * isEquip - 1)*e->GetBonusValues()[i]);
		}
		else
		{
			int index = e->GetBonuses()[i] - CombatInfo::AttCount - CombatInfo::StatCount - CombatInfo::VitalCount;
			baseC->GetMechanic(index)->SetBaseValue(GetMechanicBaseValue(index) + (2 * isEquip - 1) * e->GetBonusValues()[i]);
		}
	}
}

void ABaseHero::SwapEquips(UEquip* e, int equipSlot)
{
	if (equips[equipSlot])
	{
		//if we sucessfully add our item that is equipped to the inventory
		if (backpack->AddItem(equips[equipSlot]))
		{
			//remove bonuses and replace equipment
			SetupBonuses(equips[equipSlot], false);
			equips[equipSlot] = e;
			SetupBonuses(e, true);
		}
	}
	else
	{
		//just remove item and set bonuses
		equips[equipSlot] = e;
		SetupBonuses(e, true);
	}
}

void ABaseHero::EquipItem(UEquip* e)
{
	//Depending on what kind of equip we have, swap item into different slots allocated for that equip
	if (e->itemInfo.itemType.GetTagName() == "Item.Equippable.Armor.Helmet")
	{
		SwapEquips(e, 0);
	}
	else if (e->itemInfo.itemType.GetTagName() == "Item.Equippable.Armor.Body")
	{
		SwapEquips(e, 1);
	}
	else if (e->itemInfo.itemType.GetTagName() == "Item.Equippable.Armor.Legs")
	{
		SwapEquips(e, 2);
	}
	else if (e->itemInfo.itemType.GetTagName() == "Item.Equippable.Armor.Accessory")
	{
		if (!equips[3])
			SwapEquips(e, 3);
		else
			SwapEquips(e, 4);
	}
	else if (e->itemInfo.itemType.GetTagName() == "Item.Equippable.Weapon")
	{
		for(int i = 5; i < 9; i++)
		{
			if(!equips[i])
			{
				SwapEquips(e, i);
				return;
			}
		} //if our equip slots are full, just swap with the last equip
		SwapEquips(e, 10);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Error, equipping unknown type of item");
	}
}

void ABaseHero::UnequipItem(int slot)
{
	if (equips[slot])
	{
		backpack->AddItem(equips[slot]);
		SetupBonuses(equips[slot], false);
		equips[slot] = nullptr;
	}
}

void ABaseHero::Stop()
{
	Super::Stop();
	currentInteractable = nullptr;
	currentItem = nullptr;
}

void ABaseHero::SetSelected(bool value)
{
	Super::SetSelected(value);
	if (value)
	{
		controllerRef->GetBasePlayer()->selectedHeroes.AddUnique(this);
	}
	else
	{
		controllerRef->GetBasePlayer()->selectedHeroes.Remove(this);
	}
}

void ABaseHero::CheckShadows()
{
	if (IsValid(lightDirectionPV) && IsValid(dMat))
	{
		FLinearColor matParam;
		if (lightDirectionPV->GetVectorParameterValue("DirectLight", matParam))
		{
			//Declare a hitresult to store the raycast hit in
			FHitResult hitResult;

			//Initialize the query params - ignore the actor
			FCollisionQueryParams CQP;
			//CQP.TraceTag = GetWorld()->DebugDrawTraceTag;
			CQP.AddIgnoredActor(this);

			lightVector = FVector(matParam);
			//GEngine->AddOnScreenDebugMessage(5, 1.0f, FColor::Black, lightVector.ToString());
			//make this bool alpha to see if we're in shadow

			bool inShadow = GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(), GetActorLocation() + lightVector * shadowRange, ECC_Visibility, CQP);
			//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + lightVector * shadowRange, FColor::Red);
			FMath::FInterpTo(matParam.A, inShadow, GetWorld()->GetDeltaSeconds(), 5);
			if (dMat->GetVectorParameterValue(FMaterialParameterInfo("InstDirectLight"), matParam))
			dMat->SetVectorParameterValue("InstDirectLight", FLinearColor(lightVector.X, lightVector.Y, lightVector.Z, inShadow));
			//lightDirectionPV->SetVectorParameterValue("DirectLight", FLinearColor(lightVector.X, lightVector.Y, lightVector.Z, inShadow))
		}
	}
}

