// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Item.h"
#include "Consumable.generated.h"

/**
 * Potion item that can either heal or buff.
 * Bonuses can only increase buffvalue
 */

class UMySpell;

UCLASS()
class MYPROJECT_API UConsumable : public UMyItem
{
	GENERATED_BODY()

public:
	UConsumable();
	~UConsumable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		TSubclassOf<UMySpell> ability;

	int GetPotency() const { return potency; }
	int GetDoses() const { return doses; }

private:
	int potency = 1; //multiplier of this consuming items effect
	int doses = 1; //how many times can we use this potion
};
