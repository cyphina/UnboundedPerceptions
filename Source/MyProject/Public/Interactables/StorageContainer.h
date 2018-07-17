// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/InteractableBase.h"
#include "Items/Item.h"
#include "StorageContainer.generated.h"

/**
 * Storage container for treasureboxes which can hold items to be looted and be used for item storage
 */

class AUserInput;
class UBackpack;

UCLASS(HideCategories=(Input, Actor))
class MYPROJECT_API AStorageContainer : public AInteractableBase
{
	GENERATED_BODY()

	AUserInput*						controllerRef = nullptr;

	UPROPERTY()
	UBackpack*						backpack = nullptr;

public:

	AStorageContainer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMyItem>					initialItems;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* 				scene;

	/*Mesh of the container*/
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* 			interactableMesh;

	/*Sphere collision to determine if the hero has walked out of range othe storage container*/
	UPROPERTY(VisibleAnywhere)
	USphereComponent* 				sphereCollision;


	void							BeginPlay() override;

	void							Interact_Implementation(ABaseHero* hero) override;
	bool							CanInteract_Implementation() override;
	FVector							GetInteractableLocation_Implementation() override;

	UFUNCTION()
	void							OnLeaveRange(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int otherBodyIndex);
};
