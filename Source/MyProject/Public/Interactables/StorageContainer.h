// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/InteractableBase.h"
#include "Items/Item.h"
#include "StorageContainer.generated.h"

class IWidgetToggler;
class IHUDProvider;
class AUserInput;
class UBackpack;

/**
 * Storage container for treasure boxes which can hold items to be looted and be used for item storage
 */
UCLASS(HideCategories = (Input, Actor))
class MYPROJECT_API AStorageContainer : public AInteractableBase
{
   GENERATED_BODY()

 public:
   AStorageContainer();

   void    Interact_Implementation(ABaseHero* Hero) override;
   bool    CanInteract_Implementation() const override;
   FVector GetInteractableLocation_Implementation() const override;
   UBackpack* GetBackpack() const { return backpack; }
   
 protected:
   void BeginPlay() override;

   void SaveInteractable(FMapSaveInfo& mapData) override final;
   void LoadInteractable(FMapSaveInfo& mapData) override final;

   /** Max number of slots in the storage unit for storing items. For now, its best to keep this left than the default storage hud slot number (36 I believe).*/
   UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = "10", ClampMax = "36"))
   int maxStorage = 10;

   /**List of items that are initially in the storage unit*/
   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
   TArray<FMyItem> initialItems;

   /*Sphere collision to determine if the hero has walked out of range of the storage container*/
   UPROPERTY(VisibleAnywhere)
   USphereComponent* sphereCollision;

 private:
   UFUNCTION()
   void OnLeaveRange(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int otherBodyIndex);

   AUserInput* GetPlayerControllerRef() const;

   TScriptInterface<IWidgetToggler> GetWidgetToggler() const;
   TScriptInterface<IHUDProvider>   GetHUDProvider() const;

   UPROPERTY()
   UBackpack* backpack = nullptr;
};
