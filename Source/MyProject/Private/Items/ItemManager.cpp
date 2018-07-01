#include "MyProject.h"
#include "ItemManager.h"
#include "Equip.h"
#include "Consumable.h"

UItemManager* UItemManager::SingletonManager = nullptr;

UItemManager::UItemManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemLookupTableFinder(TEXT("/Game/RTS_Tutorial/Tables/ItemList"));
	if (ItemLookupTableFinder.Object)
		itemLookupTable = ItemLookupTableFinder.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> EquipLookupTableFinder(TEXT("/Game/RTS_Tutorial/Tables/EquipList"));
	if (EquipLookupTableFinder.Object)
		equipLookupTable = EquipLookupTableFinder.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> ConsumableLookupTableFinder(TEXT("/Game/RTS_Tutorial/Tables/ConsumableList"));
	if (ConsumableLookupTableFinder.Object)
		consumableLookupTable = ConsumableLookupTableFinder.Object;
}

FItemLookupRow* UItemManager::GetItemInfo(int itemID)
{
	return reinterpret_cast<FItemLookupRow*>(itemLookupTable->FindRowUnchecked(*FString::FromInt(itemID)));
}

FEquipLookupRow* UItemManager::GetEquipInfo(int equipID)
{
	return reinterpret_cast<FEquipLookupRow*>(equipLookupTable->FindRowUnchecked(*FString::FromInt(equipID)));
}

FConsumableLookupRow* UItemManager::GetConsumableInfo(int consumableID)
{
	return reinterpret_cast<FConsumableLookupRow*>(consumableLookupTable->FindRowUnchecked(*FString::FromInt(consumableID)));
}

void UItemManager::InitializeManager()
{
	check(!SingletonManager);
	SingletonManager = NewObject<UItemManager>(GetTransientPackage(), NAME_None);
	SingletonManager->AddToRoot();
}