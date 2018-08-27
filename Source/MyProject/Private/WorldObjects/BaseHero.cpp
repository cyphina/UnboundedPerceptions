// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"

#include "RTSGameMode.h"
#include "UserInput.h"
#include "BasePlayer.h"

#include "UI/HUDManager.h"
#include "Items/HeroInventory.h"
#include "Items/EquipmentMenu.h"

#include "BaseHero.h"
#include "State/HeroStateMachine.h"

#include "Interactables/Interactable.h"

#include "Trigger.h"

#include "ItemManager.h"
#include "Items/Equip.h"
#include "Items/Equipment.h"
#include "Items/Consumable.h"

#include "AbilitySystemComponent.h"
#include "SpellSystem/MySpell.h"
#include "SpellSystem/Spellbook.h"

#include "Materials/MaterialInstanceDynamic.h"

const float ABaseHero::nextExpMultiplier = 1.5f;

//---Object Fundamentals---
#pragma region Fundamentals
ABaseHero::ABaseHero(const FObjectInitializer& oI) : AAlly(oI)
{	
	isEnemy = false;
	state = TUniquePtr<HeroStateMachine>(new HeroStateMachine(this));

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
	backpack = NewObject<UBackpack>(this, "Backpack");
	backpack->SetItemMax(40);
	//This is the size of the hero equipment array.  Could make this a class later 
	equipment = NewObject<UEquipment>(this);
	equipment->OnEquipped.BindUObject(this, &ABaseHero::OnEquipped);
	
#if UE_EDITOR
	for(int i = 0; i < 70; i++)
	{
		baseC->LevelUp();
	}
#endif

	//Load up any stored triggers that tried to activate on this object but it wasn't alive
	TArray<FTriggerData> savedTriggers;
	Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode())->GetTriggerManager()->GetTriggerRecords().MultiFind(*GetGameName().ToString(),savedTriggers);
	for(FTriggerData& trigger : savedTriggers)
	{
		controllerRef->GetGameMode()->GetTriggerManager()->ActivateTrigger(trigger);
	}

	//TODO: Add all the item abilities to the abilitycomponent
	for(FName id : UItemManager::Get().GetAllConsumableIDs())
	{
		TSubclassOf<UMySpell> itemAbilityClass = UItemManager::Get().GetConsumableInfo(id)->abilityClass;
		if(IsValid(itemAbilityClass))
			GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(itemAbilityClass.GetDefaultObject(),1));
	}

	spellbook = NewObject<USpellBook>(this, spellbookClass.Get());
	spellbook->heroRef = this;
	spellbook->Init();

	SetEnabled(false);
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

AActor* ABaseHero::GetCurrentInteractable() const
{
	return Cast<AActor>(currentInteractable);
}

TArray<int> ABaseHero::GetEquipment() const
{
	return equipment->GetEquips();
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
	if(interactor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		state->ChangeState(EUnitState::STATE_INTERACTING);
		currentInteractable = interactor;
	}
}

void ABaseHero::Equip(int equipSlot)
{
	int prevEquipInSlot = equipment->Equip(backpack->GetItem(equipSlot).id);
	backpack->RemoveItemAtSlot(equipSlot);

	if (prevEquipInSlot)
		backpack->AddItem(FMyItem(prevEquipInSlot));

	if(controllerRef->GetHUDManager()->IsWidgetOnScreen(HUDs::HS_Equipment))
		controllerRef->GetHUDManager()->GetEquipHUD()->Update();
	if(controllerRef->GetHUDManager()->IsWidgetOnScreen(HUDs::HS_Inventory))
		controllerRef->GetHUDManager()->GetInventoryHUD()->LoadItems();
}

void ABaseHero::Unequip(int unequipSlot)
{
	if(backpack->Count() < backpack->GetItemMax())
	{
		int itemID = equipment->GetEquips()[unequipSlot];
		backpack->AddItem(FMyItem(itemID));
		equipment->Unequip(unequipSlot);
	}

	if(controllerRef->GetHUDManager()->IsWidgetOnScreen(HUDs::HS_Equipment))
		controllerRef->GetHUDManager()->GetEquipHUD()->Update();
	if(controllerRef->GetHUDManager()->IsWidgetOnScreen(HUDs::HS_Inventory))
		controllerRef->GetHUDManager()->GetInventoryHUD()->LoadItems();
}

void ABaseHero::SwapEquip(int equipSlot1, int equipSlot2)
{
	equipment->SwapEquips(equipSlot1, equipSlot2);
	if(controllerRef->GetHUDManager()->IsWidgetOnScreen(HUDs::HS_Equipment))
		controllerRef->GetHUDManager()->GetEquipHUD()->Update();
}

void ABaseHero::BeginUseItem(int itemToUseID)
{
	SetCurrentItem(itemToUseID);
	state->ChangeState(EUnitState::STATE_ITEM);
	TSubclassOf<UMySpell> itemAbility = UItemManager::Get().GetConsumableInfo(itemToUseID)->abilityClass;
	PressedCastSpell(itemAbility);
	if (itemAbility.GetDefaultObject()->GetTargetting() != FGameplayTag::RequestGameplayTag("Skill.Targetting.None"))
	{
		controllerRef->SetSecondaryCursor(ECursorStateEnum::Item);
	}
}

void ABaseHero::PrepareInteract()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, FString("Interacting ") + GetGameName().ToString());
	if (!IsStunned() && currentInteractable)
	{
		FVector targetLoc = IInteractable::Execute_GetInteractableLocation(currentInteractable);
		if (AdjustPosition(interactRange, targetLoc))
		{
			IInteractable::Execute_Interact(currentInteractable, this);
			Stop();
		}
	}
}

void ABaseHero::UseItem(int itemID)
{
	//TODO: removes first instance for now
	if(UItemManager::Get().GetItemInfo(itemID)->itemType.GetTagName() != "Item.Consumeable.Utility.Reusable")
		backpack->RemoveItem(FMyItem(itemID));

	if(controllerRef->GetHUDManager()->IsWidgetOnScreen(HUDs::HS_Inventory))
		controllerRef->GetHUDManager()->GetInventoryHUD()->LoadItems();
}
#pragma endregion

#pragma region equipment

void ABaseHero::OnEquipped(int equipID, bool isEquip)
{
	FEquipLookupRow* e = UItemManager::Get().GetEquipInfo(equipID);
	for(int i = 0; i < e->bonuses.Num(); ++i)
	{
		ApplyBonuses(static_cast<uint8>(e->bonuses[i]), e->bonusValues[i] * (2 * isEquip - 1));
	}
}

void ABaseHero::Stop()
{
	Super::Stop();
	currentInteractable = nullptr;
	currentItem = 0;
}

bool ABaseHero::CastSpell(TSubclassOf<UMySpell> spellToCast)
{
	int itemID = currentItem;
	bool sucessfulCast = Super::CastSpell(spellToCast);
	if(itemID && sucessfulCast)
		UseItem(itemID);
	return sucessfulCast;
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

