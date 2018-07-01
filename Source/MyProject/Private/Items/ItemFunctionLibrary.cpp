#include "MyProject.h"
#include "ItemFunctionLibrary.h"
#include "Stats/BaseCharacter.h"

FItemLookupRow& UItemFunctionLibrary::GetItemInfo(int itemID)
{
	return *UItemManager::Get().GetItemInfo(itemID);
}

FEquipLookupRow& UItemFunctionLibrary::GetEquipInfo(int equipID)
{
	return *UItemManager::Get().GetEquipInfo(equipID);
}

FConsumableLookupRow& UItemFunctionLibrary::GetConsumableInfo(int consumableID)
{
	return *UItemManager::Get().GetConsumableInfo(consumableID);
}

FText UItemFunctionLibrary::GetBonusDescription(int equipID)
{
	FEquipLookupRow equipInfo = GetEquipInfo(equipID);

	FText bonusDescription = FText::GetEmpty();
	for (int i = 0; i < equipInfo.bonuses.Num() - 1; i++)
	{
		FFormatOrderedArguments args;
		args.Add(bonusDescription);
		args.Add(BonusToName(static_cast<uint8>(equipInfo.bonuses[i])));
		args.Add(equipInfo.bonusValues[i]);
		bonusDescription = FText::Format(NSLOCTEXT("Items", "BonusDesc", "{0} +{2} {1} \r\n"), args);
	}

	//Last line doesn't have a line break.  Also make sure bonuses have same num as bonusValues
#if UE_EDITOR
	checkf(equipInfo.bonuses.Num() == equipInfo.bonusValues.Num(), TEXT("Item doesn't have same number of bonuses and bonusvalues"));
#endif
	FFormatOrderedArguments args;
	args.Add(bonusDescription);
	args.Add(BonusToName(static_cast<uint8>(equipInfo.bonuses[equipInfo.bonuses.Num() - 1])));
	args.Add(equipInfo.bonusValues[equipInfo.bonusValues.Num() - 1]);
	bonusDescription = FText::Format(NSLOCTEXT("Items", "BonusDesc", "{0} +{2} {1}"), args);

	return bonusDescription;
}

//Finds name of bonus based on index
FText UItemFunctionLibrary::BonusToName(int bonusValue)
{
	if (bonusValue >= CombatInfo::AttCount + CombatInfo::StatCount + CombatInfo::VitalCount)
	{
		const UEnum* eBonus = FindObject<UEnum>(ANY_PACKAGE, TEXT("Mechanics"), true);
		if (!eBonus) return FText();
		return eBonus->GetDisplayNameTextByIndex(bonusValue - (CombatInfo::AttCount + CombatInfo::StatCount + CombatInfo::VitalCount));
	}
	else if (bonusValue >= CombatInfo::AttCount + CombatInfo::StatCount)
	{
		const UEnum* eBonus = FindObject<UEnum>(ANY_PACKAGE, TEXT("Vitals"), true);
		if (!eBonus) return FText();
		return eBonus->GetDisplayNameTextByIndex(bonusValue - (CombatInfo::AttCount + CombatInfo::StatCount));
	}
	else if (bonusValue >= CombatInfo::AttCount)
	{
		const UEnum* eBonus = FindObject<UEnum>(ANY_PACKAGE, TEXT("UnitStats"), true);
		if (!eBonus) return FText();
		return eBonus->GetDisplayNameTextByIndex(bonusValue - CombatInfo::AttCount);
	}
	else
	{
		const UEnum* eBonus = FindObject<UEnum>(ANY_PACKAGE, TEXT("Attributes"), true);
		if (!eBonus) return FText();
		return eBonus->GetDisplayNameTextByIndex(bonusValue);
	}
}