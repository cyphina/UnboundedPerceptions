// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equip.h"
#include "Weapon.generated.h"

/**
 * Weapons are added to the codex.  Up to 4 may be added
 */

UENUM(BlueprintType)
enum class WeaponTypes : uint8 //rarity enum for all types of items
{
	Axe = 0,
	Bow,
	Dual_Dagger,
	Fist,
	Greatsword,
	Gun,
	Hammer,
	Katana,
	Knife,
	Lance,
	Scythe,
	Shield,
	Sword,
	Staff,
	Tome,
	Wand,
	Whip,
};

UCLASS(BlueprintType)
class UWeapon : public UEquip
{
public:
	GENERATED_BODY()

	UWeapon();
	UWeapon(int weaponP, int weaponE, int BAT, bool isDW) : weaponPower(weaponP), weaponEfficiency(weaponE), baseAttackTime(BAT), isDualWield(isDW) { SetupBaseValues();  }
	~UWeapon();

	UPROPERTY(BlueprintReadWrite, Category = "Weapon", Meta = (ExposeOnSpawn = true))
	int weaponPower = 10; //damage potential of this weapon
	UPROPERTY(BlueprintReadWrite, Category = "Weapon", Meta = (ExposeOnSpawn = true))
	int weaponEfficiency = 100; //range of how damage fluctuates
	UPROPERTY(BlueprintReadWrite, Category = "Weapon", Meta = (ExposeOnSpawn = true))
	float baseAttackTime = 2; 
	UPROPERTY(BlueprintReadWrite, Category = "Weapon", Meta = (ExposeOnSpawn = true))
	bool isDualWield = false; //can we have an offhand?

	void SetupBaseValues(); //adds attack mod on weapon based on weapon type
};
