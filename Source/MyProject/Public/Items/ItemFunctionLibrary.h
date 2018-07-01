#pragma once

#include "ItemManager.h"
#include "Equip.h"
#include "Consumable.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemFunctionLibrary.generated.h"

class UGameplayAbility;
class UGameplayEffect;

//A class that holds a blueprint library, that is it can be used anywhere inside blueprints so we can get our spellInfo via the manager in our blueprints
UCLASS(MinimalAPI)
class UItemFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item Data Accessor", meta = (DisplayName = "Get Item Info"))
		static FItemLookupRow&					GetItemInfo(int itemID);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item Data Accessor", meta = (DisplayName = "Get Equip Info"))
		static FEquipLookupRow&					GetEquipInfo(int equipID);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item Data Accessor", meta = (DisplayName = "Get Consumable Info"))
		static FConsumableLookupRow&			GetConsumableInfo(int consumableID);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DescriptionHeler")
		static FText							GetBonusDescription(int equipID);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DescriptionHeler")
		static FText							BonusToName(int bonusValue);
};