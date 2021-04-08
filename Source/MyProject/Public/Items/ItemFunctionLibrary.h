#pragma once

#include "ItemManager.h"
#include "Equip.h"
#include "Consumable.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemFunctionLibrary.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UBackpack;

// A class that holds a blueprint library, that is it can be used anywhere inside blueprints so we can get our spellInfo via the manager in our blueprints
UCLASS(MinimalAPI)
class UItemFunctionLibrary : public UBlueprintFunctionLibrary
{
   GENERATED_BODY()

   static void DescriptionBuilder(FString& bonusDescription, uint8 stat, int statVal);

 public:
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item Data Accessor", meta = (DisplayName = "Get Item Info"))
   static FItemLookupRow& GetItemInfo(int itemID);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item Data Accessor", meta = (DisplayName = "Get Item Info From Backpack"))
   static FItemLookupRow& GetItemInfoFromBackpack(UBackpack* backpack, int itemSlotIndex);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item Data Accessor", meta = (DisplayName = "Get Equip Info"))
   static FEquipLookupRow& GetEquipInfo(int equipID);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item Data Accessor", meta = (DisplayName = "Get Consumable Info"))
   static FConsumableLookupRow& GetConsumableInfo(int consumableID);

   // Lists all the bonuses a piece of equipment gives us
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DescriptionHelper")
   static FText GetBonusDescription(int equipID);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DescriptionHelper")
   static FText GetRarityText(ERarity rarity);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DescriptionHelper")
   static FText BonusToName(int bonusValue);
};
