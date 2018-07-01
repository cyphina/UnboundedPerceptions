// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MyProject.h"
#include "Item.h"
#include "Stats/BaseCharacter.h"
#include "Equip.generated.h"

/**
 * Equippable objects are/will be stored in an equipment class.  They give us stat bonuses, and eventually may impact our auto attack or something. 
 */

UENUM(BlueprintType)
enum class EStatList : uint8
{
	// Skill Damage
	Strength = 0, 
	// Elemental Affinity, Elemental Resistance, PPC Shield
	Understanding, 
	// Spell Damage
	Intelligence, 
	// Physical Affinity, Dodge Ratio, AttackSpeed
	Explosiveness, 
	// Physical Resistance, Hitpoints
	Endurance, 
	// Skill Damage 
	Agility, 
	// Hit Ratio, Critical Ratio, Critical Strike Damage
	Luck, 

	Critical_Chance,
	Critical_Damage,
	Accuracy,
	Dodge,
	Attack_Speed,
	Cast_Speed,

	Physical_Aff,
	Fire_Aff,
	Water_Aff,
	Wind_Aff,
	Earth_Aff,
	Electric_Aff,
	Darkness_Aff,
	Light_Aff,
	Arcane_Aff,
	Chaos_Aff,
	Poison_Aff,
	Blood_Aff,
	Ethereal_Aff,

	Physical_Resist,
	Fire_Resist,
	Water_Resist,
	Wind_Resist,
	Earth_Resist,
	Electric_Resist,
	Darkness_Resist,
	Light_Resist,
	Arcane_Resist,
	Chaos_Resist,
	Poison_Resist,
	Blood_Resist,
	Ethereal_Resist,

	Health,
	Mana,
	Psyche,
	Moxie, //only increasable by items, normal cap of 100
	Shield, //only increasable by items and buffs

	MovementSpeed, //max walking speed in ue4 is based off of centimeters per second.  Average persom walks 5km in an hour, which is around 138 cm/s which is how max speed is measured
	AttackRange,
	WeaponPower,
	GlobalDamageModifier //damage reduction modifier, 0 = no damage reduction, 100 = no damage recieved
};

USTRUCT(Blueprintable)
struct FEquipLookupRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/**What bonuses are increased: 0-6 attributes, 7-38 stats, 39-43 vitals, and rest are mechanics*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EStatList>		bonuses; 

	/**How much the bonus is increased by*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int>				bonusValues; 

    /**Special effects of the weapon to be considered*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName>			effects; 

	/**Level of equipment*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int						level = 1;
};


