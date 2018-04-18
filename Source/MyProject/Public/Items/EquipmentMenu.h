#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyDraggableWidget.h"
#include "EquipmentMenu.generated.h"

/**
 * Menu for equipment
 */

class UEquip;
class UWeapon;
class ABaseHero;

UCLASS()
class MYPROJECT_API UEquipmentMenu : public UMyDraggableWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, Category = "References")
	ABaseHero*		hero;

	bool			OnWidgetAddToViewport_Implementation() override;

	//virtual void OnDragDetected_Implementation(FGeometry MyGeometry, const FPointerEvent& PointerEvent, UDragDropOperation*& Operation) { Super::OnDragDetected_Implementation(MyGeometry, PointerEvent, Operation); }
	//virtual FEventReply OnMouseButtonDown_Implementation(FGeometry MyGeometry, const FPointerEvent& MouseEvent) { return Super::OnMouseButtonDown_Implementation(MyGeometry, MouseEvent); } //Setup offset and detect drag

	void			Equip(UEquip* equip);

	//Unequip an item
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void			Unequip(int slotToUnequip);

	//Call to redraw what is in equipment
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Equipment")
	void			Update();
	void			Update_Implementation();

	void			SwapWeps(UWeapon* weapon);


	
};
