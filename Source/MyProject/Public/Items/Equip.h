// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MyProject.h"
#include "Item.h"
#include "Stats/BaseCharacter.h"
#include "Equip.generated.h"

/**
 * 
 */

UCLASS(BlueprintType)
class UEquip : public UMyItem
{
	GENERATED_BODY()

public:
	UEquip() : UMyItem(), bonuses(TArray<int>()), bonusValues(TArray<int>()), effects(TArray<FName>()), level(1) {}
	UEquip(FText name, UTexture2D* image, FText desc, bool isStackable, int count, ERarity rarity,
		TArray<int> bonuses, TArray<int> bonusValues, TArray<FName> effects, FName etype, int level) : 
		UMyItem(name, image, desc, UGameplayTagsManager::Get().RequestGameplayTag("Item.Equippable.Weapon"), isStackable, count, rarity), 
		bonuses(bonuses), bonusValues(bonusValues), effects(effects), level(level)
	{
	
	}

	~UEquip() {}

	//Sets up the names and values in description
	UFUNCTION(BlueprintCallable, Category = "CALL THIS TO INITIALIZE DATA")
		void InitParams()
	{
		Super::PostInitProperties();	
		//Appends new bonus to previous description including previous bonuses
		for (int i = 0; i < bonuses.Num(); i++)
		{
			FFormatOrderedArguments args;
			args.Add(itemInfo.description);
			args.Add(BonusToName(bonuses[i]));
			args.Add(bonusValues[i]);
			itemInfo.description = FText::Format(NSLOCTEXT("Items", "ItemDesc", "{0} \r\n+{2} {1}"), args); 
		}
	}

	TArray<int> GetBonuses() const { return bonuses; }  
	TArray<int> GetBonusValues() const { return bonusValues; } 
	TArray<FName> GetEffects() const { return effects; }
	int GetLevel() const{ return level; }

private:
	//indices of what bonuses to increase (0-AttCount, AttCount-SkillCount, SkillCount-VitCount, VitCount-MechanicCount)
	UPROPERTY(BlueprintReadWrite, Category = "Equipment", Meta = (ExposeOnSpawn = true, AllowPrivateAccess = true))
	TArray<int> bonuses; 

	//how much the bonus is increased by 0-6 attributes, 7-40 stats, 41-45 vitals, and rest are mechanics
	UPROPERTY(BlueprintReadWrite, Category = "Equipment", Meta = (ExposeOnSpawn = true, AllowPrivateAccess = true))
	TArray<int> bonusValues; 

    //special effects of the weapon to be considered
	UPROPERTY(BlueprintReadWrite, Category = "Equipment", Meta = (ExposeOnSpawn = true, AllowPrivateAccess = true))
	TArray<FName> effects; 

	//level of equipment
	UPROPERTY(BlueprintReadWrite, Category = "Equipment", Meta = (ExposeOnSpawn = true, AllowPrivateAccess = true))
	int level = 1;

	//Finds name of bonus based on index
	FText BonusToName(int bonusValue)
	{
		if (bonusValue > CombatInfo::AttCount + CombatInfo::StatCount + CombatInfo::VitalCount)
		{
			const UEnum* eBonus = FindObject<UEnum>(ANY_PACKAGE, TEXT("Mechanics"), true);
			if (!eBonus) return FText();
				return eBonus->GetDisplayNameTextByIndex(bonusValue);
		}
		else if(bonusValue > CombatInfo::AttCount + CombatInfo::StatCount)
		{
			const UEnum* eBonus = FindObject<UEnum>(ANY_PACKAGE, TEXT("Vitals"), true);
			if (!eBonus) return FText();
				return eBonus->GetDisplayNameTextByIndex(bonusValue);
		}
		else if (bonusValue > CombatInfo::AttCount)
		{
			const UEnum* eBonus = FindObject<UEnum>(ANY_PACKAGE, TEXT("Stats"), true);
			if (!eBonus) return FText();
				return eBonus->GetDisplayNameTextByIndex(bonusValue);
		}
		else
		{
			const UEnum* eBonus = FindObject<UEnum>(ANY_PACKAGE, TEXT("Attributes"), true);
			if (!eBonus) return FText();
				return eBonus->GetDisplayNameTextByIndex(bonusValue);
		}
	}
};


