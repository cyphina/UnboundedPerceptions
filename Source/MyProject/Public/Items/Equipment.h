// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon.h"
#include "Equipment.generated.h"

/**Actor component for holding equips*/

class ABaseHero;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYPROJECT_API UEquipment : public UActorComponent
{
	GENERATED_BODY()
		
	//0 - Head, 1 - Body, 2 - Legs, 3 - Acc1, 4 - Codex, 5-9 - Codex Weapons
	TArray<UEquip*> equips; //a container of what we have equipped
	ABaseHero* hero;

public:	
	// Sets default values for this component's properties
	UEquipment();
	void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Interfacing Equipment")
		void OnEquip();
	
};
