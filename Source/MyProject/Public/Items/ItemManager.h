#pragma once
#include "Item.h"
#include "ItemManager.generated.h"

/**
 * Struct so we can store our item information in a table and reference item information by ID
 */
USTRUCT(Blueprintable)
struct FItemLookupRow : public FTableRowBase
{
   GENERATED_USTRUCT_BODY()

   /**Every item name should be unique*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   FText name = FText::GetEmpty();

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   TSoftObjectPtr<UTexture2D> image;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
   FText description = FText::GetEmpty();

   /**Tag with description of item application*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Item"))
   FGameplayTag itemType;

   /**Can this item be stacked in an inventory slot?*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   bool isStackable;

   /**If rarity is KEY_ITEM, we should not be able to drop the item*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   ERarity rarity;
};

class AUnit;
class UEquip;
class UConsumable;
class ABaseHero;
class UWeapon;

struct FEquipLookupRow;
struct FConsumableLookupRow;

UCLASS()
class MYPROJECT_API UItemManager : public UObject
{
   GENERATED_BODY()
 public:
   UItemManager();

   ~UItemManager() = default;

   FORCEINLINE static UItemManager& Get()
   {
      if(SingletonManager == nullptr)
      {
         InitializeManager();
      }
      return *SingletonManager;
   }

   /*UFUNCTION(BlueprintCallable, Category = "Item Data Accessor", meta = (DisplayName = "Get Item Info"))
   FORCEINLINE FItemLookupRow* GetItemInfo(int itemID);

   UFUNCTION(BlueprintCallable, Category = "Item Data Accessor", meta = (DisplayName = "Get Equip Info"))
   FORCEINLINE FEquipLookupRow* GetEquipInfo(int equipID);

   UFUNCTION(BlueprintCallable, Category = "Item Data Accessor", meta = (DisplayName = "Get Consumable Info"))
   FORCEINLINE FConsumableLookupRow* GetConsumableInfo(int consumableID);*/

   FItemLookupRow*       GetItemInfo(int itemID) const;
   FEquipLookupRow*      GetEquipInfo(int equipID) const;
   FConsumableLookupRow* GetConsumableInfo(int consumableID) const;

   FItemLookupRow*       GetItemInfo(FName itemID) const;
   FEquipLookupRow*      GetEquipInfo(FName equipID) const;
   FConsumableLookupRow* GetConsumableInfo(FName consumableID) const;
   TArray<FName>         GetAllConsumableIDs() const;

   static UDataTable* GetItemLookupTable() { return itemLookupTable; }

 private:
   static UItemManager* SingletonManager; // Our single spellmanager

   inline static UDataTable* itemLookupTable = nullptr; // Data table with the basic item information

   UPROPERTY()
   UDataTable* equipLookupTable; // Data table with equipment information

   UPROPERTY()
   UDataTable* consumableLookupTable; // Data table with consumable information

   static void InitializeManager(); // Initializes ItemManager if none exists
};
