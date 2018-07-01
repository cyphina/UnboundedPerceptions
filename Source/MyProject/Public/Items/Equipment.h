#pragma once
#include "CoreMinimal.h"
#include "Weapon.h"
#include "Equipment.generated.h"

/**UObject container for holding equips*/

class ABaseHero;

DECLARE_DELEGATE_TwoParams(FOnEquipped, int, bool);

UCLASS(ClassGroup=(Custom))
class MYPROJECT_API UEquipment : public UObject
{
	GENERATED_BODY()
		
	//0 - Head, 1 - Body, 2 - Legs, 3 - Acc1, 4 - Codex, 5-9 - Codex Weapons
	
	TArray<int>								equips;
	ABaseHero* heroRef;

public:	
	// Sets default values for this component's properties
	UEquipment();

	/**Equip some piece of equipment (itemType of the item is equippable)*/
	UFUNCTION(BlueprintCallable, Category = "Interfacing Equipment")
	void									Equip(int equipItem);
	
	/**Unequip an item to the backpack, granted there's space*/
	UFUNCTION(BlueprintCallable, Category = "Interfacing Equipment")
	void									UnequipItem(int slot);

	UFUNCTION(BlueprintCallable, Category = "Interfacing Equipment")
	FORCEINLINE TArray<int>					GetEquips() const { return equips; }

	UFUNCTION(BlueprintCallable, Category = "Interfacing Equipment")
	void									SwapEquips(int equipSlot1, int equipSlot2);

	/**Move an item from the inventory to the equipped slot*/
	UFUNCTION(BlueprintCallable, Category = "Interfacing Equipment")
	void									SwapEquipsFromInventory(int equipID, int equipSlot);

	FOnEquipped								OnEquipped;

private:


};
