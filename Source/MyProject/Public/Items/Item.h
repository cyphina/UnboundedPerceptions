// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MyProject.h"
#include "GameplayTagsModule.h"
#include "GameplayTags.h"
#include "WorldDataObject.h"
#include "Item.generated.h"

/**
 * When using structs, make any member as a UPROPERTY to protect from dangling pointing crashes.  
 * Clear pointers to UObjects for GC to collect them
 */

class UMySpell;

UENUM(BlueprintType)
enum class ERarity : uint8 //rarity enum for all types of items
{
	Common = 0,
	Uncommon,
	Rare,
	Very_Rare,
	Super_Rare,
	Mega_Rare,
	Ultra_Rare,
	Legendary,
	Mythical, /*Easy/Normal*/
	Fabled,
	SemiGod_Tier, //Hard
	DemiGod_Tier,
	God_Tier,
	Beyond_Godlike, //Expert
	Unbounded, //Nightmare - Highest Level
	Artifact,
	Key_Item
};


	USTRUCT(BlueprintType, NoExport)
		struct FMyItemInfo
	{
		FMyItemInfo() : name(FText::GetEmpty()), image(nullptr), description(FText::GetEmpty()), itemType(FGameplayTag()), isStackable(false), count(1), rarity(ERarity::Common) {}

		FMyItemInfo(FText name, UTexture2D* image, FText desc, FGameplayTag itemType, bool isStackable, int count, ERarity rarity) : image(image), itemType(itemType), isStackable(isStackable), count(count), rarity(rarity)
		{
			this->name = FText::Format(NSLOCTEXT("Items", "ItemName", "{0}"), name);
			this->description = FText::Format(NSLOCTEXT("Items", "ItemDesc", "{0}"), desc);
		}

		FMyItemInfo& operator=(const FMyItemInfo& item)
		{
			name = item.name.IsEmpty() ? FText::GetEmpty() : item.name;
			image = item.image;
			description = item.description.IsEmpty() ? FText::GetEmpty() : item.description;
			itemType = item.itemType;
			isStackable = item.isStackable;
			count = item.count;
			rarity = item.rarity;
			return *this;
		}

		//Every item name should be unique
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			FText name;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			UTexture2D* image;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			FText description;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			FGameplayTag itemType;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			bool isStackable;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			int count;

		//If rarity is KEY_ITEM, we should not be able to drop the item
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			ERarity rarity;
	};

	class AUnit;
	class UEquip;
	class UConsumable;
	class ABaseHero;
	class UWeapon;

	UCLASS(Blueprintable)
		class MYPROJECT_API UMyItem : public UObject
	{
		GENERATED_BODY()
	public:

		UPROPERTY(BlueprintReadWrite, Category = "Item Information", Meta = (ExposeOnSpawn = true))
		FMyItemInfo itemInfo;
		UMyItem() {}
		UMyItem(FText name, UTexture2D* image, FText desc, FGameplayTag itemType, bool isStackable, int count, ERarity rarity) : itemInfo(name, image, desc, itemType, isStackable, count, rarity) {}
		~UMyItem() {}

		void ItemEffect(AUnit* target, UMyItem* item); //generic

	private:
		//Activate effect of item	
		void ItemEffect(AUnit* target, UConsumable* item);
		void ItemEffect(ABaseHero* hero, UEquip* equip); //if we're dealing with equipment
		void ItemEffect(ABaseHero* hero, UWeapon* weapon); //if we're dealing with equipment
	};
