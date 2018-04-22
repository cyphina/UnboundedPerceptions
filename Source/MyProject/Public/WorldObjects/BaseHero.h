// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ally.h"
#include "Items/Backpack.h"
#include "BaseHero.generated.h"

class ABasePlayer;
class Inventory;
class IInteractable;
class UGameplayAbility;
class USpellBook;
class UConsumable;

UCLASS()
class MYPROJECT_API ABaseHero : public AAlly
{
	GENERATED_BODY()
	
public: 
	/*--CONSTANTS--*/
	static const int			interactRange = 200; //range of how close we have to be to an object to interact with it. 
	static const int			shadowRange = 600; //range of how far we cast in front to see if shadows are blocking us along the direction of the directional light parameter set in our shader
	static const float			nextExpMultiplier; //multiplier to increase amount of xp needed for next level

	// Sets default values for this character's properties
	ABaseHero(const FObjectInitializer& oI);

#pragma region Callbacks
	// Called when the game starts or when spawned
	void						BeginPlay() override;

	// Called every frame
	void						Tick(float deltaSeconds) override;

	//When hero gets destroyed
	void						Destroyed() override;

	//When game ends
	void						EndPlay(const EEndPlayReason::Type epr) override;
#pragma endregion

#pragma region HeroCharacterStuff
	//Use an item on something
	UFUNCTION(BlueprintCallable, Category = "Items")
	void						BeginUseItem(UConsumable* itemToUse);
	
	/*Interacts with an object, provided there is one*/
	UFUNCTION(BlueprintCallable, Category = "Misc")
	void						BeginInteract(AActor* interactor);

	//Levelup functionality mostly in blueprints so we can add some effects
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stats") //BlueprintNativeEvent allows you to override in blueprints
	void						LevelUp();

	//Interface to add points to some category
	UFUNCTION(BlueprintCallable)
	void 						ChangeAttribute(Attributes att, bool isIncrementing);

	///---Equipment---
	UFUNCTION(BlueprintCallable, Category = "Equipment") //When we click on an item to use it and it's a piece of equipment
	void 						EquipItem(UEquip* e);
	//When we click on an item and it's a piece of equipment and we already have a piece of equipment on that slot, swap them
	void 						UnequipItem(int slot);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void 						SwapEquips(UEquip* e1, int equipSlot);	//swap equip to whatever is in the equipslot

	//---Actions---
	void 						Stop() override;
#pragma endregion

#pragma region Accessors

	UFUNCTION(BlueprintCallable, Category = "Accessors")
	void 						SetSelected(bool value) override;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Stats")
	int 						GetCurrentExp() const;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Stats")
	int 						GetExpToLevel() const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void 						SetCurrentExp(int amount);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Equips")
	TArray<UEquip*>				GetEquips() const { return equips; }

	//Get the item set to be used currently by this hero
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Items")
	UConsumable*				GetCurrentItem() const { return currentItem; }

	//Set the item set to be used currently by this hero
	UFUNCTION(BlueprintCallable, Category = "Items")
	void						SetCurrentItem(UConsumable* item) { currentItem = item; }

	//Get the interactable this hero is targetted to interact with
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Interactable")
	UObject*					GetCurrentInteractable() const;

	//Set the interactable this hero is targetted to interact with
	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void						SetCurrentInteractable(AActor* interactable);

	//Get a copy of our spellbook
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Spells")
	USpellBook*					GetSpellBook() const { return spellbook; }

#pragma endregion

	//default material color
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor				baseColor;

	//different color
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor				currentColor;

	//dynamic material instance reference used to change material's color
	UMaterialInstanceDynamic*	dMat;

	//Index inside party.  -1 means we're not in the party
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int							heroIndex = -1;

	//attribute points to divy up
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int							attPoints = 100; 

	//attribute points to divy up
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int							skillPoints = 5;

	//Our character's backpack
	UPROPERTY(BlueprintReadWrite)
	UBackpack*					backpack;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USpellBook*					spellbook;

private:
	//see if some object is in front of us so we know its casting a shadow upon us
	void						CheckShadows();
	//Using some kind of item, consumeable, utility, etc.  We need to target some target for this item
	void						UseItem();
	//Here's where we do all the checks and repositioning for interacting.  Continuously called as long as we have an interaction target
	void						PrepareInteract();	
	//functions for adding and removing bonuses when equipping and unequipping.  Set isEquip to true when equipping, and to false when unequipping
	void						SetupBonuses(UEquip* e, bool isEquip);

	//--References--//
	ABasePlayer*				player; //reference to our player class, which has information on our team
	IInteractable*				currentInteractable; //reference to the interactable which we are trying to interact with						
	UConsumable*				currentItem = nullptr; //the item that is going to be used by this character
	
	//--Lighting effect--//
	UMaterialParameterCollection*				lightSource = nullptr; //We need this parameter to figure out light direction based on our directional light we made
	UMaterialParameterCollectionInstance*		lightDirectionPV = nullptr;
	FVector lightVector =						FVector::ZeroVector; 	//A reference we use in calculating if there's shadows
	//--Leveling information---//
	int											currentExp = 0; //how much xp we have 
	int											expForLevel = 100; //how much we need for next level

	//--equipment information--//
	//0 - Head, 1 - Body, 2 - Legs, 3 - Acc1, 4 - Codex, 5-9 - Codex Weapons
	TArray<UEquip*>								equips; //a container of what we have equipped
	friend void									USaveLoadClass::SetupAlliedUnits();

	friend void									InteractState::Update(AUnit& unit, float deltaSeconds);
};

