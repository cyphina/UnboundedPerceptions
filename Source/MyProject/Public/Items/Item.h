#pragma once
#include "GameplayTagsModule.h"
#include "GameplayTags.h"
#include "Item.generated.h"

class UMySpell;

UENUM(BlueprintType)
enum class ERarity : uint8 // rarity enum for all types of items
{
   Common = 0,
   Uncommon,
   Rare,
   Very_Rare,
   Super_Rare,
   Mega_Rare,
   Ultra_Rare,
   Legendary,
   /*Easy/Normal Cutoff*/
   Mythical,
   Fabled,
   /**Hard cutoff*/
   SemiGod_Tier,
   DemiGod_Tier,
   God_Tier,
   /**Expert cutoff*/
   Beyond_Godlike,
   /**Nightmare - Highest Level cutoff*/
   Unbounded,
   Artifact,
   Key_Item
};

USTRUCT(BlueprintType, NoExport)
struct FMyItemInfo
{
   FMyItemInfo() :
      name(FText::GetEmpty()), image(nullptr), description(FText::GetEmpty()), itemType(FGameplayTag()), isStackable(false), count(1), rarity(ERarity::Common)
   {
   }

   FMyItemInfo(FText name, UTexture2D* image, FText desc, FGameplayTag itemType, bool isStackable, int count, ERarity rarity) :
      image(image), itemType(itemType), isStackable(isStackable), count(count), rarity(rarity)
   {
      this->name        = FText::Format(NSLOCTEXT("Items", "ItemName", "{0}"), name);
      this->description = FText::Format(NSLOCTEXT("Items", "ItemDesc", "{0}"), desc);
   }

   FMyItemInfo& operator=(const FMyItemInfo& item)
   {
      name        = item.name.IsEmpty() ? FText::GetEmpty() : item.name;
      image       = item.image;
      description = item.description.IsEmpty() ? FText::GetEmpty() : item.description;
      itemType    = item.itemType;
      isStackable = item.isStackable;
      count       = item.count;
      rarity      = item.rarity;
      return *this;
   }

   /**Unique item ID*/
   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
   int id = -1;

   /**Every item name should be unique*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   FText name;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   UTexture2D* image;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   FText description;

   /**Tag with description of item application*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   FGameplayTag itemType;

   /**Can this item be stacked in an inventory slot?*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   bool isStackable;

   /**If it can be stacked, how much of this item is stacked in this slot*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int count;

   /**If rarity is KEY_ITEM, we should not be able to drop the item*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   ERarity rarity;
};

class AUnit;
class UEquip;
class UConsumable;
class ABaseHero;
class UWeapon;

/**
 * Each item struct has an id which can be used to lookup more information about the item in an item table (see UItemFunctionLibrary).
 * It also has a count which is used when storing items in a backpack which is a specialized data structure for item storage.
 */

USTRUCT(BlueprintType, NoExport)
struct FMyItem
{
   /**Item ID.  Valid ID's start at 1 since we can use this as a conditional if we let 0 be invalid*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Information")
   int id = 0;

   /**Number of items in the slot*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Information")
   int count = 1;

   FMyItem()
   {
   }

   explicit FMyItem(int id, int count = 1) :
      id(id), count(count)
   {
   }

   operator bool() const { return id > 0; }
};
