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

UCLASS(HideCategories = (Input, Actor))
class MYPROJECT_API AStorageContainer : public AInteractableBase
{
   GENERATED_BODY()

   AUserInput* controllerRef = nullptr;

   UPROPERTY()
   UBackpack* backpack = nullptr;

   static class AHUDManager* hudManagerRef;

 public:
   AStorageContainer();

   static void SetHUDManagerRef(AHUDManager* newManager) { hudManagerRef = newManager; }

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   TArray<FMyItem> initialItems;

   /*Sphere collision to determine if the hero has walked out of range of the storage container*/
   UPROPERTY(VisibleAnywhere)
   USphereComponent* sphereCollision;

   void BeginPlay() override;

   void    Interact_Implementation(ABaseHero* hero) override;
   bool    CanInteract_Implementation() const override;
   FVector GetInteractableLocation_Implementation() const override;

   UFUNCTION()
   void OnLeaveRange(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int otherBodyIndex);

   void SaveInteractable(FMapSaveInfo& mapData) override final;
   void LoadInteractable(FMapSaveInfo& mapData) override final;
};
