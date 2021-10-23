#include "MyProject.h"
#include "ItemFunctionLibrary.h"

#include "Backpack.h"
#include "Stats/BaseCharacter.h"

FItemLookupRow& UItemFunctionLibrary::GetItemInfo(int itemID)
{
   return *UItemManager::Get().GetItemInfo(itemID);
}

FItemLookupRow& UItemFunctionLibrary::GetItemInfoFromBackpack(UBackpack* backpack, int itemSlotIndex)
{
   return GetItemInfo(backpack->GetItem(itemSlotIndex).id);
}

FEquipLookupRow& UItemFunctionLibrary::GetEquipInfo(int equipID)
{
   return *UItemManager::Get().GetEquipInfo(equipID);
}

FConsumableLookupRow& UItemFunctionLibrary::GetConsumableInfo(int consumableID)
{
   return *UItemManager::Get().GetConsumableInfo(consumableID);
}

void UItemFunctionLibrary::DescriptionBuilder(FString& bonusDescription, uint8 stat, int statVal)
{
   TArray<FStringFormatArg> args;
   args.Add(bonusDescription);
   args.Add(BonusToName(stat).ToString());
   args.Add(statVal);
   bonusDescription = FString::Format(TEXT("{0}+{2} {1} \r\n"), args);
}

FText UItemFunctionLibrary::GetBonusDescription(int equipID)
{
   FEquipLookupRow equipInfo = GetEquipInfo(equipID);

   FString   bonusDescription = "";
   const int numAtts          = CombatInfo::AttCount;
   const int numScalingStats  = CombatInfo::StatCount;
   const int numVitals        = CombatInfo::VitalCount;

   for(auto& x : equipInfo.stats.defaultAttributes)
   {
      DescriptionBuilder(bonusDescription, static_cast<uint8>(x.att), x.defaultValue);
   }
   for(auto& x : equipInfo.stats.defaultUnitScalingStats)
   {
      DescriptionBuilder(bonusDescription, static_cast<uint8>(x.stat) + numAtts, x.defaultValue);
   }
   for(auto& x : equipInfo.stats.defaultVitals)
   {
      DescriptionBuilder(bonusDescription, static_cast<uint8>(x.vit) + numAtts + numScalingStats, x.defaultValue);
   }
   for(auto& x : equipInfo.stats.defaultMechanics)
   {
      DescriptionBuilder(bonusDescription, static_cast<uint8>(x.mech) + numAtts + numScalingStats + numVitals, x.defaultValue);
   }

   bonusDescription.RemoveFromEnd("\r\n");
   return FText::FromString(bonusDescription);
}

FText UItemFunctionLibrary::GetRarityText(ERarity rarity)
{
   const UEnum* eRarity = FindObject<UEnum>(ANY_PACKAGE, TEXT("ERarity"), true);
   if(!eRarity) return FText::GetEmpty();
   return eRarity->GetDisplayNameTextByValue(static_cast<int>(rarity));
}

// Finds name of bonus based on index
FText UItemFunctionLibrary::BonusToName(int bonusValue)
{
   if(bonusValue >= CombatInfo::AttCount + CombatInfo::StatCount + CombatInfo::VitalCount)
   {
      const UEnum* eBonus = FindObject<UEnum>(ANY_PACKAGE, TEXT("EMechanics"), true);
      if(!eBonus) return FText();
      return eBonus->GetDisplayNameTextByIndex(bonusValue - (CombatInfo::AttCount + CombatInfo::StatCount + CombatInfo::VitalCount));
   }
   else if(bonusValue >= CombatInfo::AttCount + CombatInfo::StatCount)
   {
      const UEnum* eBonus = FindObject<UEnum>(ANY_PACKAGE, TEXT("EVitals"), true);
      if(!eBonus) return FText();
      return eBonus->GetDisplayNameTextByIndex(bonusValue - (CombatInfo::AttCount + CombatInfo::StatCount));
   }
   else if(bonusValue >= CombatInfo::AttCount)
   {
      const UEnum* eBonus = FindObject<UEnum>(ANY_PACKAGE, TEXT("EUnitScalingStats"), true);
      if(!eBonus) return FText();
      return eBonus->GetDisplayNameTextByIndex(bonusValue - CombatInfo::AttCount);
   }
   else
   {
      const UEnum* eBonus = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAttributes"), true);
      if(!eBonus) return FText();
      return eBonus->GetDisplayNameTextByIndex(bonusValue);
   }
}
